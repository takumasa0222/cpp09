#include "BitcoinExchange.hpp"
#include "BitcoinRateDatabase.hpp"
#include <exception>
#include <iostream>
#include <stdexcept>

void runProgram(int argc, char** argv)
{
    if (argc != 2)
        throw std::logic_error("Error: Invalid argument number");
    std::ifstream in(argv[1]);
    if (!in)
        throw std::logic_error("Error: Invalid file");
    std::ifstream rawDb("data.csv");
    if (!rawDb)
        throw std::logic_error("Error: Invalid db file");
    BitcoinRateDatabase bit_db;
    bit_db.bulkLoadRawData(rawDb);
    BitcoinExchange exchanger;
    exchanger.exchange(in ,bit_db);
}

int main(int argc, char** argv){
    try 
    {
        runProgram(argc, argv);
    } 
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}