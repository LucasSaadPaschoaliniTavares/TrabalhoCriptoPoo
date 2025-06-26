#ifndef ORACULODAO_DB_H
#define ORACULODAO_DB_H

#include "IOraculoDAO.h"
#include "ServerDBConnection.h"

class OraculoDAO_DB final : public IOraculoDAO {
private:
    ServerDBConnection* dbConnection;

public:
    OraculoDAO_DB(ServerDBConnection* conn);
    ~OraculoDAO_DB();

    void incluir(const Oraculo& oraculo) override;
    Oraculo* recuperar(const std::string& data) override;
    std::vector<Oraculo> listar() const override;
};

#endif