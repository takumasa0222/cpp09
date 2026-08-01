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
private:
    std::map<std::string, double> _db;
    static bool isLeapYear(int year);
    static bool isValidDate(const std::string& date);
    static bool splitLine(const std::string& line, char delimiter, std::string& left, std::string& right);
    static bool parseExchangeRate(const std::string& text, double& exchangeRate);
};

#endif 