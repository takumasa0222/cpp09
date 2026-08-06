#ifndef RPN_HPP
#define RPN_HPP
#include <stack>

#include <string>


class RPN {
private:
    RPN();
    ~RPN();
    RPN(const RPN& source);
    RPN& operator=(const RPN& source);
    static bool isBlank(char c);
    static bool isOperator(char c);
    static bool isValidInput(char c);
    static void calculate(std::stack<int>& rpnStack, char op);

public:
    static void calculateRPN(const std::string& expression);
    

};

#endif

//考慮が必要なパターン
// 0 除算、不正な文字列、演算子・数値不足