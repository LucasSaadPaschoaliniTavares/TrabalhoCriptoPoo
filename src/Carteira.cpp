// src/Carteira.cpp

#include "Carteira.h"

// Construtor padrão
Carteira::Carteira() : id(0), nomeTitular(""), corretora("") {}

// Construtor com parâmetros
Carteira::Carteira(int id, const std::string& nomeTitular, const std::string& corretora)
    : id(id), nomeTitular(nomeTitular), corretora(corretora) {}

// Getters
int Carteira::getId() const {
    return id;
}

std::string Carteira::getNomeTitular() const {
    return nomeTitular;
}

std::string Carteira::getCorretora() const {
    return corretora;
}

// Setters
void Carteira::setId(int id) {
    this->id = id;
}

void Carteira::setNomeTitular(const std::string& nomeTitular) {
    this->nomeTitular = nomeTitular;
}

void Carteira::setCorretora(const std::string& corretora) {
    this->corretora = corretora;
}
