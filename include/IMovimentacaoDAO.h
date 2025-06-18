
#ifndef IMOVIMENTACAO_DAO_H
#define IMOVIMENTACAO_DAO_H

#include <vector>
#include "Movimentacao.h"

class IMovimentacaoDAO {
public:
    virtual ~IMovimentacaoDAO() = default;

    virtual void incluir(const Movimentacao& movimentacao) = 0;
    // Este é o método especial que os relatórios precisam
    virtual std::vector<Movimentacao> listarPorCarteira(int carteiraId) = 0;
};

#endif