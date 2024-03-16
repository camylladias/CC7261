import java.io.*;
import java.net.*;
import java.nio.file.*;
import java.util.regex.*;

public class HtmlParser {
    public static void main(String[] args) throws Exception {
        if (args.length != 1) {
            System.out.println("Informe o arquivo html, especiamente dentro desta pasta.");
            return;
        }

        String content = new String(Files.readAllBytes(Paths.get(args[0])));
        processTags(content, "<link\\s+(?:.*?\\s)?href=\"([^\"]*)\"", "Link");
        processTags(content, "<img\\s+(?:.*?\\s)?src=\"([^\"]*)\"", "Image");
        processTags(content, "<script\\s+(?:.*?\\s)?src=\"([^\"]*)\"", "Script");
        processTags(content, "<security\\s+(?:.*?\\s)?href=\"([^\"]*)\"", "Security");
    }

    private static void processTags(String content, String regex, String tagType) {
        Pattern pattern = Pattern.compile(regex);
        Matcher matcher = pattern.matcher(content);

        while (matcher.find()) {
            String filePath = matcher.group(1);
            if (tagType.equals("Security")) {
                new Thread(() -> imprimeIP(filePath)).start();
            } else {
                new Thread(() -> imprimeTamanhoArquivo(filePath)).start();
            }
        }
    }

    private static void imprimeTamanhoArquivo(String filePath) {
        Path path = Paths.get(filePath);
        try {
            long bytes = Files.size(path);
            System.out.println("Tamanho do arquivo: " + filePath + " é " + bytes + " bytes.");
        } catch (IOException e) {
            System.out.println("Nao foi possivel calcular " + filePath + ": " + e.getMessage());
        }
    }

    private static void imprimeIP(String url) {
        try {
            URL urlObject = new URL(url);
            InetAddress address = InetAddress.getByName(urlObject.getHost());
            System.out.println("O endereco de IP do hostname: " + url + " é "+ address.getHostAddress());
        } catch (UnknownHostException e) {
            System.out.println("Nao foi possivel obter o IP " + url + ": " + e.getMessage());
        } catch (MalformedURLException e) {
            System.out.println("Malformed URL: " + url);
        }
    }
}
