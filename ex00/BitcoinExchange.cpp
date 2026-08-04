#include "BitcoinExchange.hpp"
#include "Utility.hpp"

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

bool BitcoinExchange::parseValue(
    const std::string& valueText,
    double& value)
{
    errno = 0;
    char* end;
    value = std::strtod(valueText.c_str(), &end);
    if (end == valueText.c_str())
        return false;
    if (*end != '\0')
        return false;
    if (errno == ERANGE)
        return false;
    if (value != value)
        return false;

    if (value > std::numeric_limits<double>::max()
        || value < -std::numeric_limits<double>::max())
    {
        return false;
    }

    return true;
}

void BitcoinExchange::exchange(std::ifstream& in, const BitcoinRateDatabase& bitDb) const
{
    std::string line;
    bool isFirstNonEmptyLine = true;

    while (std::getline(in, line))
    {
        line = Utility::trimSpace(line);
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

        if (!Utility::splitLine(line, '|', date, valueText))
        {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }

        if (!Utility::isValidDate(date))
        {
            std::cerr << "Error: bad input => " << date << std::endl;
            continue;
        }

        if (!parseValue(valueText, value))
        {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }

        if (value < 0.0)
        {
            std::cerr << "Error: not a positive number." << std::endl;
            continue;
        }

        if (value > 1000.0)
        {
            std::cerr << "Error: too large a number." << std::endl;
            continue;
        }

        double exchangeRate;

        if (!bitDb.getExchangeRate(date, exchangeRate))
        {
            std::cerr << "Error: no exchange rate available => " << date << std::endl;
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