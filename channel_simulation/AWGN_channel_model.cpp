#include "../head.hpp"

class AWGN{
private:
    const int _num_qpsk_sym = 10;
    std::vector<std::complex<double>> _qpsk_symbols;

    double N(double expectation, double variance){
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::normal_distribution<double> dist(expectation, std::sqrt(variance));
        return dist(gen);
    }

    void gaussian_noise(){
        for (int i = 0; i < _num_qpsk_sym; i++){
            _qpsk_symbols[i] += std::complex<double>(N(0.0, 1.0), N(0.0, 1.0));
        }
    }

public:
    AWGN(std::vector<std::complex<double>> qpsk_symbols)
                : _qpsk_symbols(std::move(qpsk_symbols)) {}

    std::vector<std::complex<double>> getQpskSymbols(){
        return std::move(_qpsk_symbols);
    }
};