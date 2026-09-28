#include "../include/head.hpp"
#include "../include/classes.hpp"

double AWGN::N(double expectation, double variance){
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::normal_distribution<double> dist(expectation, std::sqrt(variance));
    return dist(gen);
}

void AWGN::gaussian_noise(double sigma){
    for (int i = 0; i < _num_qpsk_sym; i++){
        _qpsk_symbols[i] += std::complex<double>(N(0.0, sigma), N(0.0, sigma));
    }
}

AWGN::AWGN(std::vector<std::complex<double>> qpsk_symbols, double snr_db)
            : _qpsk_symbols(std::move(qpsk_symbols)){
                double snr_lin = std::pow(10, snr_db / 10.0);
                double sigma = std::sqrt(1.0 / snr_lin);
                gaussian_noise(sigma);
            }

std::vector<std::complex<double>> AWGN::getQpskSymbols(){
    return std::move(_qpsk_symbols);
}