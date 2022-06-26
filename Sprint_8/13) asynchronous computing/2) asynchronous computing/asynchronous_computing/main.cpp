#include <algorithm>
#include <execution>
#include <iostream>
#include <future>

using namespace std;

template <typename RandomAccessIterator, typename Value>
RandomAccessIterator LowerBound(const execution::parallel_policy&,
                                RandomAccessIterator range_begin, RandomAccessIterator range_end,
                                const Value& value) {
    auto left_bound = range_begin;
    auto right_bound = range_end;
//    const int length = (right_bound - left_bound) / 3;
//    const auto left_bound = left_bound + max(1, length);
//    const auto right_bound  = right_bound - max(1, length);

    while (left_bound + 1 < right_bound) {
        const int length = (right_bound - left_bound) / 3;
        const auto mid_l = left_bound + max(1, length);
        const auto mid_r = right_bound - max(1, length);

        future<bool> midr = async([value, mid_r] { return !(*mid_r < value); });

        if (!(*mid_l < value)) {
            right_bound = mid_l ;
        } else if (midr.get()) {
            right_bound = mid_r ;
            left_bound = mid_l + 1;
        } else left_bound = mid_r + 1;
    }


    if (left_bound == range_begin && !(*left_bound < value)) {
        return left_bound;
    } else {
        return right_bound;
    }
}

template <typename RandomAccessIterator, typename Value>
RandomAccessIterator LowerBound(RandomAccessIterator range_begin, RandomAccessIterator range_end,
                                const Value& value) {
    auto left_bound = range_begin;
    auto right_bound = range_end;

    while (left_bound + 1 < right_bound) {
        const int length = (right_bound - left_bound) / 3;
        const auto mid_l = left_bound + max(1, length);
        const auto mid_r = right_bound - max(1, length);

        if (!(*mid_l < value)) {
            right_bound = mid_l ;
        } else if (!(*mid_r < value)) {
            right_bound = mid_r ;
            left_bound = mid_l + 1;
        } else left_bound = mid_r + 1;
    }


    if (left_bound == range_begin && !(*left_bound < value)) {
        return left_bound;
    } else {
        return right_bound;
    }
}

template <typename RandomAccessIterator, typename Value>
RandomAccessIterator LowerBound(const execution::sequenced_policy&,
                                RandomAccessIterator range_begin, RandomAccessIterator range_end,
                                const Value& value) {
    auto left_bound = range_begin;
    auto right_bound = range_end;
    while (left_bound + 1 < right_bound) {
        const auto middle = left_bound + (right_bound - left_bound) / 2;
        if (*middle < value) {
            left_bound = middle;
        } else {
            right_bound = middle;
        }
    }
    if (left_bound == range_begin && !(*left_bound < value)) {
        return left_bound;
    } else {
        return right_bound;
    }
}
int main() {
    const vector<string> strings = {"cat", "dog", "dog", "horse"};

    const vector<string> requests = {"bear", "cat", "deer", "dog", "dogs", "horses"};

    // последовательные версии
    cout << "Request [" << requests[0] << "] → position "
         << LowerBound(strings.begin(), strings.end(), requests[0]) - strings.begin() << endl;
    cout << "Request [" << requests[1] << "] → position "
         << LowerBound(execution::seq, strings.begin(), strings.end(), requests[1])
            - strings.begin()
         << endl;
    cout << "Request [" << requests[2] << "] → position "
         << LowerBound(execution::seq, strings.begin(), strings.end(), requests[2])
            - strings.begin()
         << endl;

    // параллельные
    cout << "Request [" << requests[3] << "] → position "
         << LowerBound(execution::par, strings.begin(), strings.end(), requests[3])
            - strings.begin()
         << endl;
    cout << "Request [" << requests[4] << "] → position "
         << LowerBound(execution::par, strings.begin(), strings.end(), requests[4])
            - strings.begin()
         << endl;
    cout << "Request [" << requests[5] << "] → position "
         << LowerBound(execution::par, strings.begin(), strings.end(), requests[5])
            - strings.begin()
         << endl;
}
