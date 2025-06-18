
#ifndef MOVIMENTACAODAO_DB_H
#define MOVIMENTACAODAO_DB_H

#include "IMovimentacaoDAO.h"
#include "ServerDBConnection.h"

class MovimentacaoDAO_DB final : public IMovimentacaoDAO {
private:
    ServerDBConnection* dbConnection;

public:
    MovimentacaoDAO_DB(ServerDBConnection* conn);
    ~MovimentacaoDAO_DB();

    void incluir(const Movimentacao& movimentacao) override;
    std::vector<Movimentacao> listarPorCarteira(int carteiraId) override;
};

#endif