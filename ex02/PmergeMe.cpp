#include "PmergeMe.hpp"
#include <climits>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <sys/time.h>

int PmergeMe::parsePositiveInt(const char* str) const
{
    if (str == NULL || *str == '\0')
        throw std::invalid_argument("invalid argument");
    long value = 0;
    for (std::size_t i = 0; str[i] != '\0'; ++i) {
        if (str[i] < '0' || str[i] > '9')
            throw std::invalid_argument("arguments must be positive integers");
        const int digit = str[i] - '0';

        if (value > (INT_MAX - digit) / 10)
            throw std::overflow_error("integer is out of range");

        value = value * 10 + digit;
    }

    if (value <= 0)
        throw std::invalid_argument("arguments must be greater than zero");

    return static_cast<int>(value);
}

double PmergeMe::getTimeMicroseconds() const
{
    struct timeval tv;

    if (gettimeofday(&tv, NULL) != 0)
        throw std::runtime_error("gettimeofday failed");

    return static_cast<double>(tv.tv_sec) * 1000000.0 + static_cast<double>(tv.tv_usec);
}

PmergeMe::PmergeMe(){}

PmergeMe::~PmergeMe(){}

PmergeMe::PmergeMe(const PmergeMe& source){
    (void)source;
}

PmergeMe& PmergeMe::operator=(const PmergeMe& source)
{
    (void)source;
    return *this;
}

void PmergeMe::run(int argc, char** argv)
{
    validateArguments(argc, argv);

    printBefore(argc, argv);
     std::vector<int> vectorValues;
    const double vectorStart = getTimeMicroseconds();
    loadVector(argc, argv, vectorValues);
    mergeInsertionVector(vectorValues);
    const double vectorEnd = getTimeMicroseconds();
    std::deque<int> dequeValues;

    const double dequeStart = getTimeMicroseconds();

    loadDeque(argc,argv,dequeValues);
    mergeInsertionDeque(dequeValues);

    const double dequeEnd =getTimeMicroseconds();
    if (vectorValues.size()!= dequeValues.size()) 
        throw std::logic_error("container results do not match");

    for (std::size_t i = 0;i < vectorValues.size();++i) {
        if (vectorValues[i]!= dequeValues[i])
            throw std::logic_error("container results do not match");
    }
    printAfter(vectorValues);

    std::cout<< std::fixed<< std::setprecision(5);

    std::cout << "Time to process a range of " << vectorValues.size() << " elements with std::vector : " << vectorEnd - vectorStart << " us" << std::endl;

    std::cout << "Time to process a range of " << dequeValues.size() << " elements with std::deque  : " << dequeEnd - dequeStart << " us" << std::endl;
}

void PmergeMe::validateArguments(int argc, char **argv) const
{
    if (argc < 2)
        throw std::invalid_argument( "no input sequence");

    for (int i = 1; i < argc; ++i)
        (void)parsePositiveInt(argv[i]);
}

void PmergeMe::printBefore(int argc,char **argv) const
{
    std::cout << "Before:";
    for (int i = 1; i < argc; ++i)
        std::cout << " " << parsePositiveInt(argv[i]);
    std::cout << std::endl;
}

void PmergeMe::printAfter(const std::vector<int>& values) const
{
    std::cout << "After:";

    for (std::size_t i = 0;i < values.size(); ++i) 
        std::cout<< " "<< values[i];
    std::cout << std::endl;
}

void PmergeMe::loadVector(int argc,char **argv,std::vector<int>& values) const
{
    values.clear();
    values.reserve(static_cast<std::size_t>(argc - 1));

    for (int i = 1; i < argc; ++i) 
        values.push_back(parsePositiveInt(argv[i]));
}

void PmergeMe::loadDeque(int argc,char **argv,std::deque<int>& values) const
{
    values.clear();

    for (int i = 1; i < argc; ++i) 
        values.push_back(parsePositiveInt(argv[i]));
}

void PmergeMe::mergeInsertionVector(std::vector<int>& values)
{
    std::vector<Element> elements;

    elements.reserve(values.size());

    for (std::size_t i = 0;i < values.size(); ++i)
        elements.push_back(Element(values[i], i));

    fordJohnsonVector(elements);

    for (std::size_t i = 0; i < values.size(); ++i) 
        values[i] = elements[i].value;
}

