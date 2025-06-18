#include "Movimentacao.h"

Movimentacao::Movimentacao(int id, int carteiraId, Tipo tipo, double quantidade, const std::string& data)
    : id(id), carteiraId(carteiraId), tipo(tipo), quantidade(quantidade), data(data) {}

int Movimentacao::getId() const { return id; }
int Movimentacao::getCarteiraId() const { return carteiraId; }
Movimentacao::Tipo Movimentacao::getTipo() const { return tipo; }
double Movimentacao::getQuantidade() const { return quantidade; }
std::string Movimentacao::getData() const { return data; }

void Movimentacao::setId(int id) { this->id = id; }
void Movimentacao::setCarteiraId(int carteiraId) { this->carteiraId = carteiraId; }
void Movimentacao::setTipo(Tipo tipo) { this->tipo = tipo; }
void Movimentacao::setQuantidade(double quantidade) { this->quantidade = quantidade; }
void Movimentacao::setData(const std::string& data) { this->data = data; }
