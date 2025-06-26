#include "Controller.h"
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
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <random>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <memory>

using namespace std;

double gerarCotacaoAleatoria(double min, double max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> distrib(min, max);
    return distrib(gen);
}


Controller::Controller(DataBaseSelector selector) : dbConnection(nullptr) {
    switch (selector) {
    case DataBaseSelector::MEMORY:
        this->carteiraDao = new CarteiraDAO_Memory();
        this->movDao = new MovimentacaoDAO_Memory();
        this->oraculoDao = new OraculoDAO_Memory();
        _popularOraculo();
        break;
    case DataBaseSelector::MARIADB:
      {
            this->dbConnection = new ServerDBConnection();
            this->carteiraDao = new CarteiraDAO_DB(this->dbConnection);
            this->movDao = new MovimentacaoDAO_DB(this->dbConnection);
            this->oraculoDao = new OraculoDAO_DB(this->dbConnection);
            std::cout << "DAOs configurados para MariaDB com sucesso.\n";
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
    vector<string> principalOpcoes = {"Carteira", "Movimentacao", "Relatórios", "Ajuda", "Sair"};
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


void Controller::_processarMenuCarteira() {
    vector<string> carteiraOpcoes = {"Incluir carteira", "Listar carteiras", "Editar carteira", "Excluir carteira", "Voltar"};
    vector<void (Controller::*)()> carteiraFunctions = { &Controller::_incluirCarteira, &Controller::_listarCarteiras, &Controller::_editarCarteira, &Controller::_excluirCarteira };
    launchActions("Menu Carteira", carteiraOpcoes, carteiraFunctions);
}
void Controller::_processarMenuMovimentacao() {
    vector<string> movimentacaoOpcoes = {"Registrar compra", "Registrar venda", "Voltar"};
    vector<void (Controller::*)()> movimentacaoFunctions = { &Controller::_registrarCompra, &Controller::_registrarVenda };
    launchActions("Menu Movimentacao", movimentacaoOpcoes, movimentacaoFunctions);
}
void Controller::_processarMenuRelatorio() {
    vector<string> relatorioOpcoes = {"Listar por ID", "Listar por Nome", "Saldo da carteira", "Histórico", "Ganho/Perda Total", "Voltar"};
    vector<void (Controller::*)()> relatorioFunctions = { &Controller::listarPorId, &Controller::listarPorNome, &Controller::exibirSaldo, &Controller::exibirHistorico, &Controller::exibirGanhoPerdaTotal };
    launchActions("Menu Relatórios", relatorioOpcoes, relatorioFunctions);
}
void Controller::_processarMenuAjuda() {
    std::vector<std::string> ajudaOpcoes = {"Exibir texto de ajuda", "Créditos", "Voltar"};
    std::vector<void (Controller::*)()> ajudaFunctions = { &Controller::_mostrarTextoDeAjuda, &Controller::_mostrarCreditos };
    launchActions("Menu de Ajuda", ajudaOpcoes, ajudaFunctions);
}

void Controller::_incluirCarteira() {
    cout << "=== Incluir nova carteira ===\n";
    cout << "Nome do titular: ";
    string nome;
    getline(cin, nome);
    carteiraDao->incluir(Carteira(0, nome, "Default Broker"));
    cout << "Carteira incluída com sucesso!\n";
    Utils::pausar();
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
    Utils::pausar();
}

void Controller::_editarCarteira() {
    cout << "=== Editar carteira ===\n";
    cout << "Digite o ID da carteira: ";
    int id; cin >> id; cin.ignore();
    Carteira* c = carteiraDao->recuperar(id);
    if (c == nullptr) { cout << "Carteira não encontrada.\n"; }
    else {
        cout << "Nome atual: " << c->getNomeTitular() << "\n";
        cout << "Novo nome: ";
        string novoNome; getline(cin, novoNome);
        c->setNomeTitular(novoNome);
        carteiraDao->editar(*c);
        cout << "Carteira editada com sucesso!\n";
    }
    Utils::pausar();
}

void Controller::_excluirCarteira() {
    cout << "=== Excluir carteira ===\n";
    cout << "Digite o ID da carteira: ";
    int id; cin >> id; cin.ignore();
    if (carteiraDao->excluir(id)) { cout << "Carteira excluída com sucesso!\n"; }
    else { cout << "Carteira não encontrada.\n"; }
    Utils::pausar();
}

void Controller::_registrarCompra() {
    cout << "Registrar compra de moeda virtual\n";
    cout << "ID da carteira: ";
    int carteiraId; cin >> carteiraId; cin.ignore();
    if (carteiraDao->recuperar(carteiraId) == nullptr) { cout << "Carteira nao encontrada!\n"; }
    else {
        cout << "Quantidade: ";
        double qtd; cin >> qtd; cin.ignore();
        string data; cout << "Data (AAAA-MM-DD): "; getline(cin, data);
        movDao->incluir(Movimentacao(0, carteiraId, Movimentacao::COMPRA, qtd, data));
        cout << "Compra registrada com sucesso!\n";
    }
    Utils::pausar();
}

void Controller::_registrarVenda() {
    cout << "Registrar venda de moeda virtual\n";
    cout << "ID da carteira: ";
    int carteiraId; cin >> carteiraId; cin.ignore();
    if (carteiraDao->recuperar(carteiraId) == nullptr) { cout << "Carteira nao encontrada!\n"; }
    else {
        cout << "Quantidade: ";
        double qtd; cin >> qtd; cin.ignore();
        string data; cout << "Data (AAAA-MM-DD): "; getline(cin, data);
        movDao->incluir(Movimentacao(0, carteiraId, Movimentacao::VENDA, qtd, data));
        cout << "Venda registrada com sucesso!\n";
    }
    Utils::pausar();
}

void Controller::_mostrarTextoDeAjuda(){
    Utils::printMessage(SysInfo::getFullVersion() + " | Help");
    unique_ptr<TextFromFile> textFromFile(new TextFromFile(SysInfo::getHelpFile()));
    Utils::printFramedMessage(textFromFile->getFileContent(), "*", 120);
    Utils::pausar();
}

void Controller::_mostrarCreditos() {
    Utils::printMessage("Sobre o " + SysInfo::getSystemName());
    string text = "";
    text += SysInfo::getFullVersion() + "\n";
    text += "Autores: " + SysInfo::getAuthor() + "\n";
    text += "Instituicao: " + SysInfo::getInstitution() + "\n";
    text += "Copyright (C) " + SysInfo::getDate() + " " + SysInfo::getAuthor() + "\n";
    Utils::printFramedMessage(text, "*", 80);
    Utils::pausar();
}

// --- launchActions MODIFICADO ---
// A lógica de limpeza e exibição do menu foi centralizada aqui para o fluxo correto.
void Controller::launchActions(string title, vector<string> menuItens, vector<void (Controller::*)()> functions)
{
    try
    {
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
    catch (const exception &myException)
    {
        Utils::printMessage("Unexpected problem launching actions. " + string(myException.what()));
    }
}


void Controller::listarPorId() {
    cout << "=== Carteiras ordenadas por ID ===\n";
    vector<Carteira> lista = carteiraDao->listar();
    sort(lista.begin(), lista.end(), [](const Carteira& a, const Carteira& b) { return a.getId() < b.getId(); });
    for (const auto& c : lista) {
        cout << "ID: " << c.getId() << " | Titular: " << c.getNomeTitular() << '\n';
    }
    Utils::pausar();
}

void Controller::listarPorNome() {
    cout << "=== Carteiras ordenadas por nome ===\n";
    vector<Carteira> lista = carteiraDao->listar();
    sort(lista.begin(), lista.end(), [](const Carteira& a, const Carteira& b) { return a.getNomeTitular() < b.getNomeTitular(); });
    for (const auto& c : lista) {
        cout << "ID: " << c.getId() << " | Titular: " << c.getNomeTitular() << '\n';
    }
    Utils::pausar();
}

void Controller::exibirSaldo() {
    cout << "=== Exibir saldo da carteira ===\n";
    cout << "Digite o ID da carteira: ";
    int id; cin >> id; cin.ignore();
    Carteira* c = carteiraDao->recuperar(id);
    if (!c) { cout << "Carteira nao encontrada.\n"; }
    else {
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
    cout << "=== Histórico da carteira ===\n";
    cout << "Digite o ID da carteira: ";
    int id; cin >> id; cin.ignore();
    Carteira* c = carteiraDao->recuperar(id);
    if (!c) { cout << "Carteira nao encontrada.\n"; }
    else {
        cout << "Movimentações da carteira \"" << c->getNomeTitular() << "\":\n";
        vector<Movimentacao> movimentacoes = movDao->listarPorCarteira(id);
        if (movimentacoes.empty()) { cout << "Nenhuma movimentação encontrada para esta carteira.\n"; }
        else {
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
    cout << "=== Ganho ou perda total das carteiras (baseado no Oráculo) ===\n";
    vector<Carteira> carteiras = carteiraDao->listar();
    if (carteiras.empty()) { cout << "Nenhuma carteira cadastrada para gerar relatório.\n"; }
    else {
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
                 << " | Ganho/Perda Realizado: " << ganhoPerda << '\n';
        }
    }
    Utils::pausar();
}