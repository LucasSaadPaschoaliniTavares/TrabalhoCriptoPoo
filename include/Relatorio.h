
#ifndef RELATORIO_H
#define RELATORIO_H

#include "ICarteiraDAO.h"
#include "IMovimentacaoDAO.h"
#include "IOraculoDAO.h"


void listarPorId(ICarteiraDAO& dao);

void listarPorNome(ICarteiraDAO& dao);

void exibirSaldo(ICarteiraDAO& carteiraDao, IMovimentacaoDAO& movimentacaoDao);

void exibirHistorico(IMovimentacaoDAO& movimentacaoDao, ICarteiraDAO& carteiraDao);

void exibirGanhoPerdaTotal(ICarteiraDAO& carteiraDao, IMovimentacaoDAO& movDao, IOraculoDAO& oraculoDao);

#endif //RELATORIO_H