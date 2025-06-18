
#include "MovimentacaoDAO_DB.h"
#include <iostream>
#include <memory>

MovimentacaoDAO_DB::MovimentacaoDAO_DB(ServerDBConnection* conn) : dbConnection(conn) {}
MovimentacaoDAO_DB::~MovimentacaoDAO_DB() {}

void MovimentacaoDAO_DB::incluir(const Movimentacao& mov) {
    try {
        sql::Connection* conn = dbConnection->getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("INSERT INTO Movimentacoes(carteira_id, tipo, quantidade, data) VALUES (?, ?, ?, ?)"));

        pstmt->setInt(1, mov.getCarteiraId());

        std::string tipoStr = (mov.getTipo() == Movimentacao::COMPRA) ? "C" : "V";
        pstmt->setString(2, tipoStr);

        pstmt->setDouble(3, mov.getQuantidade());
        pstmt->setString(4, mov.getData());

        pstmt->executeUpdate();

    } catch (sql::SQLException &e) {
        std::cerr << "Erro ao inserir Movimentacao: " << e.what() << std::endl;
    }
}

std::vector<Movimentacao> MovimentacaoDAO_DB::listarPorCarteira(int carteiraId) {
    std::vector<Movimentacao> movimentacoes;
    try {
        sql::Connection* conn = dbConnection->getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT id, carteira_id, tipo, quantidade, data FROM Movimentacoes WHERE carteira_id = ? ORDER BY data"));

        pstmt->setInt(1, carteiraId);

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        while (res->next()) {
            std::string tipoStr = res->getString("tipo");
            Movimentacao::Tipo tipo = (tipoStr == "C") ? Movimentacao::COMPRA : Movimentacao::VENDA;

            movimentacoes.push_back(Movimentacao(
                res->getInt("id"),
                res->getInt("carteira_id"),
                tipo,
                res->getDouble("quantidade"),
                res->getString("data")
            ));
        }
    } catch (sql::SQLException &e) {
        std::cerr << "Erro ao listar Movimentacoes: " << e.what() << std::endl;
    }
    return movimentacoes;
}