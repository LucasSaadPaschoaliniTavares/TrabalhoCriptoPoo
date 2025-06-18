

#ifndef SERVERDBCONNECTION_H_
#define SERVERDBCONNECTION_H_

#include <mariadb/conncpp.hpp>
#include <string>

class ServerDBConnection final {
private:
    std::string serverIP       = "endereco_do_servidor";
    std::string serverDatabase = "seu_nome_de_banco";
    std::string serverUser     = "seu_usuario";
    std::string serverPassword = "sua_senha";
    std::string serverPort     = "3306";

    sql::Driver* driver;
    sql::Connection* connection;

public:
    ServerDBConnection();

    virtual ~ServerDBConnection();

    sql::Connection* getConnection() const;
};

#endif /* SERVERDBCONNECTION_H_ */