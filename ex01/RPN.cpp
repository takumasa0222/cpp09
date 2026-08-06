#include "RPN.hpp"
#include <exception>
#include <iostream>

RPN::RPN(){}

RPN::~RPN(){
}

RPN::RPN(const RPN& source)
{
    (void)source;
}

RPN& RPN::operator=(const RPN& source)
{
    (void)source;
    return *this;
}

bool RPN::isBlank(char c) 
{
    return c == ' ';
}

bool RPN::isOperator(char c) 
{
    return c == '+' || c == '-' || c == '*' || c == '/';
}
 
bool RPN::isValidInput(char c) 
{
    return ((c >= '0' && c <= '9') || isOperator(c));
}

void RPN::calculate(std::stack<int>& rpnStack, char op)
{
    if (rpnStack.size() < 2)
        throw std::runtime_error("insufficient operands");

    int right = rpnStack.top();
    rpnStack.pop();

    int left = rpnStack.top();
    rpnStack.pop();

    int result;

    if (op == '+')
        result = left + right;
    else if (op == '-')
        result = left - right;
    else if (op == '*')
        result = left * right;
    else if (op == '/')
    {
        if (right == 0)
            throw std::runtime_error("division by zero");

        result = left / right;
    }
    else
    {
        throw std::runtime_error("Unknown operator");
    }

    rpnStack.push(result);
}
void RPN::calculateRPN(const std::string& expression)
{
    std::stack<int> rpn_stack;
    for (size_t i = 0; i < expression.size(); i++)
    {
        if (isBlank(expression[i]))
            continue;
        if (!isValidInput(expression[i]))
            throw std::runtime_error("invalid input");
        if (isOperator(expression[i]))
        {
            calculate(rpn_stack, expression[i]);
            continue;
        }
        rpn_stack.push(expression[i] - '0');
    }

    if (rpn_stack.size() != 1)
        throw  std::runtime_error("too many operands remaining");
    std::cout << rpn_stack.top() << std::endl;

}