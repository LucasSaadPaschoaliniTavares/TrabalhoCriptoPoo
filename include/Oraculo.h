
#ifndef ORACULO_H
#define ORACULO_H

#include <string>
#include <stdexcept>

class Oraculo {
private:
    std::string data;
    double cotacao;

public:
    Oraculo() = default;


    Oraculo(const std::string& data, double cotacao);

    std::string getData() const;
    double getCotacao() const;

    void setData(const std::string& data);
    void setCotacao(double novaCotacao);
};

#endif //ORACULO_H