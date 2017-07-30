#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>


template <typename RandomIter, typename Comparator, typename Container>
void merge(RandomIter begin, RandomIter middle, RandomIter end, Comparator cmp, Container tmp)
{    
    std::copy(begin, middle, std::back_inserter(tmp));
    
    RandomIter left1 = tmp.begin(), right1 = tmp.end();
    RandomIter left2 = middle, right2 = end;
    RandomIter out = begin;

    while ((left1 != right1) && (left2 != right2)) {
        *out++ = cmp(*left1, *left2) ? *left1++ : *left2++;
    }

    while (left1 != right1) {
        *out++ = *left1++;
    }

    while (left2 != right2) {
        *out++ = *left2++;
    }

    tmp.clear();
}


template <typename RandomIter, typename Comparator>
void merge_sort(RandomIter begin, RandomIter end, Comparator cmp)
{
    size_t size = std::distance(begin, end);

    if (size == 0) {
        return;
    }

    std::vector<typename RandomIter::value_type> tmp; tmp.reserve(size/2 + 1);

    for (size_t chunk_size = 2; chunk_size < 2*size; chunk_size *= 2) {
        for (size_t i = 0; i < size/chunk_size + (size%chunk_size != 0); ++i) {
            RandomIter left = begin + i * chunk_size;
            RandomIter middle = std::min(left + chunk_size/2, end);
            RandomIter right = std::min(left + chunk_size, end);

            merge(left, middle, right, cmp, tmp);
        }        
    }
}



template <typename Comparator>
void test(std::vector<std::vector<int>> tests, Comparator cmp)
{
    for (auto& test: tests) { 
        merge_sort(test.begin(), test.end(), cmp);
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