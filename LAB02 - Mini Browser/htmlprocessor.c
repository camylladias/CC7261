#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/stat.h>
#include <netdb.h>
#include <unistd.h>

#define MAX_PATH_LENGTH 256

typedef struct {
    char path[MAX_PATH_LENGTH];
} file_info;

void *get_file_size(void *arg) {
    file_info *info = (file_info *)arg;
    struct stat st;
    
    if (stat(info->path, &st) == 0) {
        printf("O arquivo %s tem %zu bytes.\n", info->path, st.st_size);
    } else {
        perror("Erro ao obter o tamanho do arquivo");
    }

    free(info);
    return NULL;
}

void *get_hostname_ip(void *arg) {
    char *hostname = (char *)arg;
    struct addrinfo hints, *res;
    int status;
    char ipstr[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // Both IPv4 and IPv6
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(hostname, NULL, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return NULL;
    }

    for (struct addrinfo *p = res; p != NULL; p = p->ai_next) {
        void *addr;
        if (p->ai_family == AF_INET) { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &ipv4->sin_addr;
        } else { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &ipv6->sin6_addr;
        }

        inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
        printf("%s IP address: %s\n", hostname, ipstr);
        break; // If you just want the first IP, then break here
    }

    freeaddrinfo(res); // Free the linked list
    free(hostname);
    return NULL;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <arquivo_html>\n", argv[0]);
        return 1;
    }

    char *html_filename = argv[1];
    FILE *html_file = fopen(html_filename, "r");
    if (!html_file) {
        perror("Erro ao abrir o arquivo HTML");
        return 1;
    }

    char line[1024];
    pthread_t thread;
    file_info *info;
    char *hostname = NULL;

    while (fgets(line, sizeof(line), html_file)) {
        if (strstr(line, "<link") || strstr(line, "<img") || strstr(line, "<script")) {
            info = malloc(sizeof(file_info));
            char *token = strtok(line, "\"");
            while (token != NULL) {
                if (strcmp(token, " href=") == 0 || strcmp(token, " src=") == 0) {
                    token = strtok(NULL, "\"");
                    strcpy(info->path, token);
                    pthread_create(&thread, NULL, get_file_size, info);
                    pthread_detach(thread);
                    break;
                }
                token = strtok(NULL, "\"");
            }
        } else if (strstr(line, "<security")) {
            // Assuming <security cert="https://letsencrypt.org/"></security>
            hostname = malloc(MAX_PATH_LENGTH);
            sscanf(line, "<security cert=\"%255[^\"]", hostname);
            pthread_create(&thread, NULL, get_hostname_ip, hostname);
            pthread_detach(thread);
        }
    }

    fclose(html_file);

    sleep(2); // Wait for threads to finish; replace with pthread_join in real code

    return 0;
}
