#include "head.hpp"

#include <random>

std::vector<int> generate_random_bits(int n){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 1);

    std::vector<int> bits(n);
    for (int i = 0; i < n; i++){
        bits[i] = dist(gen);
    }

    return bits;
}

std::complex<double> string_to_complex(const std::string &str){
    double real, imag;
    char sign, j;

    std::stringstream ss(str);
    ss >> real >> sign >> imag >> j;    
    if (sign == '-'){ imag = -imag; }
    
    return std::complex<double>(real, imag);
}

void coding_mode(int n, const std::vector<int> bits){

}

void decoding_mode(int n, const std::vector<std::string> input_arr){
    std::vector<std::complex<double>> qpsk_symbols(10);
    for (int i = 0; i < 10; i++){
        qpsk_symbols[i] = string_to_complex(input_arr[i]);
    }

}

void channel_simulation_mode(int n, int iterations){
    for (int i = 0; i < iterations; i++){
        std::vector<int> bits = generate_random_bits(n);
        BlockEncoder encoder(n, std::move(bits));
        const std::vector<int> &encod_data = encoder.getEncodData();
    }
}