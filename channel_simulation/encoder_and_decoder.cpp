#include "../head.hpp"

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
    {1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1},
    {1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1},
    {1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1},
    {1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1},
    {1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1},
    {1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1},
    {1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1},
    {1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1},
    {1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1}
};

class BlockEncoder{
private:
    int _n;
    const int _num_of_col = 13;
    const int _m = 20;
    const std::vector<int> _bits;
    std::vector<int> _encod_data;

    void encoder(){
        _encod_data.resize(_m);

        int start_col = _num_of_col - _n;
        for (int i = 0; i < _m; i++){
            int d = (BASIS_MATRIX[i][start_col] * _bits[0]);
            for (int j = start_col + 1; j < _num_of_col; j++){
                d ^= (BASIS_MATRIX[i][j] * _bits[j - start_col]);
            }
            _encod_data[i] = d;
        }
    }

public:
    BlockEncoder(int n, std::vector<int> bits)
                : _n(n), _bits(std::move(bits)){
                    encoder();
                }

    std::vector<int> getEncodData(){
        return std::move(_encod_data);
    }
};