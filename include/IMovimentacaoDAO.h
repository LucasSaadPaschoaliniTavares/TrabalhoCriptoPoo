#ifndef IMOVIMENTACAO_DAO_H
#define IMOVIMENTACAO_DAO_H

#include <vector>
#include "Movimentacao.h"

class IMovimentacaoDAO {
public:
    virtual ~IMovimentacaoDAO() = default;

    virtual void incluir(const Movimentacao& movimentacao) = 0;
    virtual std::vector<Movimentacao> listarPorCarteira(int carteiraId) = 0;
};

#endif