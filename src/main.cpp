

#include <iostream>
#include <exception>
#include <memory>

#include "Controller.h"
#include "DataBaseSelector.h"


void welcome() {
    std::cout << "--- Meu App de Carteira de Cripto ---\n";
    std::cout << "Iniciando...\n";
}


void bye() {
    std::cout << "\nPrograma finalizado.\n";
}



#include <limits>

DataBaseSelector escolherModoDePersistencia() {
    int escolha = 0;
    while (true) {
        std::cout << "--- Configuração de Persistência ---\n";
        std::cout << "Escolha o modo de armazenamento de dados:\n";
        std::cout << "1 - Em Memória (rápido, os dados são perdidos ao fechar)\n";
        std::cout << "2 - Banco de Dados MariaDB (persistente, requer conexão)\n";
        std::cout << "Sua opção: ";

        std::cin >> escolha;

        // Verifica se a entrada foi um número válido
        if (std::cin.fail()) {
            std::cout << "\nEntrada inválida. Por favor, digite 1 ou 2.\n\n";
            std::cin.clear(); // Limpa o estado de erro do cin
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Descarta a entrada ruim
            continue;
        }

        if (escolha == 1) {
            return DataBaseSelector::MEMORY;
        } else if (escolha == 2) {
            return DataBaseSelector::MARIADB;
        } else {
            std::cout << "\nOpção inválida. Tente novamente.\n\n";
        }
    }
}




int main() {
    welcome();

    try {

        DataBaseSelector modoEscolhido = escolherModoDePersistencia();


        std::unique_ptr<Controller> controller(new Controller(modoEscolhido));


        controller->executar();

    } catch (const std::exception& e) {
        std::cerr << "\nERRO CRÍTICO: O programa foi encerrado de forma inesperada.\n";
        std::cerr << "Motivo: " << e.what() << std::endl;
        return 1;
    }

    bye();
    return 0;
}