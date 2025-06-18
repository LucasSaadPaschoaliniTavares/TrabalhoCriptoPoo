
#ifndef MOVIMENTACAODAO_MEMORY_H
#define MOVIMENTACAODAO_MEMORY_H

#include <vector>
#include "Movimentacao.h"
#include "IMovimentacaoDAO.h"

class MovimentacaoDAO_Memory : public IMovimentacaoDAO {
private:
    std::vector<Movimentacao> movimentacoes;
    int nextId = 1;

public:
    void incluir(const Movimentacao& mov) override;
    std::vector<Movimentacao> listarPorCarteira(int carteiraId) override;
};

#endif