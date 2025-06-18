
#include "Relatorio.h"
#include "Oraculo.h"   // Para usar getCotacao()
#include "Carteira.h"  // Para usar getNomeTitular() etc.
#include "Movimentacao.h" // Para usar getTipo(), getQuantidade() etc.

#include <iostream>
#include <vector>
#include <algorithm> // Para std::sort
#include <string>

using namespace std;


void listarPorId(ICarteiraDAO& dao) {
    cout << "=== Carteiras ordenadas por ID ===\n";
    vector<Carteira> lista = dao.listar();

    sort(lista.begin(), lista.end(),
        [](const Carteira& a, const Carteira& b) {
            return a.getId() < b.getId();
        });

    for (const auto& c : lista) {
        cout << "ID: " << c.getId()
             << " | Titular: " << c.getNomeTitular() << '\n';
    }
    cout << endl;
}

void listarPorNome(ICarteiraDAO& dao) {
    cout << "=== Carteiras ordenadas por nome ===\n";
    vector<Carteira> lista = dao.listar();

    sort(lista.begin(), lista.end(),
        [](const Carteira& a, const Carteira& b) {
            return a.getNomeTitular() < b.getNomeTitular();
        });

    for (const auto& c : lista) {
        cout << "ID: " << c.getId()
             << " | Titular: " << c.getNomeTitular() << '\n';
    }
    cout << endl;
}

// Implementação usando as interfaces
void exibirSaldo(ICarteiraDAO& carteiraDao, IMovimentacaoDAO& movimentacaoDao) {
    cout << "=== Exibir saldo da carteira ===\n";
    cout << "Digite o ID da carteira: ";
    int id;
    cin >> id;
    cin.ignore();

    Carteira* c = carteiraDao.recuperar(id);
    if (!c) {
        cout << "Carteira nao encontrada.\n\n";
        return;
    }

    vector<Movimentacao> movimentacoes = movimentacaoDao.listarPorCarteira(id);
    double saldo = 0.0;

    for (const auto& m : movimentacoes) {
        saldo += (m.getTipo() == Movimentacao::COMPRA ? m.getQuantidade() : -m.getQuantidade());
    }

    cout << "Saldo atual (em quantidade de moeda) da carteira \"" << c->getNomeTitular() << "\": " << saldo << "\n\n";
}

void exibirHistorico(IMovimentacaoDAO& movimentacaoDao, ICarteiraDAO& carteiraDao) {
    cout << "=== Histórico da carteira ===\n";
    cout << "Digite o ID da carteira: ";
    int id;
    cin >> id;
    cin.ignore();

    Carteira* c = carteiraDao.recuperar(id);
    if (!c) {
        cout << "Carteira nao encontrada.\n\n";
        return;
    }

    cout << "Movimentações da carteira \"" << c->getNomeTitular() << "\":\n";
    vector<Movimentacao> movimentacoes = movimentacaoDao.listarPorCarteira(id);

    if (movimentacoes.empty()) {
        cout << "Nenhuma movimentação encontrada para esta carteira.\n";
    } else {
        for (const auto& m : movimentacoes) {
            cout << (m.getTipo() == Movimentacao::COMPRA ? "Compra" : "Venda")
                 << " - Qtde: " << m.getQuantidade()
                 << ", Data: " << m.getData() << '\n';
        }
    }
    cout << endl;
}

void exibirGanhoPerdaTotal(ICarteiraDAO& carteiraDao, IMovimentacaoDAO& movDao, IOraculoDAO& oraculoDao) {
    cout << "=== Ganho ou perda total das carteiras (baseado no Oráculo) ===\n";
    vector<Carteira> carteiras = carteiraDao.listar();

    if (carteiras.empty()) {
        cout << "Nenhuma carteira cadastrada para gerar relatório.\n\n";
        return;
    }

    for (const auto& c : carteiras) {
        vector<Movimentacao> movimentacoes = movDao.listarPorCarteira(c.getId());

        double totalGastoEmCompras = 0.0;
        double totalRecebidoDeVendas = 0.0;

        for (const auto& m : movimentacoes) {
            Oraculo* oraculoDoDia = oraculoDao.recuperar(m.getData());

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
    cout << endl;
}