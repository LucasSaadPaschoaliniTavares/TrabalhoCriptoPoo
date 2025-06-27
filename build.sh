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