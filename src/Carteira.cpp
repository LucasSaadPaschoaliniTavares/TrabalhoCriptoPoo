#include "Carteira.h"

Carteira::Carteira(int id, const std::string& nomeTitular, const std::string& corretora)
    : id(id), nomeTitular(nomeTitular), corretora(corretora) {
}

int Carteira::getId() const {
    return id;
}

void Carteira::setId(int id) {
    this->id = id;
}

std::string Carteira::getNomeTitular() const {
    return nomeTitular;
}

void Carteira::setNomeTitular(const std::string& nome) {
    nomeTitular = nome;
}

std::string Carteira::getCorretora() const {
    return corretora;
}

void Carteira::setCorretora(const std::string& corretora) {
    this->corretora = corretora;
}