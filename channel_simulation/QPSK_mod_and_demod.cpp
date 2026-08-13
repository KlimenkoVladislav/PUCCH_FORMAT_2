#include "../head.hpp"

class QPSKModulator{
private:
    const int _m = 20;
    const int _num_qpsk_sym = 10;
    std::vector<int> _bits;
    std::vector<std::complex<double>> _qpsk_symbols;

    void modulate(){
        _qpsk_symbols.resize(_num_qpsk_sym);

        for (int i = 0; i < _m; i += 2){
            double Re = (_bits[i] == 1)? 1 : -1;
            double Im = (_bits[i+1] == 1)? 1 : -1;
            _qpsk_symbols[i/2] = {Re, Im};
        }
    }

public:
    QPSKModulator(std::vector<int> bits)
                : _bits(std::move(bits)) {
                    modulate();
                }

    std::vector<std::complex<double>> getQpskSymbols(){
        return std::move(_qpsk_symbols);
    }
};

class QPSKDemodulator{
private:
    const int _m = 20;
    const int _num_qpsk_symbols = 10;
    std::vector<std::complex<double>> _qpsk_symbols;
    std::vector<double> _LLRs;

    void demodulate(){
        _LLRs.resize(_m);

        for (int i = 0; i < _num_qpsk_symbols; i++){
            _LLRs[i] = _qpsk_symbols[i].real();
            _LLRs[i+1] = _qpsk_symbols[i].imag();
        }
    }

public:
    QPSKDemodulator(std::vector<std::complex<double>> qpsk_symbols)
                : _qpsk_symbols(std::move(qpsk_symbols)) {
                    demodulate();
                }

    std::vector<double> getLLRs(){
        return std::move(_LLRs);
    }
};