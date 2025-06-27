#include "Controller.h"

// Includes da Biblioteca Padrão
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <iomanip>
#include <algorithm>
#include <memory>

// Includes do Projeto
#include "Menu.h"
#include "Carteira.h"
#include "Movimentacao.h"
#include "Oraculo.h"
#include "CarteiraDAO_Memory.h"
#include "MovimentacaoDAO_Memory.h"
#include "OraculoDAO_Memory.h"
#include "SysInfo.h"
#include "TextFromFile.h"
#include "Utils.h"
#include "CarteiraDAO_DB.h"
#include "MovimentacaoDAO_DB.h"
#include "OraculoDAO_DB.h"
#include "ServerDBConnection.h"

using namespace std;

// --- Construtor e Destrutor ---

Controller::Controller(DataBaseSelector selector) : dbConnection(nullptr) {
    switch (selector) {
        case DataBaseSelector::MEMORY:
            this->carteiraDao = new CarteiraDAO_Memory();
            this->movDao = new MovimentacaoDAO_Memory();
            this->oraculoDao = new OraculoDAO_Memory();
            break;

        case DataBaseSelector::MARIADB:
            {
                this->dbConnection = new ServerDBConnection();
                this->carteiraDao = new CarteiraDAO_DB(this->dbConnection);
                this->movDao = new MovimentacaoDAO_DB(this->dbConnection);
                this->oraculoDao = new OraculoDAO_DB(this->dbConnection);
                cout << "DAOs configurados para MariaDB com sucesso.\n";
            }
            break;

        default:
            throw std::invalid_argument("Modelo de persistência selecionado não está disponível.");
    }
}

Controller::~Controller() {
    delete carteiraDao;
    delete movDao;
    delete oraculoDao;

    if (dbConnection != nullptr) {
        delete dbConnection;
    }
}

// --- Lógica de Execução e Menus ---

void Controller::executar() {
    vector<string> principalOpcoes = { "Carteira", "Movimentacao", "Relatórios", "Ajuda", "Sair" };
    vector<void (Controller::*)()> principalFunctions = {
        &Controller::_processarMenuCarteira,
        &Controller::_processarMenuMovimentacao,
        &Controller::_processarMenuRelatorio,
        &Controller::_processarMenuAjuda
    };
    launchActions("Menu Principal", principalOpcoes, principalFunctions);
    Utils::limparTela();
    cout << "Saindo do sistema...\n";
}

void Controller::launchActions(string title, vector<string> menuItens, vector<void (Controller::*)()> functions) {
    try {
        Menu menu(menuItens, title, "Sua opção: ");
        menu.setSymbol("*");

        int choice;
        do {
            Utils::limparTela();
            choice = menu.getChoice();

            if (choice > 0 && (size_t)(choice - 1) < functions.size()) {
                Utils::limparTela();
                (this->*functions.at(choice - 1))();
            } else if (choice != 0) {
                Utils::limparTela();
                cout << "Opcao invalida! Por favor, tente novamente.\n";
                Utils::pausar();
            }
        } while (choice != 0);
    }
    catch (const exception &myException) {
        Utils::printMessage("Unexpected problem launching actions. " + string(myException.what()));
    }
}

// --- Processadores de Submenus ---

void Controller::_processarMenuCarteira() {
    vector<string> carteiraOpcoes = { "Incluir carteira", "Listar carteiras", "Editar carteira", "Excluir carteira", "Voltar" };
    vector<void (Controller::*)()> carteiraFunctions = {
        &Controller::_incluirCarteira,
        &Controller::_listarCarteiras,
        &Controller::_editarCarteira,
        &Controller::_excluirCarteira
    };
    launchActions("Menu Carteira", carteiraOpcoes, carteiraFunctions);
}

void Controller::_processarMenuMovimentacao() {
    vector<string> movimentacaoOpcoes = { "Registrar compra", "Registrar venda", "Voltar" };
    vector<void (Controller::*)()> movimentacaoFunctions = {
        &Controller::_registrarCompra,
        &Controller::_registrarVenda
    };
    launchActions("Menu Movimentacao", movimentacaoOpcoes, movimentacaoFunctions);
}

void Controller::_processarMenuRelatorio() {
    vector<string> relatorioOpcoes = { "Listar por ID", "Listar por Nome", "Saldo da carteira", "Histórico", "Ganho/Perda Total", "Voltar" };
    vector<void (Controller::*)()> relatorioFunctions = {
        &Controller::listarPorId,
        &Controller::listarPorNome,
        &Controller::exibirSaldo,
        &Controller::exibirHistorico,
        &Controller::exibirGanhoPerdaTotal
    };
    launchActions("Menu Relatórios", relatorioOpcoes, relatorioFunctions);
}

