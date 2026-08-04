#ifndef BITCOINRATEDATABASE
#define BITCOINRATEDATABASE
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cerrno>
#include <climits>
#include <limits>
#include <map>
#include <string>

class BitcoinRateDatabase {
public: 
    BitcoinRateDatabase();
    ~BitcoinRateDatabase();
    BitcoinRateDatabase(const BitcoinRateDatabase& source);
    BitcoinRateDatabase& operator=(const BitcoinRateDatabase& source);
    std::map<std::string, double> getDb() const;
    void bulkLoadRawData(std::ifstream& rawData);
    bool getExchangeRate(const std::string date, double &exchangeRate) const;
private:
    std::map<std::string, double> _db;
    static bool parseExchangeRate(const std::string& text, double& exchangeRate);
};

#endif 