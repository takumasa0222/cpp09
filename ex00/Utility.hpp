#ifndef UTILITY_HPP
#define UTILITY_HPP
#include <string>

class Utility {
private:
    Utility();
    ~Utility();
    Utility(const Utility& source);
    Utility& operator=(const Utility& source);
    static const std::string kTrimChar;
public:
    static std::string trimSpace(const std::string& s);
    static bool isLeapYear(int year);
    static bool isValidDate(const std::string& date);
    static bool splitLine(const std::string& line, char delimiter, std::string& left, std::string& right);
    
};

#endif
