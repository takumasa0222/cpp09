#include "BitcoinRateDatabase.hpp"
#include "Utility.hpp"

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
    bool isFirstNonEmptyLine = true;
    unsigned int lineNumber = 0;
    while (std::getline(rawData, line))
    {
        ++lineNumber;
        line = Utility::trimSpace(line);
        if (line.empty())
            continue;
        if (isFirstNonEmptyLine)
        {
            isFirstNonEmptyLine = false;

            if (line == "date,exchange_rate")
                continue;
        }
        std::string date;
        std::string rateText;
        double exchangeRate;
        if (!Utility::splitLine(line, ',', date, rateText))
        {
            std::cerr << "Error: invalid database format at line " << lineNumber << " => " << line << std::endl;
            continue;
        }
        if (!Utility::isValidDate(date))
        {
            std::cerr << "Error: invalid date at line " << lineNumber << " => " << date << std::endl;
            continue;
        }
        if (!parseExchangeRate(rateText, exchangeRate))
        {
            std::cerr << "Error: invalid exchange rate at line " << lineNumber << " => " << rateText << std::endl;
            continue;
        }
        std::pair<std::map<std::string, double>::iterator, bool> result;
        result = _db.insert(std::make_pair(date, exchangeRate));
        if (!result.second)
        {
            std::cerr << "Error: duplicate date at line " << lineNumber << " => " << date << std::endl;
            continue;
        }
    }
}

bool BitcoinRateDatabase::parseExchangeRate(const std::string& text, double& exchangeRate)
{
    errno = 0;
    char* end;
    exchangeRate = std::strtod(text.c_str(), &end);
    if (end == text.c_str())
        return false;
    if (*end != '\0')
        return false;
    if (errno == ERANGE)
        return false;
    if (exchangeRate != exchangeRate)
        return false;
    if (exchangeRate > std::numeric_limits<double>::max() || exchangeRate < -std::numeric_limits<double>::max())
        return false;
    if (exchangeRate < 0.0)
        return false;
    return true;
}

bool BitcoinRateDatabase::getExchangeRate(const std::string date, double &exchangeRate) const
{
    if (_db.empty())
        return false;
    std::map<std::string, double>::const_iterator it;
    it = _db.lower_bound(date);

    if (it != _db.end() && it->first == date)
    {
        exchangeRate = it->second;
        return true;
    }
    if (it == _db.begin())
        return false;
    --it;

    exchangeRate = it->second;
    return true;
}
