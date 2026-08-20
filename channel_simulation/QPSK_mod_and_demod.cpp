#include "../include/head.hpp"
#include "../include/classes.hpp"

void QPSKModulator::modulate(){
    _qpsk_symbols.resize(_num_qpsk_sym);

    for (int i = 0; i < _m; i += 2){
        double Re = (_bits[i] == 1)? 1 : -1;
        double Im = (_bits[i+1] == 1)? 1 : -1;
        _qpsk_symbols[i/2] = {Re, Im};
    }
}

void QPSKDemodulator::demodulate(){
    _LLRs.resize(_m);

    for (int i = 0; i < _num_qpsk_symbols; i++){
        _LLRs[2*i] = _qpsk_symbols[i].real();
        _LLRs[2*i+1] = _qpsk_symbols[i].imag();
    }
}

QPSKModulator::QPSKModulator(std::vector<int> bits)
            : _bits(std::move(bits)) {
                modulate();
            }

QPSKDemodulator::QPSKDemodulator(std::vector<std::complex<double>> qpsk_symbols)
            : _qpsk_symbols(std::move(qpsk_symbols)) {
                demodulate();
            }

std::vector<std::complex<double>> QPSKModulator::getQpskSymbols(){
    return std::move(_qpsk_symbols);
}

std::vector<double> QPSKDemodulator::getLLRs(){
    return std::move(_LLRs);
}