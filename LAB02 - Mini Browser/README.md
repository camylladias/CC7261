# Threads: HTML Parser 🧵🐱
###### Disciplina: Sistemas Distribuídos (CC7261) - Ciência da Computação - FEI

Este projeto em Java lê um arquivo HTML fornecido e executa ações específicas em threads separadas para tags `<link>`, `<img>`, `<script>` e `<security>`. <p>
![enter image description here](https://github.com/camylladias/randomimages/raw/main/akirathecat.png?raw=true)
[Akira The Cat](https://camylladias.w3spaces.com/)

## Descrição

O programa analisa um arquivo HTML e procura por tags específicas. Para cada tag `<link>`, `<img>` e `<script>` encontrada, uma nova thread é iniciada para calcular o tamanho do arquivo referenciado. Para a tag `<security>`, uma thread separada é iniciada para resolver o endereço IP do hostname especificado no atributo `href`.

## Funcionalidades

- Análise de arquivos HTML para identificar tags específicas.
- Processamento multithread.
- Exibição do tamanho do arquivo para as tags `<link>`, `<img>` e `<script>`.
- Resolução e exibição do endereço IP para o hostname especificado na tag `<security>`.

## Uso

1. Compile o código usando `javac HtmlParser.java`.
2. Execute o programa com `java HtmlParser index.html`.

