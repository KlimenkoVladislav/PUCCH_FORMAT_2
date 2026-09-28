#include "head.hpp"

class BlockEncoder{
private:
    int _n;
    const int _num_of_col = 13;
    const int _m = 20;
    std::vector<int> _bits;
    std::vector<int> _encod_data;

    void encoder();
public:
    BlockEncoder(int n, const std::vector<int> bits);
    std::vector<int> getEncodData();
};


class QPSKModulator{
private:
    const int _m = 20;
    const int _num_qpsk_sym = 10;
    const std::vector<int> _bits;
    std::vector<std::complex<double>> _qpsk_symbols;

    void modulate();
public:
    QPSKModulator(std::vector<int> bits);
    std::vector<std::complex<double>> getQpskSymbols();
};


class AWGN{
private:
    const int _num_qpsk_sym = 10;
    std::vector<std::complex<double>> _qpsk_symbols;

    double N(double expectation, double variance);
    void gaussian_noise(double snr_lin);
public:
    AWGN(std::vector<std::complex<double>> qpsk_symbols, double snr_db);
    std::vector<std::complex<double>> getQpskSymbols();
};


class QPSKDemodulator{
private:
    const int _m = 20;
    const int _num_qpsk_symbols = 10;
    std::vector<std::complex<double>> _qpsk_symbols;
    std::vector<double> _LLRs;

    void demodulate();
public:
    QPSKDemodulator(std::vector<std::complex<double>> qpsk_symbols);
    std::vector<double> getLLRs();
};


class BlockDecoder{
private:
    int _n;
    const int _num_of_col = 13;
    const int _m = 20;
    std::vector<double> _LLRs;
    std::vector<int> _bits;

    void decoder();
public:
    BlockDecoder(int n, std::vector<double> LLRs);
    std::vector<int> getBits();
};