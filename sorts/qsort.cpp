#include <vector>
#include <iterator>
#include <functional>
#include <iostream>


template <typename RandomIterator, typename Comparator>
RandomIterator partition(RandomIterator begin, RandomIterator end, Comparator cmp)
{
    RandomIterator left = begin, right = begin, pivot = end - 1;

    while (right != pivot) {
        if (cmp(*right, *pivot)) {
            std::swap(*left, *right);
            ++left;
        }
        ++right;
    }

    std::swap(*pivot, *left);

	return left;
}


template <typename RandomIterator, typename Comparator>
void qsort(RandomIterator begin, RandomIterator end, Comparator cmp)
{
    if (std::distance(begin, end) <= 1) {
        return;
    }

    RandomIterator pivot = partition(begin, end, cmp);

    qsort(begin, pivot, cmp);
    qsort(pivot + 1, end, cmp);
}


template <typename Comparator>
void test(std::vector<std::vector<int>> tests, Comparator cmp)
{
    for (auto& test: tests) { 
        qsort(test.begin(), test.end(), cmp);
        for (auto& val: test) {
            std::cout << val << " ";
        }
 
        std::cout << std::endl;
    }
}


int main()
{    
    std::vector<std::vector<int>> tests = {
        {},
        {1},
        {1,1},
        {2,2,2},
        {1,2,3,4},
        {4,3,2,1},
        {-1,0,1,2},
        {1,6,3,8,4,8,2,2,1},
        {1,6,3,8,4,8,2,2,0},
        {1,6,3},
        {1,2},
        {2,1}
    };

    std::cout << "===less or equal===" << std::endl;

    test(tests, std::less_equal<int>());

    std::cout << "======greater======" << std::endl;

    test(tests, std::greater<int>());    

    return 0;
}