void Controller::_processarMenuAjuda() {
    vector<string> ajudaOpcoes = { "Exibir texto de ajuda", "Créditos", "Voltar" };
    vector<void (Controller::*)()> ajudaFunctions = {
        &Controller::_mostrarTextoDeAjuda,
        &Controller::_mostrarCreditos
    };
    launchActions("Menu de Ajuda", ajudaOpcoes, ajudaFunctions);
}

// --- Ações do Menu Carteira ---

void Controller::_incluirCarteira() {
    cout << "=== Incluir Nova Carteira ===\n";
    cout << "Nome do titular: ";
    string nomeTitular;
    getline(cin, nomeTitular);

    cout << "Corretora: ";
    string corretora;
    getline(cin, corretora);


    carteiraDao->incluir(Carteira(0, nomeTitular, corretora));
    cout << "Carteira incluída com sucesso!\n";
    Utils::pausar();
}

void Controller::_listarCarteiras() {
    cout << "=== Lista de Carteiras ===\n";
    vector<Carteira> lista = carteiraDao->listar();
    if (lista.empty()) {
        cout << "Nenhuma carteira cadastrada.\n";
    } else {
        for (const auto& c : lista) {
            cout << "ID: " << c.getId()
                 << " | Titular: " << c.getNomeTitular()
                 << " | Corretora: " << c.getCorretora() << '\n';
        }
    }
    Utils::pausar();
}

void Controller::_editarCarteira() {
    cout << "=== Editar Carteira ===\n";
    cout << "Digite o ID da carteira: ";
    int id;
    cin >> id;
    cin.ignore();

    Carteira* c = carteiraDao->recuperar(id);

    if (c == nullptr) {
        cout << "Carteira não encontrada.\n";
    } else {
        cout << "Nome atual: " << c->getNomeTitular() << "\n";
        cout << "Novo nome (deixe em branco para manter o atual): ";
        string novoNome;
        getline(cin, novoNome);
        if (!novoNome.empty()) {
            c->setNomeTitular(novoNome);
        }

        cout << "Corretora atual: " << c->getCorretora() << "\n";
        cout << "Nova corretora (deixe em branco para manter a atual): ";
        string novaCorretora;
        getline(cin, novaCorretora);
        if (!novaCorretora.empty()) {
            c->setCorretora(novaCorretora);
        }


        if (carteiraDao->editar(*c)) {
            cout << "Carteira editada com sucesso!\n";
        } else {
            cout << "Erro ao salvar as alterações da carteira.\n";
        }
    }
    Utils::pausar();
}

void Controller::_excluirCarteira() {
    cout << "=== Excluir Carteira ===\n";
    cout << "Digite o ID da carteira: ";
    int id;
    cin >> id;
    cin.ignore();
    if (carteiraDao->excluir(id)) {
        cout << "Carteira excluída com sucesso!\n";
    } else {
        cout << "Carteira não encontrada.\n";
    }
    Utils::pausar();
}

// --- Ações do Menu Movimentação ---

void Controller::_registrarCompra() {
    cout << "=== Registrar Compra de Moeda ===\n";
    cout << "ID da carteira: ";
    int carteiraId;
    cin >> carteiraId;
    cin.ignore();
    if (carteiraDao->recuperar(carteiraId) == nullptr) {
        cout << "Carteira não encontrada!\n";
    } else {
        cout << "Quantidade: ";
        double qtd;
        cin >> qtd;
        cin.ignore();
        string data;
        cout << "Data (AAAA-MM-DD): ";
        getline(cin, data);
        movDao->incluir(Movimentacao(0, carteiraId, Movimentacao::COMPRA, qtd, data));
        cout << "Compra registrada com sucesso!\n";
    }
    Utils::pausar();
}

void Controller::_registrarVenda() {
    cout << "=== Registrar Venda de Moeda ===\n";
    cout << "ID da carteira: ";
    int carteiraId;
    cin >> carteiraId;
    cin.ignore();
    if (carteiraDao->recuperar(carteiraId) == nullptr) {
        cout << "Carteira não encontrada!\n";
    } else {
        cout << "Quantidade: ";
        double qtd;
        cin >> qtd;
        cin.ignore();
        string data;
        cout << "Data (AAAA-MM-DD): ";
        getline(cin, data);
        movDao->incluir(Movimentacao(0, carteiraId, Movimentacao::VENDA, qtd, data));
        cout << "Venda registrada com sucesso!\n";
    }
    Utils::pausar();
}

// --- Ações do Menu Ajuda ---

void Controller::_mostrarTextoDeAjuda() {
    Utils::printMessage(SysInfo::getFullVersion() + " | Help");
    unique_ptr<TextFromFile> textFromFile(new TextFromFile(SysInfo::getHelpFile()));
    Utils::printFramedMessage(textFromFile->getFileContent(), "*", 120);
    Utils::pausar();
}

