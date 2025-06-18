

#include "OraculoDAO_Memory.h"

void OraculoDAO_Memory::incluir(const Oraculo& oraculo) {
    oraculos.push_back(oraculo);
}

Oraculo* OraculoDAO_Memory::recuperar(const std::string& data) {
    for (auto& o : oraculos) {
        if (o.getData() == data) {
            return &o;
        }
    }
    return nullptr;
}

std::vector<Oraculo> OraculoDAO_Memory::listar() const {
    return oraculos;
}