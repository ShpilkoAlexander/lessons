//#include <cassert>
//#include <iostream>
//#include <string_view>
//#include <vector>

//using namespace std;

//vector<string_view> SplitIntoWordsView(string_view str) {
//    vector<string_view> result;
//    str.remove_prefix(min(str.find_first_not_of(" "), str.size()));
//    //int64_t pos = str.find_first_not_of(" ");
//    const int64_t pos_end = str.npos;
//    while (!str.empty()) {
//        int64_t space = str.find(' ');

//        if (space == pos_end) {
//            result.push_back(str);
//            str = "";
//        }
//        else {
//            result.push_back(str.substr(0, space));
//            str.remove_prefix(space + 1);
//            str.remove_prefix(min(str.find_first_not_of(" "), str.size()));
//        }
//    }
//    return result;
//}

//int main() {
//    assert((SplitIntoWordsView("") == vector<string_view>{}));
//    assert((SplitIntoWordsView("     ") == vector<string_view>{}));
//    assert((SplitIntoWordsView("aaaaaaa") == vector{"aaaaaaa"sv}));
//    assert((SplitIntoWordsView("a") == vector{"a"sv}));
//    assert((SplitIntoWordsView("a b c") == vector{"a"sv, "b"sv, "c"sv}));
//    assert((SplitIntoWordsView("a    bbb   cc") == vector{"a"sv, "bbb"sv, "cc"sv}));
//    assert((SplitIntoWordsView("  a    bbb   cc") == vector{"a"sv, "bbb"sv, "cc"sv}));
//    assert((SplitIntoWordsView("a    bbb   cc   ") == vector{"a"sv, "bbb"sv, "cc"sv}));
//    assert((SplitIntoWordsView("  a    bbb   cc   ") == vector{"a"sv, "bbb"sv, "cc"sv}));
//    cout << "All OK" << endl;
//}


//Task 2

#include <cassert>
#include <iostream>
#include <string_view>
#include <vector>
#include <map>

using namespace std;

class Translator {
public:
    Translator() = default;

    void Add(string_view source, string_view target) {
        forward[string(source)] = string(target);
        backward[string(target)] = string(source);
    }

    string_view TranslateForward(string_view source) const {
        string tmp = string(source);
        if (forward.find(tmp) == forward.end())
            return {};
        else{
            return forward.at(tmp);
        }
    }
    string_view TranslateBackward(string_view target) const {
        string tmp = string(target);
        if (backward.find(tmp) == backward.end())
            return {};
        else{
            return backward.at(tmp);
        }
    }

private:
    map<string, string> forward;
    map<string, string> backward;
};




void TestSimple() {
      Translator translator;
    translator.Add(string("okno"s), string("window"s));
    translator.Add(string("stol"s), string("table"s));

    assert(translator.TranslateForward("okno"s) == "window"s);
    assert(translator.TranslateBackward("table"s) == "stol"s);
    assert(translator.TranslateForward("table"s) == ""s);
}

int main() {
    TestSimple();
    return 0;
}
