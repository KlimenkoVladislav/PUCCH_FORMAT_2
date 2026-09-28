#pragma once

#include "json.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <random>
#include <optional>
#include <complex>
#include <fstream>

using json = nlohmann::json;

// work_with_json.cpp
std::optional<std::complex<double>> validate_string_to_complex(const std::string& str);
bool validate_extra_fields(const json &data, const std::vector<std::string> &allowed_fields);
bool validate_value(const json &data);
int distribution(std::string filename);
int coding_mode_output(std::vector<std::complex<double>> qpsk_symbols);
int decoding_mode_output(int n, std::vector<int> pucch_f2_bits);
int channel_simulation_mode_output(int n, float bler, int success, int failed);

// mods.cpp
std::vector<int> generate_random_bits(int n);
std::complex<double> string_to_complex(const std::string &str);
int coding_mode(int n, const std::vector<int> bits);
int decoding_mode(int n, const std::vector<std::string> str);
int channel_simulation_mode(int n, int iterations, double snr_db);

std::pair<int, double> BLER(std::vector<int> input_bits, std::vector<int> output_bits, int n);