
#include "CarteiraDAO_DB.h"
#include <iostream>
#include <memory>


CarteiraDAO_DB::CarteiraDAO_DB(ServerDBConnection* conn) : dbConnection(conn) {}

CarteiraDAO_DB::~CarteiraDAO_DB() {}

void CarteiraDAO_DB::incluir(const Carteira& carteira) {
    try {
        sql::Connection* conn = dbConnection->getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("INSERT INTO Carteiras(nome_titular, corretora) VALUES (?, ?)"));

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
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT id, nome_titular, corretora FROM Carteiras WHERE id = ?"));

        pstmt->setInt(1, id);

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        if (res->next()) {
            return new Carteira(
                res->getInt("id"),
                res->getString("nome_titular"),
                res->getString("corretora")
            );
        }
    } catch (sql::SQLException &e) {
        std::cerr << "Erro ao recuperar Carteira: " << e.what() << std::endl;
    }
    return nullptr;
}

std::vector<Carteira> CarteiraDAO_DB::listar() {
    std::vector<Carteira> carteiras;
    try {
        sql::Connection* conn = dbConnection->getConnection();
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT id, nome_titular, corretora FROM Carteiras ORDER BY nome_titular"));

        while (res->next()) {
            carteiras.push_back(Carteira(
                res->getInt("id"),
                res->getString("nome_titular"),
                res->getString("corretora")
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
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("UPDATE Carteiras SET nome_titular = ?, corretora = ? WHERE id = ?"));

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
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("DELETE FROM Carteiras WHERE id = ?"));

        pstmt->setInt(1, id);

        return pstmt->executeUpdate() > 0;
    } catch (sql::SQLException &e) {
        std::cerr << "Erro ao excluir Carteira: " << e.what() << std::endl;
    }
    return false;
}