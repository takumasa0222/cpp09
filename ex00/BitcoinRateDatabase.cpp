#include "BitcoinRateDatabase.hpp"

BitcoinRateDatabase::BitcoinRateDatabase(){};

BitcoinRateDatabase::~BitcoinRateDatabase(){};

BitcoinRateDatabase::BitcoinRateDatabase(const BitcoinRateDatabase& source){
    this->_db = source.getDb();
}

BitcoinRateDatabase& BitcoinRateDatabase::operator=(const BitcoinRateDatabase& source)
{
    if (this == &source)
        return *this;
    this->_db == source.getDb();
    return *this;
}

std::map<std::string, double> BitcoinRateDatabase::getDb() const 
{
    return _db;
}

void BitcoinRateDatabase::bulkLoadRawData(std::ifstream& rawData)
{
    std::string line;
    std::getline(rawData,line);
}