#include <iostream>
#include <set>
#include <algorithm>
#include <vector>


using namespace std;

//Task_1
set<int>::const_iterator FindNearestElement(const set<int>& numbers, int border) {
    if (numbers.empty()){
        return numbers.end();
    }

    auto num_above_point = numbers.lower_bound(border);
    if (num_above_point == numbers.end()){
        return prev(numbers.end());
    }
    if (*num_above_point == border || num_above_point == numbers.begin()) {
        return num_above_point;
    }

    int num_above = *num_above_point;
    auto num_below_point = prev(num_above_point);
    int num_below = *num_below_point;
    if ((border - num_below) > (num_above - border)) {
        return num_above_point;
    }

    else{
        return num_below_point;
    }


}

template <typename RandomIt>
pair<RandomIt, RandomIt> FindStartsWith(RandomIt range_begin, RandomIt range_end, string prefix) {
    // Все строки, начинающиеся с prefix, больше или равны строке "<prefix>"
    auto left = lower_bound(range_begin, range_end, prefix);
    // Составим строку, которая в рамках буквенных строк является точной верхней гранью
    // множества строк, начинающихся с prefix
    string upper_bound = prefix;
    ++upper_bound[upper_bound.size() - 1];
    // Первое встреченное слово, не меньшее upper_bound, обязательно является концом полуинтервала
    auto right = lower_bound(range_begin, range_end, upper_bound);
    return {left, right};
}


int main() {
    const vector<string> sorted_strings = {"moscow", "motovilikha", "murmansk"};

    const auto mo_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), "mo");
    for (auto it = mo_result.first; it != mo_result.second; ++it) {
        cout << *it << " ";
    }
    cout << endl;

    const auto mt_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), "mt");
    cout << (mt_result.first - begin(sorted_strings)) << " " << (mt_result.second - begin(sorted_strings)) << endl;

    const auto na_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), "na");
    cout << (na_result.first - begin(sorted_strings)) << " " << (na_result.second - begin(sorted_strings)) << endl;

    return 0;
}
