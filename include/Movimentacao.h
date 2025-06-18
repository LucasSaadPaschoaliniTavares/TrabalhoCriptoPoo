
#ifndef MOVIMENTACAO_H
#define MOVIMENTACAO_H

#include <string>
#include <ctime>

class Movimentacao {
public:
    enum Tipo { COMPRA, VENDA };

private:
    int id;
    int carteiraId;
    Tipo tipo;
    double quantidade;
    std::string data;

public:
    Movimentacao() = default;
    Movimentacao(int id, int carteiraId, Tipo tipo, double quantidade, const std::string& data);

    int getId() const;
    int getCarteiraId() const;
    Tipo getTipo() const;
    double getQuantidade() const;
    std::string getData() const;

    void setId(int id);
    void setCarteiraId(int carteiraId);
    void setTipo(Tipo tipo);
    void setQuantidade(double quantidade);
    void setData(const std::string& data);
};

#endif