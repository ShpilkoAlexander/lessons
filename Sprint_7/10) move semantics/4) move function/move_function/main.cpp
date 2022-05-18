#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>
#include <valarray>
#include <list>

using namespace std;

template <typename RandomIt>
void MakeJosephusPermutation(RandomIt first, RandomIt last, uint32_t step_size) {
    list<typename RandomIt::value_type> pool;
    for (auto i = first; i < last; ++i) {
        pool.push_back(move(*i));
    }
    size_t cur_pos = 0;
    auto ptr_pool = pool.begin();
    while (!pool.empty()) {
        *(first++) = move(*ptr_pool);
        pool.erase(ptr_pool);
        if (move(pool.empty())) {
            break;
        }
        cur_pos = move((cur_pos + step_size - 1) % move(pool.size()));
        ptr_pool = pool.begin();
        advance(ptr_pool, cur_pos);
    }
}

template <typename RandomIt>
void MakeJosephusPermutationOld(RandomIt first, RandomIt last, uint32_t step_size) {
    vector<typename RandomIt::value_type> pool(first, last);
    size_t cur_pos = 0;
    while (!pool.empty()) {
        *(first++) = pool[cur_pos];
        pool.erase(pool.begin() + cur_pos);
        if (pool.empty()) {
            break;
        }
        cur_pos = (cur_pos + step_size - 1) % pool.size();
    }
}

vector<int> MakeTestVector() {
    vector<int> numbers(100);
    iota(begin(numbers), end(numbers), 0);
    return numbers;
}

void TestIntVector() {
    const vector<int> numbers = MakeTestVector();
    {
        vector<int> numbers_copy = numbers;
        MakeJosephusPermutationOld(begin(numbers_copy), end(numbers_copy), 5);
        for (int g : numbers_copy) {
            cout << g << ", ";
        }
        cout << endl;

        vector<int> copy2 = numbers;
        MakeJosephusPermutation(begin(copy2), end(copy2), 5);
        for (int g : copy2) {
            cout << g << ", ";
        }
        cout << endl;


        assert(copy2 == numbers_copy);
        cout << "DONE" << endl;
    }

}

// Это специальный тип, который поможет вам убедиться, что ваша реализация
// функции MakeJosephusPermutation не выполняет копирование объектов.
// Сейчас вы, возможно, не понимаете как он устроен, однако мы расскажем
// об этом далее в нашем курсе

struct NoncopyableInt {
    int value;

    NoncopyableInt(const NoncopyableInt&) = delete;
    NoncopyableInt& operator=(const NoncopyableInt&) = delete;

    NoncopyableInt(NoncopyableInt&&) = default;
    NoncopyableInt& operator=(NoncopyableInt&&) = default;
};

bool operator==(const NoncopyableInt& lhs, const NoncopyableInt& rhs) {
    return lhs.value == rhs.value;
}

ostream& operator<<(ostream& os, const NoncopyableInt& v) {
    return os << v.value;
}

int main() {
    TestIntVector();
}
