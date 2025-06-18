#include "MovimentacaoDAO_Memory.h"

void MovimentacaoDAO_Memory::incluir(const Movimentacao& mov) {
    Movimentacao copia = mov;
    copia.setId(nextId++);
    movimentacoes.push_back(copia);
}

std::vector<Movimentacao> MovimentacaoDAO_Memory::listarPorCarteira(int carteiraId) {
    std::vector<Movimentacao> resultado;
    for (const auto& m : movimentacoes) {
        if (m.getCarteiraId() == carteiraId) {
            resultado.push_back(m);
        }
    }
    return resultado;
}
