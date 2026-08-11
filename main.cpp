#include "head.hpp"

int main(){
    std::string filename;
    std::cout << "Введите название json файла: ";
    std::getline(std::cin, filename);

    return distribution(filename);
}