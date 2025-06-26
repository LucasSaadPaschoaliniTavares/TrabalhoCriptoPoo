#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <vector>
#include <string>

#include "DataBaseSelector.h"
#include "ICarteiraDAO.h"
#include "IMovimentacaoDAO.h"
#include "IOraculoDAO.h"
#include "ServerDBConnection.h"


class Controller {
private:
    ICarteiraDAO* carteiraDao;
    IMovimentacaoDAO* movDao;
    IOraculoDAO* oraculoDao;
    ServerDBConnection* dbConnection;


    void _popularOraculo();
    void _incluirCarteira();
    void _listarCarteiras();
    void _editarCarteira();
    void _excluirCarteira();
    void _registrarCompra();
    void _registrarVenda();
    void _processarMenuCarteira();
    void _processarMenuMovimentacao();
    void _processarMenuRelatorio();
    void _processarMenuAjuda();
    void _mostrarTextoDeAjuda();
    void _mostrarCreditos();
    void listarPorId();
    void listarPorNome();
    void exibirSaldo();
    void exibirHistorico();
    void exibirGanhoPerdaTotal();


    void launchActions(std::string title, std::vector<std::string> menuItens, std::vector<void (Controller::*)()> functions);


public:
    Controller(DataBaseSelector selector);
    ~Controller();
    void executar();
};

#endif // CONTROLLER_H