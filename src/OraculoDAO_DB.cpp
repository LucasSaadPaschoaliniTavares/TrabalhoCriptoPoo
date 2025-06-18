
#include "OraculoDAO_DB.h"
#include <iostream>
#include <memory>

OraculoDAO_DB::OraculoDAO_DB(ServerDBConnection* conn) : dbConnection(conn) {}
OraculoDAO_DB::~OraculoDAO_DB() {}

void OraculoDAO_DB::incluir(const Oraculo& oraculo) {
    try {
        sql::Connection* conn = dbConnection->getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("INSERT INTO Oraculo(data, cotacao) VALUES (?, ?) ON DUPLICATE KEY UPDATE cotacao = VALUES(cotacao)"));

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
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT data, cotacao FROM Oraculo WHERE data = ?"));

        pstmt->setString(1, data);

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next()) {
            return new Oraculo(
                res->getString("data"),
                res->getDouble("cotacao")
            );
        }
    } catch (sql::SQLException &e) {
        std::cerr << "Erro ao recuperar do Oraculo: " << e.what() << std::endl;
    }
    return nullptr;
}

std::vector<Oraculo> OraculoDAO_DB::listar() {
    std::vector<Oraculo> oraculos;
    try {
        sql::Connection* conn = dbConnection->getConnection();
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT data, cotacao FROM Oraculo ORDER BY data"));

        while (res->next()) {
            oraculos.push_back(Oraculo(
                res->getString("data"),
                res->getDouble("cotacao")
            ));
        }
    } catch (sql::SQLException &e) {
        std::cerr << "Erro ao listar Oraculo: " << e.what() << std::endl;
    }
    return oraculos;
}