void Controller::_mostrarCreditos() {
    Utils::printMessage("Sobre o " + SysInfo::getSystemName());
    string text;
    text += SysInfo::getFullVersion() + "\n";
    text += "Autores: " + SysInfo::getAuthor() + "\n";
    text += "Instituicao: " + SysInfo::getInstitution() + "\n";
    text += "Copyright (C) " + SysInfo::getDate() + " " + SysInfo::getAuthor() + "\n";
    Utils::printFramedMessage(text, "*", 80);
    Utils::pausar();
}

// --- Ações do Menu Relatórios ---

void Controller::listarPorId() {
    cout << "=== Carteiras Ordenadas por ID ===\n";
    vector<Carteira> lista = carteiraDao->listar();
    sort(lista.begin(), lista.end(), [](const Carteira& a, const Carteira& b) {
        return a.getId() < b.getId();
    });
    for (const auto& c : lista) {
        cout << "ID: " << c.getId() << " | Titular: " << c.getNomeTitular() << '\n';
    }
    Utils::pausar();
}

void Controller::listarPorNome() {
    cout << "=== Carteiras Ordenadas por Nome ===\n";
    vector<Carteira> lista = carteiraDao->listar();
    sort(lista.begin(), lista.end(), [](const Carteira& a, const Carteira& b) {
        return a.getNomeTitular() < b.getNomeTitular();
    });
    for (const auto& c : lista) {
        cout << "ID: " << c.getId() << " | Titular: " << c.getNomeTitular() << '\n';
    }
    Utils::pausar();
}

void Controller::exibirSaldo() {
    cout << "=== Saldo da Carteira ===\n";
    cout << "Digite o ID da carteira: ";
    int id;
    cin >> id;
    cin.ignore();
    Carteira* c = carteiraDao->recuperar(id);
    if (!c) {
        cout << "Carteira não encontrada.\n";
    } else {
        vector<Movimentacao> movimentacoes = movDao->listarPorCarteira(id);
        double saldo = 0.0;
        for (const auto& m : movimentacoes) {
            saldo += (m.getTipo() == Movimentacao::COMPRA ? m.getQuantidade() : -m.getQuantidade());
        }
        cout << "Saldo atual (em quantidade de moeda) da carteira \"" << c->getNomeTitular() << "\": " << saldo << "\n";
    }
    Utils::pausar();
}

void Controller::exibirHistorico() {
    cout << "=== Histórico da Carteira ===\n";
    cout << "Digite o ID da carteira: ";
    int id;
    cin >> id;
    cin.ignore();
    Carteira* c = carteiraDao->recuperar(id);
    if (!c) {
        cout << "Carteira não encontrada.\n";
    } else {
        cout << "Movimentações da carteira \"" << c->getNomeTitular() << "\":\n";
        vector<Movimentacao> movimentacoes = movDao->listarPorCarteira(id);
        if (movimentacoes.empty()) {
            cout << "Nenhuma movimentação encontrada para esta carteira.\n";
        } else {
            for (const auto& m : movimentacoes) {
                cout << (m.getTipo() == Movimentacao::COMPRA ? "Compra" : "Venda")
                     << " - Qtde: " << m.getQuantidade()
                     << ", Data: " << m.getData() << '\n';
            }
        }
    }
    Utils::pausar();
}

void Controller::exibirGanhoPerdaTotal() {
    cout << "=== Ganho ou Perda Total das Carteiras (baseado no Oráculo) ===\n";
    vector<Carteira> carteiras = carteiraDao->listar();
    if (carteiras.empty()) {
        cout << "Nenhuma carteira cadastrada para gerar relatório.\n";
    } else {
        for (const auto& c : carteiras) {
            vector<Movimentacao> movimentacoes = movDao->listarPorCarteira(c.getId());
            double totalGastoEmCompras = 0.0;
            double totalRecebidoDeVendas = 0.0;

            for (const auto& m : movimentacoes) {
                Oraculo* oraculoDoDia = oraculoDao->recuperar(m.getData());
                if (!oraculoDoDia) {
                    cout << "[AVISO] Carteira '" << c.getNomeTitular()
                         << "': Sem cotação no Oráculo para a data " << m.getData()
                         << ". Movimentação ignorada no cálculo.\n";
                    continue;
                }
                double cotacaoDoDia = oraculoDoDia->getCotacao();
                double valorDaTransacao = m.getQuantidade() * cotacaoDoDia;

                if (m.getTipo() == Movimentacao::COMPRA) {
                    totalGastoEmCompras += valorDaTransacao;
                } else {
                    totalRecebidoDeVendas += valorDaTransacao;
                }
            }
            double ganhoPerda = totalRecebidoDeVendas - totalGastoEmCompras;
            cout << "Carteira: " << c.getNomeTitular()
                 << " | Ganho/Perda Realizado: " << fixed << setprecision(2) << ganhoPerda << '\n';
        }
    }
    Utils::pausar();
}