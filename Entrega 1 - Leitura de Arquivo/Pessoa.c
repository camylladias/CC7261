//Camylla Lima Dias 22217005-2 Turma 760

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nome[15];
    int idade;
    float altura;
} Pessoa;

int lerPessoas(Pessoa **pessoas, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        printf("Erro em abrir o arquivo.\n");
        return 0;
    }

    Pessoa p;
    int quant = 0;
    while (fscanf(arquivo, "%14s %d %f\n", p.nome, &p.idade, &p.altura) == 3) {
        quant++;
    }
    rewind(arquivo);

    *pessoas = (Pessoa *)malloc(quant * sizeof(Pessoa));
    if (*pessoas == NULL) {
        printf("Erro malloc.\n");
        fclose(arquivo);
        return 0;
    }

    for (int i=0; i<quant && fscanf(arquivo, "%14s %d %f\n", (*pessoas)[i].nome, &(*pessoas)[i].idade, &(*pessoas)[i].altura) == 3; i++);
    
    fclose(arquivo);
    return quant;
}

int comparaAltura(const void *a, const void *b) {
    Pessoa *pessoaA = (Pessoa *)a;
    Pessoa *pessoaB = (Pessoa *)b;
    return (pessoaA->altura > pessoaB->altura) - (pessoaA->altura < pessoaB->altura);
}

void ordena(Pessoa *pessoas, int quantidade) {
    qsort(pessoas, quantidade, sizeof(Pessoa), comparaAltura);
}

void arquivoOrdenado(Pessoa *pessoas, int quantidade, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    for (int i = 0; i < quantidade; i++) {
        fprintf(arquivo, "%s %d %.1f\n", pessoas[i].nome, pessoas[i].idade, pessoas[i].altura);
    }

    fclose(arquivo);
}

int main() {
    Pessoa *pessoas = NULL;
    int quantidade = lerPessoas(&pessoas, "pessoas.txt");

    ordena(pessoas, quantidade);
    arquivoOrdenado(pessoas, quantidade, "pessoas-ordenadas-altura.txt");

    free(pessoas);
    return 0;
}