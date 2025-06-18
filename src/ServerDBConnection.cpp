#include "ServerDBConnection.h"
#include <iostream>
#include <stdexcept>

ServerDBConnection::ServerDBConnection() : driver(nullptr), connection(nullptr) {
    try {
        this->driver = sql::mariadb::get_driver_instance();

        std::string url = "jdbc:mariadb://" + serverIP + ":" + serverPort + "/" + serverDatabase;

        this->connection = driver->connect(url, serverUser, serverPassword);

        std::cout << "[INFO] Conectado com sucesso ao banco de dados '" << serverDatabase << "'." << std::endl;

    } catch (sql::SQLException &e) {
        std::cerr << "ERRO CRÍTICO: Falha ao conectar ao MariaDB." << std::endl;
        std::cerr << "Motivo: " << e.what() << std::endl;
        exit(1);
    }
}

ServerDBConnection::~ServerDBConnection() {
    if (this->connection) {
        this->connection->close();
    }
}

sql::Connection* ServerDBConnection::getConnection() const {
    return this->connection;
}