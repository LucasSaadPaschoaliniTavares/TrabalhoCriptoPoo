// Arquivo: include/OraculoDAO_Memory.h

#ifndef ORACULO_DAO_MEMORY_H
#define ORACULO_DAO_MEMORY_H

#include <vector>
#include <string>
#include "Oraculo.h"
#include "IOraculoDAO.h"

class OraculoDAO_Memory : public IOraculoDAO {
private:
    std::vector<Oraculo> oraculos;

public:
    OraculoDAO_Memory() = default;

    void incluir(const Oraculo& oraculo) override;
    Oraculo* recuperar(const std::string& data) override;
    std::vector<Oraculo> listar() const override;
};

#endif