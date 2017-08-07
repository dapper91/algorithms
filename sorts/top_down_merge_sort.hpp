#include <algorithm>


template <typename RandomIt, typename OutIt, typename Comparator>
void merge(RandomIt begin1, RandomIt end1, 
           RandomIt begin2, RandomIt end2, 
           OutIt output, Comparator comp)
{
    while (begin1 != end1 && begin2 != end2) {
        if (comp(*begin1, *begin2)) {
            *output = *begin1;
            output++;
            begin1++;
        }
        else {
            *output = *begin2;
            output++;
            begin2++;
        }
    }

    while (begin1 != end1) {
        *output++ = *begin1++;
    }

    while (begin2 != end2) {
        *output++ = *begin2++;
    }
}

template <typename RandomIt, typename Comparator>
void merge_sort(RandomIt begin, RandomIt end, Comparator comp)
{
    typedef typename RandomIt::value_type value_type;

    size_t size = end - begin;
    RandomIt pivot = begin + size / 2;    

    if (size > 1) {
        value_type* tmp = new value_type[size];

        merge_sort(begin, pivot, comp);
        merge_sort(pivot, end, comp);
        merge(begin, pivot, pivot, end, tmp, comp);

        std::copy(tmp, tmp + size, begin);

        delete[] tmp;
    }
}