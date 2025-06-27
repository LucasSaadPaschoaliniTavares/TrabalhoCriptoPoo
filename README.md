FT_Coin: Sistema de Gerenciamento de Carteira

Um sistema de gerenciamento de carteira de moedas desenvolvido em C++. Versão 1.0.0

Requisitos
Compilador C++:

Um compilador com suporte ao padrão C++11 (ex: g++).

MariaDB Connector/C++:

Biblioteca para conexão com o banco de dados MariaDB. O projeto foi testado com a versão 1.1.6.

Para instalar (Debian/Ubuntu via arquivo .deb):

a. Baixe o arquivo .deb compatível com a sua distribuição a partir do site oficial da MariaDB.

b. Navegue até o seu diretório de downloads no terminal.

c. Instale o pacote usando dpkg. Por exemplo:


sudo dpkg -i mariadb-connector-cpp_1.1.6-1+maria~noble_amd64.deb
d. Corrija quaisquer dependências que possam ter faltado com o apt:


sudo apt-get install -f
Como Compilar e Executar
O projeto inclui um script de Shell chamado build.sh para automatizar todo o processo de compilação.

1. Verifique o Script build.sh

Garanta que o arquivo build.sh na raiz do seu projeto tenha o seguinte conteúdo :


#!/bin/bash
echo ">>> Iniciando a compilação..."

g++ \
    -w \
    -std=c++11 \
    -I/usr/include/mariadb \
    -Iinclude \
    src/*.cpp \
    -lmariadbcpp \
    -o carteiraMovimentacaoApp

if [ $? -eq 0 ]; then
    echo ">>> Sucesso! Executável 'carteiraMovimentacaoApp' criado."
    echo ">>> Para executar, use: ./carteiraMovimentacaoApp"
else
    echo ">>> ERRO: Falha na compilação."
fi

2. Dê Permissão de Execução

No terminal, execute este comando apenas uma vez para tornar o script executável:

chmod +x build.sh

3. Compile o Projeto

Agora, para compilar, basta executar o script:

./build.sh

4. Execute o Programa

Após a compilação bem-sucedida, rode o programa com:

./carteiraMovimentacaoApp


Funcionalidades
Gerenciamento de Carteira

Incluir nova carteira

Editar carteira existente

Excluir carteira

Listar carteiras (ordenadas por ID ou nome)

Movimentações

Registrar compras de moeda

Registrar vendas de moeda

Relatórios

Exibir saldo atual de uma carteira

Exibir histórico de movimentações

Apresentar ganho ou perda total realizado

Autores e Licença
Programa criado pelo grupo composto por:

Lucas Gabriel

Lucas SaaD

Gabriel Gaudio

João Yokoyama

Desenvolvido na Universidade de Campinas - UNICAMP, 1º semestre de 2025.

© 2025 FT_Coin. Todos os direitos reservados.
