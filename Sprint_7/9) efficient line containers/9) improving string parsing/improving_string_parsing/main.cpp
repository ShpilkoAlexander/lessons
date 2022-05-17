#include <cassert>
#include <iostream>
#include <string_view>
#include <vector>

using namespace std;

vector<string_view> SplitIntoWordsView(string_view str) {
    vector<string_view> result;
    str.remove_prefix(min(str.find_first_not_of(" "), str.size()));
    //int64_t pos = str.find_first_not_of(" ");
    const int64_t pos_end = str.npos;
    while (!str.empty()) {
        int64_t space = str.find(' ');

        if (space == pos_end) {
            result.push_back(str);
            str = "";
        }
        else {
            result.push_back(str.substr(0, space));
            str.remove_prefix(space + 1);
            str.remove_prefix(min(str.find_first_not_of(" "), str.size()));
        }
    }
    return result;
}

int main() {
    assert((SplitIntoWordsView("") == vector<string_view>{}));
    assert((SplitIntoWordsView("     ") == vector<string_view>{}));
    assert((SplitIntoWordsView("aaaaaaa") == vector{"aaaaaaa"sv}));
    assert((SplitIntoWordsView("a") == vector{"a"sv}));
    assert((SplitIntoWordsView("a b c") == vector{"a"sv, "b"sv, "c"sv}));
    assert((SplitIntoWordsView("a    bbb   cc") == vector{"a"sv, "bbb"sv, "cc"sv}));
    assert((SplitIntoWordsView("  a    bbb   cc") == vector{"a"sv, "bbb"sv, "cc"sv}));
    assert((SplitIntoWordsView("a    bbb   cc   ") == vector{"a"sv, "bbb"sv, "cc"sv}));
    assert((SplitIntoWordsView("  a    bbb   cc   ") == vector{"a"sv, "bbb"sv, "cc"sv}));
    cout << "All OK" << endl;
}
