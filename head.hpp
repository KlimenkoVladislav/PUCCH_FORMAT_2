#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <complex>

std::optional<std::complex<double>> validate_string_to_complex(const std::string& str);
bool validate_extra_fields(const json &data, const std::vector<std::string> &allowed_fields);
bool validate_value(const json &data);
int distribution(std::string filename);

std::vector<int> generate_random_bits(int n);
std::complex<double> string_to_complex(const std::string &str);
void coding_mode(int n, const std::vector<int> bits);
void decoding_mode(int n, const std::vector<std::string> str);
void channel_simulation_mode(int n, int iterations);

class BlockEncoder{
private:
    int _n;
    int _num_of_col = 13;
    int _m = 20;
    std::vector<int> _bits;
    std::vector<int> _encod_data;

    void encoder();
public:
    BlockEncoder(int n, const std::vector<int> bits);
    const std::vector<int> &getEncodData();
};