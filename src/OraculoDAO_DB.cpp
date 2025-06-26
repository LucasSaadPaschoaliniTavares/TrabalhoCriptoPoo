#include "OraculoDAO_DB.h"
#include <iostream>
#include <memory>

OraculoDAO_DB::OraculoDAO_DB(ServerDBConnection* conn) : dbConnection(conn) {}
OraculoDAO_DB::~OraculoDAO_DB() {}

void OraculoDAO_DB::incluir(const Oraculo& oraculo) {
    try {
        sql::Connection* conn = dbConnection->getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("INSERT INTO ORACULO(Data, Cotacao) VALUES (?, ?) ON DUPLICATE KEY UPDATE Cotacao = VALUES(Cotacao)"));

        pstmt->setString(1, oraculo.getData());
        pstmt->setDouble(2, oraculo.getCotacao());

        pstmt->executeUpdate();

    } catch (sql::SQLException &e) {
        std::cerr << "Erro ao inserir no Oraculo: " << e.what() << std::endl;
    }
}

Oraculo* OraculoDAO_DB::recuperar(const std::string& data) {
    try {
        sql::Connection* conn = dbConnection->getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT Data, Cotacao FROM ORACULO WHERE Data = ?"));

        pstmt->setString(1, data);

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next()) {
            return new Oraculo(
                (std::string)res->getString("Data"),
                res->getDouble("Cotacao")
            );
        }
    } catch (sql::SQLException &e) {
        std::cerr << "Erro ao recuperar do Oraculo: " << e.what() << std::endl;
    }
    return nullptr;
}

std::vector<Oraculo> OraculoDAO_DB::listar() const {
    std::vector<Oraculo> oraculos;
    try {
        sql::Connection* conn = dbConnection->getConnection();
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT Data, Cotacao FROM ORACULO ORDER BY Data"));

        while (res->next()) {
            oraculos.push_back(Oraculo(
                (std::string)res->getString("Data"),
                res->getDouble("Cotacao")
            ));
        }
    } catch (sql::SQLException &e) {
        std::cerr << "Erro ao listar Oraculo: " << e.what() << std::endl;
    }
    return oraculos;
}