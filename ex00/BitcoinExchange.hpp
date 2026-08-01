#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP
#include <map>
#include "BitcoinRateDatabase.hpp"

class BitcoinExchange {
public:
    BitcoinExchange();
    ~BitcoinExchange();
    BitcoinExchange(const BitcoinExchange& source);
    BitcoinExchange operator=(const BitcoinExchange& source);
    void exchange(const std::ifstream& in, const BitcoinRateDatabase& bitDb) const;
private:

};

#endif