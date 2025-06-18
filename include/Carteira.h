#ifndef CARTEIRA_H
#define CARTEIRA_H

#include <string>

class Carteira {
private:
    int id;
    std::string nomeTitular;
    std::string corretora;

public:

    Carteira(int id = 0, const std::string& nomeTitular = "", const std::string& corretora = "");

    int getId() const;
    void setId(int id);

    std::string getNomeTitular() const;
    void setNomeTitular(const std::string& nome);


    std::string getCorretora() const;
    void setCorretora(const std::string& corretora);
};

#endif