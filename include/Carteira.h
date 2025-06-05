// include/Carteira.h

#ifndef CARTEIRA_H
#define CARTEIRA_H

#include <string>

class Carteira {
private:
    int id;
    std::string nomeTitular;
    std::string corretora;

public:
    Carteira();
    Carteira(int id, const std::string& nomeTitular, const std::string& corretora);

    // Getters
    int getId() const;
    std::string getNomeTitular() const;
    std::string getCorretora() const;

    // Setters
    void setId(int id);
    void setNomeTitular(const std::string& nomeTitular);
    void setCorretora(const std::string& corretora);
};

#endif
