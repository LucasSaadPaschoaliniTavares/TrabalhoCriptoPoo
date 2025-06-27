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
        std::cout << "1 - Em Memória (Os dados são perdidos ao fechar)\n";
        std::cout << "2 - Banco de Dados MariaDB (Persistente, requer conexão)\n";
        std::cout << "Sua opção: ";

        std::cin >> escolha;

        if (std::cin.fail()) {
            std::cout << "\nEntrada inválida. Por favor, digite 1 ou 2.\n\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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