void PmergeMe::mergeInsertionDeque(std::deque<int>& values)
{
    std::deque<Element> elements;

    for (std::size_t i = 0;i < values.size();++i) 
        elements.push_back(Element(values[i], i));
    fordJohnsonDeque(elements);

    for (std::size_t i = 0;i < values.size(); ++i)
        values[i] =elements[i].value;
}
void  PmergeMe::fordJohnsonVector(std::vector<Element>& values)
{
    if (values.size() <= 1)
        return;
    // pair を作る
    std::vector<PairInfo> pairs;
    std::vector<Element> larger;

    pairs.reserve(values.size() / 2);
    larger.reserve(values.size() / 2);

    for (std::size_t i = 0; i + 1 < values.size(); i += 2) 
    {
        PairInfo pair;

        if (values[i].value <= values[i + 1].value) {
            pair.small = values[i];
            pair.large = values[i + 1];
        }
        else {
            pair.small = values[i + 1];
            pair.large = values[i];
        }
        pairs.push_back(pair);
        larger.push_back(pair.large);
    }

     //奇数個かチェック
    const bool hasStray =(values.size() % 2 != 0);
    Element stray;
    if (hasStray)
        stray = values[values.size() - 1];

     // 2. 各ペアの larger 側を Ford-Johnson で再帰的に sort。
    fordJohnsonVector(larger);

     // 3. sort 済み larger に合わせてpair を並べなおす
    std::vector<PairInfo> orderedPairs;
    orderedPairs.reserve(pairs.size());

    for (std::size_t i = 0;i < larger.size();++i) {
        const std::size_t pairIndex = findVectorPair(pairs, larger[i].id);
        orderedPairs.push_back(pairs[pairIndex]);
    }
    //  4. main chain を作る。
    std::vector<Element> mainChain;
    mainChain.reserve(values.size());
    mainChain.push_back(orderedPairs[0].small);

    for (std::size_t i = 0; i < larger.size(); ++i) 
        mainChain.push_back(larger[i]);

    // 5. pending:partnerId には対応する a_i の id を持たせる。
    std::vector<PendingInfo> pending;
    pending.reserve(values.size() / 2 + 1);

    for (std::size_t i = 1;i < orderedPairs.size();++i) {
        pending.push_back(
            PendingInfo(orderedPairs[i].small,true,orderedPairs[i].large.id)
        );
    }

    // 奇数個の場合はペアを持たない
    if (hasStray) {
        pending.push_back(PendingInfo(stray, false, 0));
    }

    const std::size_t maxBIndex = orderedPairs.size() + (hasStray ? 1 : 0);


    // Jacobsthal 順を作る。
    std::vector<std::size_t> insertionOrder;
    buildVectorInsertionOrder(maxBIndex, insertionOrder);

    //7. pending を main chain へ挿入。
    for (std::size_t i = 0;i < insertionOrder.size(); ++i) {
        const std::size_t bIndex = insertionOrder[i];
        PendingInfo& item = pending[bIndex - 2];
        std::size_t searchEnd = mainChain.size();

        if (item.hasPartner) 
            searchEnd = findVectorPartner(mainChain,item.partnerId);
        const std::size_t insertPos = lowerBoundVector(mainChain, searchEnd, item.element.value);

        mainChain.insert(mainChain.begin() + insertPos, item.element);
    }
    values = mainChain;
}

void PmergeMe::fordJohnsonDeque(std::deque<Element>& values)
{
    if (values.size() <= 1)
        return;
    std::deque<PairInfo> pairs;
    std::deque<Element> larger;

    for (std::size_t i = 0; i + 1 < values.size(); i += 2) {
        PairInfo pair;

        if (values[i].value <= values[i + 1].value) {
            pair.small = values[i];
            pair.large = values[i + 1];
        }
        else {
            pair.small = values[i + 1];
            pair.large = values[i];
        }
        pairs.push_back(pair);
        larger.push_back(pair.large);
    }

    const bool hasStray = (values.size() % 2 != 0);
    Element stray;
    if (hasStray)
        stray = values[values.size() - 1];
    fordJohnsonDeque(larger);

    std::deque<PairInfo> orderedPairs;

    for (std::size_t i = 0; i < larger.size(); ++i) {
        const std::size_t pairIndex = findDequePair(pairs, larger[i].id);
        orderedPairs.push_back(pairs[pairIndex]);
    }

    std::deque<Element> mainChain;

    mainChain.push_back(orderedPairs[0].small);

    for (std::size_t i = 0; i < larger.size(); ++i)
        mainChain.push_back(larger[i]);

    std::deque<PendingInfo> pending;

    for (std::size_t i = 1;i < orderedPairs.size();++i)
        pending.push_back(PendingInfo(orderedPairs[i].small,true,orderedPairs[i].large.id));

    if (hasStray) 
        pending.push_back(PendingInfo(stray, false, 0));

    const std::size_t maxBIndex = orderedPairs.size() + (hasStray ? 1 : 0);

    std::deque<std::size_t> insertionOrder;

    buildDequeInsertionOrder(maxBIndex,insertionOrder);

    for (std::size_t i = 0;i < insertionOrder.size();++i) {
        const std::size_t bIndex = insertionOrder[i];
        PendingInfo& item = pending[bIndex - 2];

        std::size_t searchEnd = mainChain.size();

        if (item.hasPartner) 
            searchEnd = findDequePartner(mainChain, item.partnerId);
        const std::size_t insertPos = lowerBoundDeque(mainChain, searchEnd, item.element.value);
        mainChain.insert(mainChain.begin() + insertPos, item.element);
    }

    values = mainChain;
}
PmergeMe::Element::Element() : value(0), id(0){}

