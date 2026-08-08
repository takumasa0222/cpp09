#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <cstddef>

class PmergeMe {
public:
    PmergeMe();
    ~PmergeMe();
    PmergeMe(const PmergeMe& source);
    PmergeMe& operator=(const PmergeMe& source);

    void run(int argc, char **argv);

private:
    struct Element {
        int value;
        std::size_t id;

        Element();
        Element(int value, std::size_t id);
    };

    struct PairInfo {
        Element small;
        Element large;
    };

    struct PendingInfo {
        Element element;
        bool hasPartner;
        std::size_t partnerId;

        PendingInfo();
        PendingInfo(
            const Element& element,
            bool hasPartner,
            std::size_t partnerId
        );
    };

    void validateArguments(int argc, char **argv) const;

    void loadVector(int argc,char **argv,std::vector<int>& values) const;

    void loadDeque(int argc,char **argv,std::deque<int>& values) const;

    void mergeInsertionVector(std::vector<int>& values);
    void mergeInsertionDeque(std::deque<int>& values);

    void fordJohnsonVector(std::vector<Element>& values);
    void fordJohnsonDeque(std::deque<Element>& values);

    void buildVectorInsertionOrder(std::size_t maxIndex,std::vector<std::size_t>& order) const;

    void buildDequeInsertionOrder(std::size_t maxIndex,std::deque<std::size_t>& order) const;

    std::size_t findVectorPair(const std::vector<PairInfo>& pairs,std::size_t largeId) const;

    std::size_t findDequePair(const std::deque<PairInfo>& pairs,std::size_t largeId) const;

    std::size_t findVectorPartner(const std::vector<Element>& chain,std::size_t partnerId) const;

    std::size_t findDequePartner(const std::deque<Element>& chain,std::size_t partnerId) const;

    std::size_t lowerBoundVector(const std::vector<Element>& chain,std::size_t end,int value) const;

    std::size_t lowerBoundDeque(const std::deque<Element>& chain,std::size_t end,int value) const;

    int parsePositiveInt(const char *str) const;
    double getTimeMicroseconds() const;

    void printBefore(int argc, char **argv) const;
    void printAfter(const std::vector<int>& values) const;
};

#endif