
#include "Controller.h"
#include "Menu.h"
#include "Relatorio.h"
#include "Carteira.h"
#include "Movimentacao.h"
#include "Oraculo.h"
#include "CarteiraDAO_Memory.h"
#include "MovimentacaoDAO_Memory.h"
#include "OraculoDAO_Memory.h"

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <random>
#include <iomanip>
#include <sstream>
#include <ctime>

using namespace std;

// --- Função Auxiliar Interna ---
double gerarCotacaoAleatoria(double min, double max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> distrib(min, max);
    return distrib(gen);
}


// --- Implementação do Controller ---

Controller::Controller(DataBaseSelector selector) {
    switch (selector) {
    case DataBaseSelector::MEMORY:
        this->carteiraDao = new CarteiraDAO_Memory();
        this->movDao = new MovimentacaoDAO_Memory();
        this->oraculoDao = new OraculoDAO_Memory();
        _popularOraculo(); // Popular oraculo só faz sentido para a memória
        break;


        /* case DataBaseSelector::MARIADB:
            {
                ServerDBConnection* conn = new ServerDBConnection();
                this->carteiraDao = new CarteiraDAO_DB(conn);
                this->movDao = new MovimentacaoDAO_DB(conn);
                this->oraculoDao = new OraculoDAO_DB(conn);
            }
            break;
        */


    default:
            if (selector != DataBaseSelector::MEMORY) {
                throw std::invalid_argument("Modelo de persistência selecionado não está disponível.");
            }
        break;
    }
}

Controller::~Controller() {
    delete carteiraDao;
    delete movDao;
    delete oraculoDao;
}

void Controller::_popularOraculo() {
    cout << "Gerando cotações aleatórias para o Oráculo...\n";
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    int anoAtual = now->tm_year + 1900;
    int mesAtual = now->tm_mon + 1;

    for (int dia = 1; dia <= 30; ++dia) {
        stringstream ss;
        ss << anoAtual << "-" << setfill('0') << setw(2) << mesAtual << "-" << setfill('0') << setw(2) << dia;
        oraculoDao->incluir(Oraculo(ss.str(), gerarCotacaoAleatoria(60000.0, 75000.0)));
    }
    cout << "Oráculo populado com sucesso!\n\n";
}

void Controller::executar() {
    vector<string> principalOpcoes = {"Carteira", "Movimentacao", "Relatórios", "Sair"};

    while (true) {
        Menu menuPrincipal(principalOpcoes, "Menu Principal", "Escolha uma opcao:");
        menuPrincipal.setZeroForLastOpt(true);
        int escolha = menuPrincipal.getChoice();

        if (escolha == 0 || escolha == 4) { cout << "Saindo...\n"; break; }

        switch (escolha) {
            case 1: _processarMenuCarteira(); break;
            case 2: _processarMenuMovimentacao(); break;
            case 3: _processarMenuRelatorio(); break;
            default: cout << "Opcao invalida!\n\n";
        }
    }
}

void Controller::_processarMenuCarteira() {
    vector<string> carteiraOpcoes = {"Incluir carteira", "Listar carteiras", "Editar carteira", "Excluir carteira", "Voltar"};
    while (true) {
        Menu menuCarteira(carteiraOpcoes, "Menu Carteira", "Escolha uma opcao:");
        menuCarteira.setZeroForLastOpt(true);
        int escolha = menuCarteira.getChoice();

        if (escolha == 0 || escolha == 5) break;

        switch (escolha) {
            case 1: _incluirCarteira(); break;
            case 2: _listarCarteiras(); break;
            case 3: _editarCarteira(); break;
            case 4: _excluirCarteira(); break;
            default: cout << "Opcao invalida!\n\n";
        }
    }
}

void Controller::_processarMenuMovimentacao() {
    vector<string> movimentacaoOpcoes = {"Registrar compra", "Registrar venda", "Voltar"};
    while (true) {
        Menu menuMovimentacao(movimentacaoOpcoes, "Menu Movimentacao", "Escolha uma opcao:");
        menuMovimentacao.setZeroForLastOpt(true);
        int escolha = menuMovimentacao.getChoice();

        if (escolha == 0 || escolha == 3) break;

        switch (escolha) {
            case 1: _registrarCompra(); break;
            case 2: _registrarVenda(); break;
            default: cout << "Opcao invalida!\n\n";
        }
    }
}

