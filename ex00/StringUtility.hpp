#ifndef STRINGUTILITY_HPP
#define STRINGUTILITY_HPP
#include <string>

class StringUtility {
private:
    StringUtility();
    ~StringUtility();
    StringUtility(const StringUtility& source);
    StringUtility& operator=(const StringUtility& source);
    static const std::string kTrimChar;
public:
    static std::string trimSpace(const std::string& s);
};

#endif
