#include "../include/head.hpp"
#include "../include/classes.hpp"

const int BASIS_MATRIX[20][13] = {
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0},
    {1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0},
    {1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1},
    {1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1},
    {1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1},
    {1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1},
    {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1},
    {1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1},
    {1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1},
    {1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1},
    {1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1},
    {1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1},
    {1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1},
    {1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1},
    {1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1},
    {1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1},
    {1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1},
    {1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1},
    {1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0}
};

void BlockEncoder::encoder(){
    // for (int i = 0; i < _n; i++){
    //     std::cout << _bits[i] << " ";
    // }
    // std::cout << "\n";

    _encod_data.resize(_m);

    int start_col = _num_of_col - _n;
    for (int i = 0; i < _m; i++){
        int c = BASIS_MATRIX[i][start_col] * _bits[0];
        for (int j = start_col + 1; j < _num_of_col; j++){
            c ^= (BASIS_MATRIX[i][j] * _bits[j - start_col]);
        }
        _encod_data[i] = c;
    }
}

void BlockDecoder::decoder(){
    _bits.resize(_n);
    
    int start_col = _num_of_col - _n;
    double best_metric = -9999999;
    // тут решил немного поработать с массивом бит как с int. потом, 
    // может, и в кодере так же сделаю.
    int best_data = 0;
    
    for (int data = 0; data < (1 << _n); data++){
        std::vector<int> codeword(20, 0);
        for (int i = 0; i < _n; i++){
            if (data & (1 << i)){
                int col = start_col + i;
                for (int j = 0; j < 20; j++){
                    codeword[j] ^= BASIS_MATRIX[j][col];
                }
            }
        }
        
        double metric = 0.0;
        for (int j = 0; j < 20; j++){
            metric += _LLRs[j] * ((codeword[j] == 1)? 1 : -1);
        }
        
        if (metric > best_metric){
            best_metric = metric;
            best_data = data;
        }
    }
    
    for (int i = 0; i < _n; i++){
        _bits[i] = (best_data >> i) & 1;
    }

    // for (int i = 0; i < _n; i++){
    //     std::cout << _bits[i] << " ";
    // }
    // std::cout << "\n\n";
}

BlockEncoder::BlockEncoder(int n, std::vector<int> bits)
            : _n(n), _bits(std::move(bits)){
                encoder();
            }

BlockDecoder::BlockDecoder(int n, std::vector<double> LLRs)
            : _n(n), _LLRs(std::move(LLRs)) {
                decoder();
            }

std::vector<int> BlockEncoder::getEncodData(){
    return std::move(_encod_data);
}

std::vector<int> BlockDecoder::getBits(){
    return std::move(_bits);
}