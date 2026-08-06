#include "RPN.hpp"
#include <iostream>

int main (int argc, char** argv)
{
    try {
        if (argc != 2)
            throw std::logic_error(" Invalid argument number");
        RPN::calculateRPN(argv[1]);
    }
    catch (const std::exception &e){
        std::cerr << "Error : " << e.what() << std::endl;
        return (1);
    }
    return (0);
}