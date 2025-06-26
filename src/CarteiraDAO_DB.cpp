#include "CarteiraDAO_DB.h"
#include <iostream>
#include <memory>


CarteiraDAO_DB::CarteiraDAO_DB(ServerDBConnection* conn) : dbConnection(conn) {}

CarteiraDAO_DB::~CarteiraDAO_DB() {}

void CarteiraDAO_DB::incluir(const Carteira& carteira) {
    try {
        sql::Connection* conn = dbConnection->getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("INSERT INTO CARTEIRA(Titular, Corretora) VALUES (?, ?)"));

        pstmt->setString(1, carteira.getNomeTitular());
        pstmt->setString(2, carteira.getCorretora());

        pstmt->executeUpdate();

    } catch (sql::SQLException &e) {
        std::cerr << "Erro ao inserir Carteira: " << e.what() << std::endl;
    }
}

Carteira* CarteiraDAO_DB::recuperar(int id) {
    try {
        sql::Connection* conn = dbConnection->getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT IdCarteira, Titular, Corretora FROM CARTEIRA WHERE IdCarteira = ?"));

        pstmt->setInt(1, id);

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        if (res->next()) {
            return new Carteira(
                res->getInt("IdCarteira"),
                (std::string)res->getString("Titular"),
                (std::string)res->getString("Corretora")
            );
        }
    } catch (sql::SQLException &e) {
        std::cerr << "Erro ao recuperar Carteira: " << e.what() << std::endl;
    }
    return nullptr;
}

std::vector<Carteira> CarteiraDAO_DB::listar() const {
    std::vector<Carteira> carteiras;
    try {
        sql::Connection* conn = dbConnection->getConnection();
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT IdCarteira, Titular, Corretora FROM CARTEIRA ORDER BY Titular"));

        while (res->next()) {
            carteiras.push_back(Carteira(
                res->getInt("IdCarteira"),
                (std::string)res->getString("Titular"),
                (std::string)res->getString("Corretora")
            ));
        }
    } catch (sql::SQLException &e) {
        std::cerr << "Erro ao listar Carteiras: " << e.what() << std::endl;
    }
    return carteiras;
}

bool CarteiraDAO_DB::editar(const Carteira& carteira) {
    try {
        sql::Connection* conn = dbConnection->getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("UPDATE CARTEIRA SET Titular = ?, Corretora = ? WHERE IdCarteira = ?"));

        pstmt->setString(1, carteira.getNomeTitular());
        pstmt->setString(2, carteira.getCorretora());
        pstmt->setInt(3, carteira.getId());

        // executeUpdate retorna o número de linhas afetadas
        return pstmt->executeUpdate() > 0;
    } catch (sql::SQLException &e) {
        std::cerr << "Erro ao editar Carteira: " << e.what() << std::endl;
    }
    return false;
}

bool CarteiraDAO_DB::excluir(int id) {
    try {
        sql::Connection* conn = dbConnection->getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("DELETE FROM CARTEIRA WHERE IdCarteira = ?"));

        pstmt->setInt(1, id);

        return pstmt->executeUpdate() > 0;
    } catch (sql::SQLException &e) {
        std::cerr << "Erro ao excluir Carteira: " << e.what() << std::endl;
    }
    return false;
}