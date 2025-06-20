#include "Oraculo.h"


Oraculo::Oraculo(const std::string& data, double cotacao)
    : data(data) {
    setCotacao(cotacao);
}

std::string Oraculo::getData() const {
    return data;
}

double Oraculo::getCotacao() const {
    return cotacao;
}

void Oraculo::setData(const std::string& data) {
    this->data = data;
}

void Oraculo::setCotacao(double novaCotacao) {
    if (novaCotacao < 0) {
        throw std::invalid_argument("A cotação não pode ser um valor negativo.");
    }
    this->cotacao = novaCotacao;
}