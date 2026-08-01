#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <fstream>
#include <string>
#include "BitcoinRateDatabase.hpp"

class BitcoinExchange
{
public:
    BitcoinExchange();
    BitcoinExchange(const BitcoinExchange& source);
    BitcoinExchange& operator=(const BitcoinExchange& source);
    ~BitcoinExchange();
    void exchange(std::ifstream& in, const BitcoinRateDatabase& bitDb) const;

private:
    static bool splitInputLine(const std::string& line, std::string& date, std::string& valueText);
    static bool isValidDate(const std::string& date);
    static bool parseValue(const std::string& valueText, double& value);
    static bool isLeapYear(int year);
};

#endif