
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "DataBaseSelector.h"
#include "ICarteiraDAO.h"
#include "IMovimentacaoDAO.h"
#include "IOraculoDAO.h"


class Controller {
private:
    ICarteiraDAO* carteiraDao;
    IMovimentacaoDAO* movDao;
    IOraculoDAO* oraculoDao;

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

public:
    Controller(DataBaseSelector selector);
    ~Controller();
    void executar();
};

#endif