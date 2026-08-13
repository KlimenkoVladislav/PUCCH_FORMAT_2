#include "json.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <random>
#include <optional>
#include <complex>

using json = nlohmann::json;

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

class AWGH{
private:
    const int _num_qpsk_sym = 10;
    std::vector<std::complex<double>> _qpsk_symbols;

    double N(double expectation, double variance);
    void gaussian_noise();
public:
    AWGH(std::vector<std::complex<double>> qpsk_symbols);
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


// work_with_json.cpp
std::optional<std::complex<double>> validate_string_to_complex(const std::string& str);
bool validate_extra_fields(const json &data, const std::vector<std::string> &allowed_fields);
bool validate_value(const json &data);
int distribution(std::string filename);

// mods.cpp
std::vector<int> generate_random_bits(int n);
std::complex<double> string_to_complex(const std::string &str);
void coding_mode(int n, const std::vector<int> bits);
void decoding_mode(int n, const std::vector<std::string> str);
void channel_simulation_mode(int n, int iterations);