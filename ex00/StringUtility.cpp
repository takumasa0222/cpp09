#include "StringUtility.hpp"
#include <algorithm>

const std::string StringUtility::kTrimChar = " \t\n\r";

std::string StringUtility::trimSpace(const std::string& line)
{
    size_t it_lft = line.find_first_not_of(kTrimChar);
    size_t it_rgt = line.find_last_not_of(kTrimChar);
    if (it_lft == std::string::npos)
        return "";
    return line.substr(it_lft, it_rgt - it_lft + 1);
}

StringUtility::StringUtility(){};

StringUtility::~StringUtility(){};

StringUtility::StringUtility(const StringUtility& source){}

StringUtility& StringUtility::operator=(const StringUtility& source){}
