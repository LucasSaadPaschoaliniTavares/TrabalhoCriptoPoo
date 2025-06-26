#include "MovimentacaoDAO_DB.h"
#include <iostream>
#include <memory>

MovimentacaoDAO_DB::MovimentacaoDAO_DB(ServerDBConnection* conn) : dbConnection(conn) {}
MovimentacaoDAO_DB::~MovimentacaoDAO_DB() {}

void MovimentacaoDAO_DB::incluir(const Movimentacao& mov) {
    try {
        sql::Connection* conn = dbConnection->getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("INSERT INTO MOVIMENTACAO(IdCarteira, TipoOperacao, Quantidade, Data) VALUES (?, ?, ?, ?)"));

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
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT IdMovimento, IdCarteira, TipoOperacao, Quantidade, Data FROM MOVIMENTACAO WHERE IdCarteira = ? ORDER BY Data"));

        pstmt->setInt(1, carteiraId);

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        while (res->next()) {
            std::string tipoStr = (std::string)res->getString("tipo");
            Movimentacao::Tipo tipo = (tipoStr == "C") ? Movimentacao::COMPRA : Movimentacao::VENDA;

            movimentacoes.push_back(Movimentacao(
                res->getInt("IdMovimento"),
                res->getInt("IdCarteira"),
                tipo,
                res->getDouble("Quantidade"),
                (std::string)res->getString("Data")
            ));
        }
    } catch (sql::SQLException &e) {
        std::cerr << "Erro ao listar Movimentacoes: " << e.what() << std::endl;
    }
    return movimentacoes;
}