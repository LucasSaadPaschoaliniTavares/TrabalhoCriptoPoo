//
// Created by lucas on 18-04-2025.
//

#ifndef CARTEIRA_H
#define CARTEIRA_H

#include "Carteira.h"
#include <string>

class Carteira {
private:
    int id;
    std::string nomeTitular;
    std::string corretora;

public:
    Carteira(); 
    Carteira(int id, const std::string& nomeTitular, const std::string& corretora);

    int getId() const;
    void setId(int id);

    std::string getNomeTitular() const;
    void setNomeTitular(const std::string& nomeTitular);

    std::string getCorretora() const;
    void setCorretora(const std::string& corretora);

    std::string toString() const;

};

#endif