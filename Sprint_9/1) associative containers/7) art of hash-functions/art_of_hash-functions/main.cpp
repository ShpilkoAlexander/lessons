#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <sstream>

using namespace std;

template <typename Hash>
int FindCollisions(const Hash& hasher, istream& text) {
    string word;
    unordered_map<size_t, unordered_set<string>> words_hash;

    while (text >> word) {
        words_hash[move(hasher(word))].insert(move(word));
    }

    size_t collisions = 0;
    for (const auto& [word_hash, words] : words_hash) {
        collisions += words.size() - 1;
    }

    return collisions;
}

// Это плохой хешер. Его можно использовать для тестирования.
// Подумайте, в чём его недостаток
struct DummyHash {
    size_t operator()(const string&) const {
        return 42;
    }
};


int main() {
    DummyHash dummy_hash;
    hash<string> good_hash;

    {
        istringstream stream("I love C++"s);
        cout << FindCollisions(dummy_hash, stream) << endl;
    }
    {
        istringstream stream("I love C++"s);
        cout << FindCollisions(good_hash, stream) << endl;
    }
}
