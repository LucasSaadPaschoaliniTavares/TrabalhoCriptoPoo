#ifndef ICARTEIRA_DAO_H
#define ICARTEIRA_DAO_H

#include <vector>
#include "Carteira.h"

class ICarteiraDAO {
public:
    virtual ~ICarteiraDAO() = default;

    virtual void incluir(const Carteira& carteira) = 0;
    virtual Carteira* recuperar(int id) = 0;
    virtual bool editar(const Carteira& carteira) = 0;
    virtual bool excluir(int id) = 0;
    virtual std::vector<Carteira> listar() const = 0;
};

#endif