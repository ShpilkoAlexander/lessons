#include <algorithm>
#include <execution>
#include <iostream>
#include <list>
#include <random>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>
#include <future>

#include "log_duration.h"

using namespace std;

template <typename Strings>
void PrintStrings(const Strings& strings) {
    for (string_view s : strings) {
        cout << s << " ";
    }
    cout << endl;
}

template <typename ForwardRange, typename Function>
void ForEach(ForwardRange& range, Function function, bool check) {
    if (check){};
    static constexpr int PART_COUNT = 4;
    const auto part_length = range.size() / PART_COUNT;
    auto part_begin = range.begin();
    auto part_end = next(part_begin, part_length);

    vector<future<void>> futures;
    for (int i = 0;
         i < PART_COUNT;
         ++i,
             part_begin = part_end,
             part_end = (i == PART_COUNT - 1
                             ? range.end()
                             : next(part_begin, part_length))
         ) {
        futures.push_back(async([function, part_begin, part_end] {
            for_each(part_begin, part_end, function);
        }));
    }
}

template <typename ExecutionPolicy, typename ForwardRange, typename Function>
void ForEach(const ExecutionPolicy& policy, ForwardRange& range, Function function) {

    if constexpr (!is_same_v<decay_t<ExecutionPolicy>, execution::sequenced_policy> &&
            !is_same_v<typename std::iterator_traits<typename ForwardRange::iterator>::iterator_category, std::random_access_iterator_tag>) {
        ForEach(range, function, true);
    }
    else{
        for_each(policy, range.begin(), range.end(), function);
    }
}

template <typename ForwardRange, typename Function>
void ForEach(ForwardRange& range, Function function) {
    ForEach(execution::seq, range, function);
}






int main() {
    auto reverser = [](string& s) { reverse(s.begin(), s.end()); };

    list<string> strings_list = {"cat", "dog", "code"};

    ForEach(strings_list, reverser);
    PrintStrings(strings_list);
    // tac god edoc

    ForEach(execution::seq, strings_list, reverser);
    PrintStrings(strings_list);
    // cat dog code

    // единственный из вызовов, где должна работать ваша версия
    // из предыдущего задания
    ForEach(execution::par, strings_list, reverser);
    PrintStrings(strings_list);
    // tac god edoc

    vector<string> strings_vector = {"cat", "dog", "code"};

    ForEach(strings_vector, reverser);
    PrintStrings(strings_vector);
    // tac god edoc

    ForEach(execution::seq, strings_vector, reverser);
    PrintStrings(strings_vector);
    // cat dog code

    ForEach(execution::par, strings_vector, reverser);
    PrintStrings(strings_vector);
    // tac god edoc

    return 0;
}
