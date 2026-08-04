#include "Utility.hpp"
#include <algorithm>

const std::string Utility::kTrimChar = " \t\n\r";

std::string Utility::trimSpace(const std::string& line)
{
    size_t it_lft = line.find_first_not_of(kTrimChar);
    size_t it_rgt = line.find_last_not_of(kTrimChar);
    if (it_lft == std::string::npos)
        return "";
    return line.substr(it_lft, it_rgt - it_lft + 1);
}

bool Utility::isLeapYear(int year)
{
    if (year % 400 == 0)
        return true;
    if (year % 100 == 0)
        return false;
    return year % 4 == 0;
}

bool Utility::isValidDate(const std::string& date)
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
    if (Utility::isLeapYear(year))
        daysInMonth[1] = 29;
    if (day < 1 || day > daysInMonth[month - 1])
        return false;
    return true;
}

bool Utility::splitLine(const std::string& line, char delimiter, std::string& left, std::string& right)
{
    std::size_t delimiterPosition = line.find(delimiter);
    if (delimiterPosition == std::string::npos)
        return false;
    if (line.find(delimiter, delimiterPosition + 1) != std::string::npos)
        return false;
    left = Utility::trimSpace(line.substr(0, delimiterPosition));
    right = Utility::trimSpace(line.substr(delimiterPosition + 1));
    if (left.empty() || right.empty())
        return false;
    return true;
}

Utility::Utility(){};

Utility::~Utility(){};

Utility::Utility(const Utility& source){
    (void)source;
}

Utility& Utility::operator=(const Utility& source){
    (void)source;
    return *this;
}