PmergeMe::Element::Element(int value, std::size_t id) : value(value), id(id){}

PmergeMe::PendingInfo::PendingInfo() : element(), hasPartner(false), partnerId(0){}

PmergeMe::PendingInfo::PendingInfo(const Element& element,bool hasPartner,std::size_t partnerId) : element(element), hasPartner(hasPartner), partnerId(partnerId){}

void PmergeMe::buildVectorInsertionOrder(std::size_t maxIndex, std::vector<std::size_t>& order) const
{
    if (maxIndex < 2)
        return;

    std::size_t previous = 1;
    std::size_t current = 3;

    while (previous < maxIndex) {
        const std::size_t end = current < maxIndex ? current : maxIndex;

        for (std::size_t i = end; i > previous; --i)
            order.push_back(i);

        if (current >= maxIndex)
            break;

        const std::size_t next = current + 2 * previous;
        previous = current;
        current = next;
    }
}

void PmergeMe::buildDequeInsertionOrder(std::size_t maxIndex, std::deque<std::size_t>& order) const
{
    if (maxIndex < 2)
        return;

    std::size_t previous = 1;
    std::size_t current = 3;

    while (previous < maxIndex) {
        const std::size_t end = current < maxIndex ? current : maxIndex;

        for (std::size_t i = end; i > previous; --i)
            order.push_back(i);

        if (current >= maxIndex)
            break;

        const std::size_t next = current + 2 * previous;
        previous = current;
        current = next;
    }
}

std::size_t PmergeMe::findVectorPair(const std::vector<PairInfo>& pairs, std::size_t largeId) const
{
    for (std::size_t i = 0; i < pairs.size(); ++i) {
        if (pairs[i].large.id == largeId)
            return i;
    }
    throw std::logic_error("partner pair was not found");
}

std::size_t PmergeMe::findDequePair(const std::deque<PairInfo>& pairs, std::size_t largeId) const
{
    for (std::size_t i = 0; i < pairs.size(); ++i) {
        if (pairs[i].large.id == largeId)
            return i;
    }
    throw std::logic_error("partner pair was not found");
}

std::size_t PmergeMe::findVectorPartner(const std::vector<Element>& chain, std::size_t partnerId) const
{
    for (std::size_t i = 0; i < chain.size(); ++i) {
        if (chain[i].id == partnerId)
            return i;
    }
    throw std::logic_error("partner element was not found");
}

std::size_t PmergeMe::findDequePartner(const std::deque<Element>& chain, std::size_t partnerId) const
{
    for (std::size_t i = 0; i < chain.size(); ++i) {
        if (chain[i].id == partnerId)
            return i;
    }
    throw std::logic_error("partner element was not found");
}

std::size_t PmergeMe::lowerBoundVector(const std::vector<Element>& chain, std::size_t end, int value) const
{
    std::size_t left = 0;
    std::size_t right = end;

    while (left < right) {
        const std::size_t middle = left + (right - left) / 2;

        if (chain[middle].value < value)
            left = middle + 1;
        else
            right = middle;
    }
    return left;
}

std::size_t PmergeMe::lowerBoundDeque(const std::deque<Element>& chain, std::size_t end, int value) const
{
    std::size_t left = 0;
    std::size_t right = end;

    while (left < right) {
        const std::size_t middle = left + (right - left) / 2;

        if (chain[middle].value < value)
            left = middle + 1;
        else
            right = middle;
    }
    return left;
}