#ifndef IORACULO_DAO_H
#define IORACULO_DAO_H

#include <string>
#include <vector>
#include "Oraculo.h"

class IOraculoDAO {
public:
    virtual ~IOraculoDAO() = default;

    virtual void incluir(const Oraculo& oraculo) = 0;
    virtual Oraculo* recuperar(const std::string& data) = 0;
    virtual std::vector<Oraculo> listar() const = 0;
};

#endif