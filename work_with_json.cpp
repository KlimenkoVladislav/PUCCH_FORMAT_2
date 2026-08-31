#include "include/head.hpp"

#include <fstream>

std::optional<std::complex<double>> validate_string_to_complex(const std::string &str){
    if (str.empty()){
        std::cerr << "Ошибка: строка пуста\n";
        return std::nullopt;
    }

    double real, imag;
    char sign, j;

    std::stringstream ss(str);
    ss >> real >> sign >> imag >> j;
    if (ss.fail()){
        std::cerr << "Ошибка: не удалось распарсить строку '" << str << "'" << std::endl;
        return std::nullopt;
    }

    if (j != 'j'){
        std::cerr << "Ошибка: строка должна заканчиваться на 'j': " << str << std::endl;
        return std::nullopt;
    }
    
    if (sign == '-'){ imag = -imag; }
    
    return std::complex<double>(real, imag);
}

bool validate_extra_fields(const json &data, const std::vector<std::string> &allowed_fields){
    for (const auto &[key, value] : data.items()){
        bool valid_field = false;
        for (const std::string &allowed_field : allowed_fields){
            if (key == allowed_field){
                valid_field = true;
                break;
            }
        }
        if (!valid_field){
            std::cerr << "Ошибка: поля " << key << " существовать не должно\n";
            std::cerr << "Разрешённые поля:\n";
            for (const std::string &field: allowed_fields){
                std::cerr << field << std::endl;
            }
            return false;
        }
    }
    
    return true;
}

bool validate_value(const json &data){
    int n;
    for (const auto &[key, value] : data.items()){
        if (key == "num_of_pucch_f2_bits"){
            if (!value.is_number_integer()){
                std::cerr << "Ошибка: поле " << key << " должно иметь тип int\n";
                return false;
            }
            if (!((value == 2) or (value == 4) or (value == 6) or (value == 8) or (value == 11))){
                std::cerr << "Ошибка: поле " << key << " сожержит недопустимое значение\n";
                return false;
            }
            n = value;
        }
        else if (key == "pucch_f2_bits"){
            if (!value.is_array()){
                std::cerr << "Ошибка: поле " << key << " должно быть array\n";
                return false;
            }
            if (value.size() != n){
                std::cerr << "Ошибка: поле 'pucch_f2_bits' содержит неверное количество бит";
                return false;
            }
            for (const auto val : value){
                if (!((val == 0) or (val == 1))){
                    std::cerr << "Ошибка: поле " << key << " должно содержать биты\n";
                    return false;
                }
            }
        }
        else if (key == "qpsk_symbols"){
            if (!value.is_array()){
                std::cerr << "Ошибка: поле " << key << " должно быть array\n";
                return false;
            }

            // это можно провалидировать сразу, т.к. выходной (зашифрованный) массив 
            // содержит m=20 бит; QPSK может передавать по 2 бита, поэтому количество 
            // QPSK символов всегда равно 10.
            if (value.size() != 10){
                std::cerr << "Ошибка: QPSK символов должно быть 10\n";
                return false;
            }

            for (const auto &val : value){
                if (!val.is_string()){
                    std::cerr << "Ошибка: значения в поле " << key << " должны быть типа string\n";
                    return false;
                }
                if (!validate_string_to_complex(val).has_value()){
                    return false;
                }
            }
        }
        else if (key == "iterations"){
            if (!value.is_number_integer()){
                std::cerr << "Ошибка: поле " << key << " должно иметь тип int\n";
                return false;
            }
            if (value < 1){
                std::cerr << "Ошибка: нулевое или отрицательное количество итераций\n";
                return false;
            }
        }
    }

    return true;
}