void Controller::_processarMenuRelatorio() {
    vector<string> relatorioOpcoes = {"Listar por ID", "Listar por Nome", "Saldo da carteira", "Histórico", "Ganho/Perda Total", "Voltar"};
    while (true) {
        Menu menuRelatorio(relatorioOpcoes, "Menu Relatórios", "Escolha uma opcao:");
        menuRelatorio.setZeroForLastOpt(true);
        int escolha = menuRelatorio.getChoice();

        if (escolha == 0 || escolha == 6) break;

        switch (escolha) {
            case 1: listarPorId(*carteiraDao); break;
            case 2: listarPorNome(*carteiraDao); break;
            case 3: exibirSaldo(*carteiraDao, *movDao); break;
            case 4: exibirHistorico(*movDao, *carteiraDao); break;
            case 5: exibirGanhoPerdaTotal(*carteiraDao, *movDao, *oraculoDao); break;
            default: cout << "Opcao invalida!\n\n"; break;
        }
    }
}

void Controller::_incluirCarteira() {
    cout << "=== Incluir nova carteira ===\n";
    cout << "Nome do titular: ";
    string nome;
    getline(cin, nome);
    carteiraDao->incluir(Carteira(0, nome, "Default Broker"));
    cout << "Carteira incluída com sucesso!\n\n";
}

void Controller::_listarCarteiras() {
    cout << "=== Lista de carteiras ===\n";
    auto lista = carteiraDao->listar();
    if (lista.empty()) { cout << "Nenhuma carteira cadastrada.\n"; }
    else {
        for (const auto& c : lista) {
            cout << "ID: " << c.getId() << " | Titular: " << c.getNomeTitular() << '\n';
        }
    }
    cout << endl;
}

void Controller::_editarCarteira() {
    cout << "=== Editar carteira ===\n";
    cout << "Digite o ID da carteira: ";
    int id; cin >> id; cin.ignore();
    Carteira* c = carteiraDao->recuperar(id);
    if (c == nullptr) { cout << "Carteira não encontrada.\n\n"; return; }
    cout << "Nome atual: " << c->getNomeTitular() << "\n";
    cout << "Novo nome: ";
    string novoNome; getline(cin, novoNome);
    c->setNomeTitular(novoNome);
    carteiraDao->editar(*c);
    cout << "Carteira editada com sucesso!\n\n";
}

void Controller::_excluirCarteira() {
    cout << "=== Excluir carteira ===\n";
    cout << "Digite o ID da carteira: ";
    int id; cin >> id; cin.ignore();
    if (carteiraDao->excluir(id)) { cout << "Carteira excluída com sucesso!\n\n"; }
    else { cout << "Carteira não encontrada.\n\n"; }
}

void Controller::_registrarCompra() {
    cout << "Registrar compra de moeda virtual\n";
    cout << "ID da carteira: ";
    int carteiraId; cin >> carteiraId; cin.ignore();
    if (carteiraDao->recuperar(carteiraId) == nullptr) { cout << "Carteira nao encontrada!\n\n"; return; }
    cout << "Quantidade: ";
    double qtd; cin >> qtd; cin.ignore();
    string data; cout << "Data (AAAA-MM-DD): "; getline(cin, data);
    movDao->incluir(Movimentacao(0, carteiraId, Movimentacao::COMPRA, qtd, data));
    cout << "Compra registrada com sucesso!\n\n";
}

void Controller::_registrarVenda() {
    cout << "Registrar venda de moeda virtual\n";
    cout << "ID da carteira: ";
    int carteiraId; cin >> carteiraId; cin.ignore();
    if (carteiraDao->recuperar(carteiraId) == nullptr) { cout << "Carteira nao encontrada!\n\n"; return; }
    cout << "Quantidade: ";
    double qtd; cin >> qtd; cin.ignore();
    string data; cout << "Data (AAAA-MM-DD): "; getline(cin, data);
    movDao->incluir(Movimentacao(0, carteiraId, Movimentacao::VENDA, qtd, data));
    cout << "Venda registrada com sucesso!\n\n";
}