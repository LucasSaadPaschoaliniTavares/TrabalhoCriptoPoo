#ifndef CARTEIRA_DAO_MEMORY_H
#define CARTEIRA_DAO_MEMORY_H

#include <vector>
#include <memory>
#include "Carteira.h"
#include "ICarteiraDAO.h"


class CarteiraDAO_Memory : public ICarteiraDAO {
private:
    std::vector<Carteira> carteiras;
    int proximoId;

public:
    CarteiraDAO_Memory();
    void incluir(const Carteira& carteira) override;
    Carteira* recuperar(int id) override;
    bool editar(const Carteira& carteira) override;
    bool excluir(int id) override;
    std::vector<Carteira> listar() const override;
};

#endif