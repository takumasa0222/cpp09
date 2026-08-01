#include "BitcoinRateDatabase.hpp"
#include "StringUtility.hpp"

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
    bool isHeaderCheck = false;
    while (std::getline(rawData,line))
    {
        if (line.empty()) 
            continue;
        if (!isHeaderCheck){
            isHeaderCheck = true;
            std::string headerLine = StringUtility::trimSpace(line);
            if (headerLine == "date,exchange_rate")
                continue;
        }
    }
}
void BitcoinRateDatabase::bulkLoadRawData(std::ifstream& rawData)
{
    std::string line;
    bool isFirstNonEmptyLine = true;
    unsigned int lineNumber = 0;
    while (std::getline(rawData, line))
    {
        ++lineNumber;
        line = StringUtility::trimSpace(line);
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
        if (!splitLine(line, ',', date, rateText))
        {
            std::cerr << "Error: invalid database format at line " << lineNumber << " => " << line << std::endl;
            continue;
        }
        if (!isValidDate(date))
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

bool BitcoinRateDatabase::isLeapYear(int year)
{
    if (year % 400 == 0)
        return true;
    if (year % 100 == 0)
        return false;
    return year % 4 == 0;
}

bool BitcoinRateDatabase::isValidDate(const std::string& date)
{
    if (date.length() != 10)
        return false;
    if (date[4] != '-' || date[7] != '-')
        return false;
    for (std::size_t i = 0; i < date.length(); ++i)
    {
        if (i == 4 || i == 7)
            continue;
        if (date[i] < '0' || date[i] > '9')
            return false;
    }
    int year = std::atoi(date.substr(0, 4).c_str());
    int month = std::atoi(date.substr(5, 2).c_str());
    int day = std::atoi(date.substr(8, 2).c_str());
    if (year < 1)
        return false;
    if (month < 1 || month > 12)
        return false;
    int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31,30, 31, 30, 31};
    if (isLeapYear(year))
        daysInMonth[1] = 29;
    if (day < 1 || day > daysInMonth[month - 1])
        return false;
    return true;
}

bool BitcoinRateDatabase::splitLine(const std::string& line, char delimiter, std::string& left, std::string& right)
{
    std::size_t delimiterPosition = line.find(delimiter);
    if (delimiterPosition == std::string::npos)
        return false;
    if (line.find(delimiter, delimiterPosition + 1) != std::string::npos)
        return false;
    left = StringUtility::trimSpace(line.substr(0, delimiterPosition));
    right = StringUtility::trimSpace(line.substr(delimiterPosition + 1));
    if (left.empty() || right.empty())
        return false;
    return true;
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
