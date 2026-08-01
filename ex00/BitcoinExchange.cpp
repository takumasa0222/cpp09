#include "BitcoinExchange.hpp"
#include "StringUtility.hpp"

#include <iostream>
#include <cstdlib>
#include <cerrno>
#include <limits>

BitcoinExchange::BitcoinExchange()
{
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& source)
{
    (void)source;
}

BitcoinExchange&
BitcoinExchange::operator=(const BitcoinExchange& source)
{
    (void)source;
    return *this;
}

BitcoinExchange::~BitcoinExchange()
{
}

bool BitcoinExchange::isLeapYear(int year)
{
    if (year % 400 == 0)
        return true;

    if (year % 100 == 0)
        return false;

    return year % 4 == 0;
}

bool BitcoinExchange::isValidDate(const std::string& date)
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

    if (year <= 0)
        return false;

    if (month < 1 || month > 12)
        return false;

    int daysInMonth[12] = {
        31, 28, 31, 30,
        31, 30, 31, 31,
        30, 31, 30, 31
    };

    if (isLeapYear(year))
        daysInMonth[1] = 29;

    if (day < 1 || day > daysInMonth[month - 1])
        return false;

    return true;
}

bool BitcoinExchange::splitInputLine(
    const std::string& line,
    std::string& date,
    std::string& valueText)
{
    std::size_t delimiterPosition = line.find('|');

    if (delimiterPosition == std::string::npos)
        return false;

    // "|" が複数存在する入力を拒否
    if (line.find('|', delimiterPosition + 1) != std::string::npos)
        return false;

    date = StringUtility::trimSpace(
        line.substr(0, delimiterPosition)
    );

    valueText = StringUtility::trimSpace(
        line.substr(delimiterPosition + 1)
    );

    if (date.empty() || valueText.empty())
        return false;

    return true;
}

bool BitcoinExchange::parseValue(
    const std::string& valueText,
    double& value)
{
    errno = 0;

    char* end;
    value = std::strtod(valueText.c_str(), &end);

    // 一文字も数値として読み込めなかった
    if (end == valueText.c_str())
        return false;

    // 数値の後ろに文字が残っている
    if (*end != '\0')
        return false;

    if (errno == ERANGE)
        return false;

    // NaN
    if (value != value)
        return false;

    // infinity
    if (value > std::numeric_limits<double>::max()
        || value < -std::numeric_limits<double>::max())
    {
        return false;
    }

    return true;
}

void BitcoinExchange::exchange(
    std::ifstream& in,
    const BitcoinRateDatabase& bitDb) const
{
    std::string line;
    bool isFirstNonEmptyLine = true;

    while (std::getline(in, line))
    {
        line = StringUtility::trimSpace(line);

        if (line.empty())
            continue;

        if (isFirstNonEmptyLine)
        {
            isFirstNonEmptyLine = false;

            if (line == "date | value")
                continue;
        }

        std::string date;
        std::string valueText;
        double value;

        if (!splitInputLine(line, date, valueText))
        {
            std::cerr
                << "Error: bad input => "
                << line
                << std::endl;
            continue;
        }

        if (!isValidDate(date))
        {
            std::cerr
                << "Error: bad input => "
                << date
                << std::endl;
            continue;
        }

        if (!parseValue(valueText, value))
        {
            std::cerr
                << "Error: bad input => "
                << line
                << std::endl;
            continue;
        }

        if (value < 0.0)
        {
            std::cerr
                << "Error: not a positive number."
                << std::endl;
            continue;
        }

        if (value > 1000.0)
        {
            std::cerr
                << "Error: too large a number."
                << std::endl;
            continue;
        }

        double exchangeRate;

        if (!bitDb.getExchangeRate(date, exchangeRate))
        {
            std::cerr
                << "Error: no exchange rate available => "
                << date
                << std::endl;
            continue;
        }

        std::cout
            << date
            << " => "
            << value
            << " = "
            << value * exchangeRate
            << std::endl;
    }
}