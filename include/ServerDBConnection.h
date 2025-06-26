#ifndef SERVERDBCONNECTION_H_
#define SERVERDBCONNECTION_H_

#include <mariadb/conncpp.hpp>
#include <string>

class ServerDBConnection final {
private:
    std::string serverIP       = "143.106.243.64";
    std::string serverDatabase = "PooI_25_B12";
    std::string serverUser     = "PooI_25_B12";
    std::string serverPassword = "kDtAZStbJh";
    std::string serverPort     = "3306";

    sql::Driver* driver;
    sql::Connection* connection;

public:
    ServerDBConnection();

    virtual ~ServerDBConnection();

    sql::Connection* getConnection() const;
};

#endif /* SERVERDBCONNECTION_H_ */