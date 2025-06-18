#include "CarteiraDAO_Memory.h"

CarteiraDAO_Memory::CarteiraDAO_Memory() : proximoId(1) {}

void CarteiraDAO_Memory::incluir(const Carteira& carteira) {
    Carteira c = carteira;
    c.setId(proximoId++);
    carteiras.push_back(c);
}

Carteira* CarteiraDAO_Memory::recuperar(int id) {
    for (auto &c : carteiras) {
        if (c.getId() == id) {
            return &c;
        }
    }
    return nullptr;
}

bool CarteiraDAO_Memory::editar(const Carteira& carteira) {
    for (auto &c : carteiras) {
        if (c.getId() == carteira.getId()) {
            c.setNomeTitular(carteira.getNomeTitular());
            return true;
        }
    }
    return false;
}

bool CarteiraDAO_Memory::excluir(int id) {
    for (auto it = carteiras.begin(); it != carteiras.end(); ++it) {
        if (it->getId() == id) {
            carteiras.erase(it);
            return true;
        }
    }
    return false;
}

std::vector<Carteira> CarteiraDAO_Memory::listar() const {
    return carteiras;
}
