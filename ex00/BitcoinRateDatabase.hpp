#ifndef BITCOINRATEDATABASE
#define BITCOINRATEDATABASE
#include <map>
#include <string>
#include <fstream>
#include <istream>

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
};

#endif BITCOINRATEDATABASE