
#ifndef CARTEIRADAO_DB_H
#define CARTEIRADAO_DB_H

#include "ICarteiraDAO.h"
#include "ServerDBConnection.h"

class CarteiraDAO_DB final : public ICarteiraDAO {
private:
    ServerDBConnection* dbConnection;

public:
    CarteiraDAO_DB(ServerDBConnection* conn);
    ~CarteiraDAO_DB();


    void incluir(const Carteira& carteira) override;
    Carteira* recuperar(int id) override;
    std::vector<Carteira> listar() override;
    bool editar(const Carteira& carteira) override;
    bool excluir(int id) override;
};

#endif