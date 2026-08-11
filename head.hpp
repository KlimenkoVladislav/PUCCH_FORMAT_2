#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <complex>

std::optional<std::complex<double>> string_to_complex(const std::string& str);
bool validate_extra_fields(const json &data, const std::vector<std::string> &allowed_fields);
bool validate_value(const json &data);
int distribution(std::string filename);