int distribution(std::string filename){
    std::ifstream file(filename);
    if (!file.is_open()){
        std::cerr << "Не удалось открыть файл " << filename << std::endl;
        return -1;
    }

    json data;
    try{
        file >> data;
    } catch (const json::parse_error& e){
        std::cerr << "Ошибка: невалидный json: " << e.what() << std::endl;
        return -2;
    }

    if (!data.contains("mode")){
        std::cerr << "Ошибка: отсутствует поле 'mode'\n";
        return -3;
    }
    if (!data["mode"].is_string()){
        std::cerr << "Ошибка: поле 'mode' должно быть строкой" << std::endl;
        return -3;
    }

    if (data["mode"] == "coding"){
        if (!data.contains("num_of_pucch_f2_bits") or !data.contains("pucch_f2_bits")){
            std::cerr << "Ошибка: отсутствуют нужные для режима coding поля\n";
            return -4;
        }
        std::vector<std::string> allowed_fields = {"mode", "num_of_pucch_f2_bits", "pucch_f2_bits"};
        if (!validate_extra_fields(data, allowed_fields)){ return -5; }
        if (!validate_value(data)){ return -6; }

        return coding_mode(data["num_of_pucch_f2_bits"], data["pucch_f2_bits"]);
    }
    else if (data["mode"] == "decoding"){
        if (!data.contains("num_of_pucch_f2_bits") or !data.contains("qpsk_symbols")){
            std::cerr << "Ошибка: отсутствуют нужные для режима decoding поля\n";
            return -4;
        }
        std::vector<std::string> allowed_fields = {"mode", "num_of_pucch_f2_bits", "qpsk_symbols"};
        if (!validate_extra_fields(data, allowed_fields)){ return -5; }
        if (!validate_value(data)){ return -6; }

        return decoding_mode(data["num_of_pucch_f2_bits"], data["qpsk_symbols"]);
    }
    else if (data["mode"] == "channel simulation"){
        if (!data.contains("num_of_pucch_f2_bits") or !data.contains("iterations")){
            std::cerr << "Ошибка: отсутствуют нужные для режима channel simulation поля\n";
            return -4;
        }
        std::vector<std::string> allowed_fields = {"mode", "num_of_pucch_f2_bits", "iterations"};
        if (!validate_extra_fields(data, allowed_fields)){ return -5; }
        if (!validate_value(data)){ return -6; }

        return channel_simulation_mode(data["num_of_pucch_f2_bits"], data["iterations"]);
    }
    else {
        std::cerr << "Ошибка: данного значения поля 'mode' существовать не может\n";
        return -3;
    }
}

int coding_mode_output(std::vector<std::complex<double>> qpsk_symbols){
    std::ofstream file("../result.json", std::ios::out);
    if (!file.is_open()){
        std::cerr << "Не удалось открыть файл result.json\n";
        return -7;
    }

    json output_data;
    output_data["mode"] = "coding";
    
    std::vector<std::string> symbols_str;
    for (const auto& sym : qpsk_symbols){
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(3) 
            << sym.real() << "+" << sym.imag() << "j";
        symbols_str.push_back(oss.str());
    }
    output_data["qpsk_symbols"] = symbols_str;

    file << output_data.dump(4);
    file.close();

    return 0;
}

int decoding_mode_output(int n, std::vector<int> pucch_f2_bits){
    std::ofstream file("../result.json", std::ios::out);
    if (!file.is_open()){
        std::cerr << "Не удалось открыть файл result.json\n";
        return -7;
    }

    json output_data;
    output_data["mode"] = "decoding";
    output_data["num_of_pucch_f2_bits"] = n;
    output_data["pucch_f2_bits"] = pucch_f2_bits;

    file << output_data.dump(4);
    file.close();

    return 0;
}

int channel_simulation_mode_output(int n, float bler, int success, int failed){
    std::ofstream file("../result.json", std::ios::out);
    if (!file.is_open()){
        std::cerr << "Не удалось открыть файл result.json\n";
        return -7;
    }

    json output_data;
    output_data["mode"] = "channel simulation";
    output_data["num_of_pucch_f2_bits"] = n;
    output_data["bler"] = std::round(bler * 1000.0) / 1000.0;
    output_data["success"] = success;
    output_data["failed"] = failed;

    file << output_data.dump(4);
    file.close();

    return 0;
}