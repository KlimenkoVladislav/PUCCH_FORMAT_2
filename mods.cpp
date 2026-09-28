#include "include/head.hpp"
#include "include/classes.hpp"

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
    double Re, Im;
    char sign, j;

    std::stringstream ss(str);
    ss >> Re >> sign >> Im >> j;    
    if (sign == '-'){ Im = -Im; }
    
    return std::complex<double>(Re, Im);
}

int coding_mode(int n, const std::vector<int> bits){
    BlockEncoder encoder(n, std::move(bits));
    std::vector<int> encod_data = encoder.getEncodData();

    QPSKModulator modulator(std::move(encod_data));
    std::vector<std::complex<double>> qpsk_symbols = modulator.getQpskSymbols();

    return coding_mode_output(qpsk_symbols);
}

int decoding_mode(int n, const std::vector<std::string> input_arr){
    std::vector<std::complex<double>> qpsk_w_noise(10);
    for (int i = 0; i < 10; i++){
        qpsk_w_noise[i] = string_to_complex(input_arr[i]);
    }

    QPSKDemodulator demodulator(std::move(qpsk_w_noise));
    std::vector<double> LLRs = demodulator.getLLRs();

    BlockDecoder decoder(n, std::move(LLRs));
    std::vector<int> pucch_f2_bits = decoder.getBits();

    return decoding_mode_output(n, pucch_f2_bits);
}

int channel_simulation_mode(int n, int iterations, double snr_db){
    std::pair<int, double> res;

    for (int i = 0; i < iterations; i++){
        std::vector<int> bits = generate_random_bits(n);
        BlockEncoder encoder(n, bits);  // тут вот не move т.к. надо для BLER
        std::vector<int> encod_data = encoder.getEncodData();

        QPSKModulator modulator(std::move(encod_data));
        std::vector<std::complex<double>> qpsk_symbols = modulator.getQpskSymbols();

        AWGN noise(std::move(qpsk_symbols), snr_db);
        std::vector<std::complex<double>> qpsk_w_noise = noise.getQpskSymbols();

        QPSKDemodulator demodulator(std::move(qpsk_w_noise));
        std::vector<double> LLRs = demodulator.getLLRs();

        BlockDecoder decoder(n, std::move(LLRs));
        std::vector<int> pucch_f2_bits = decoder.getBits();

        res = BLER(std::move(bits), pucch_f2_bits, iterations);
    }

    std::ofstream file("../output.txt", std::ios::app);
    if (!file.is_open()){
        std::cerr << "Ошибка: не удалось открыть файл\n";
        return -1;
    }

    file << snr_db << " " << n << " " << iterations << " " << res.second << std::endl;

    file.close();

    return channel_simulation_mode_output(n, res.second, iterations - res.first, res.first);
}