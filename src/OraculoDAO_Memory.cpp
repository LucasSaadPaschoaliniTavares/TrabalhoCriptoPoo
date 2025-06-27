#include "OraculoDAO_Memory.h"

#include <string>
#include <sstream>
#include <algorithm>
#include <vector>

void OraculoDAO_Memory::incluir(const Oraculo& oraculo) {
   (void)oraculo;
}

Oraculo* OraculoDAO_Memory::recuperar(const std::string& data) {
    static const double PRECO_BASE = 1000.0;
    static const double FATOR_CRESCIMENTO_DIARIO = 400.0;
    static const long DATA_BASE = 20250101;

    std::string dataNumericaStr = data;
    dataNumericaStr.erase(std::remove(dataNumericaStr.begin(), dataNumericaStr.end(), '-'), dataNumericaStr.end());

    long dataNumerica = 0;
    std::stringstream ss(dataNumericaStr);
    ss >> dataNumerica;

    long diasPassados = dataNumerica - DATA_BASE;

    double precoCalculado = PRECO_BASE + (diasPassados * FATOR_CRESCIMENTO_DIARIO);
    if (precoCalculado < 0) {
        precoCalculado = 0;
    }

    return new Oraculo(data, precoCalculado);
}

std::vector<Oraculo> OraculoDAO_Memory::listar() const {
    return std::vector<Oraculo>();
}