#include "../include/head.hpp"

std::pair<int, double> BLER(std::vector<int> input_bits, 
                           std::vector<int> output_bits, int iterations){
    static int failed = 0;
    if (input_bits != output_bits){ failed++; }

    return { failed, (double)failed/iterations };
}