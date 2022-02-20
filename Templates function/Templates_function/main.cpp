#include <iostream>
#include <vector>
#include <set>
#include <map>

using namespace std;



template <typename Key, typename Value>
ostream& operator<<(ostream& out, const pair<Key, Value> container){
    out << "(" << container.first << ", " << container.second << ")";
    return out;
}

template <typename Collection>
ostream& Print(ostream& out, const Collection container){
    bool isFirst = true;
    for(const auto& element : container){
        if(!isFirst){
            out << ", ";
        }
        out << element;
        isFirst = false;
    }
    return out;
}

template <typename Element>
ostream& operator<<(ostream& out, const vector<Element> container){
    out << "[";
    Print(out, container);
    out << "]";

    return out;
}

template <typename Element>
ostream& operator<<(ostream& out, const set<Element> container){
    out << "{";
    Print(out, container);
    out << "}";

    return out;
}

template <typename Key, typename Value>
ostream& operator<<(ostream& out, const map<Key, Value> container){
    out << "<";
    Print(out, container);
    out << ">";
    return out;
}

int main() {
    const set<string> cats = {"Мурка"s, "Белка"s, "Георгий"s, "Рюрик"s};
    cout << cats << endl;

    const vector<int> ages = {10, 5, 2, 12};
    cout << ages << endl;

    const map<string, int> cat_ages = {
        {"Мурка"s, 10},
        {"Белка"s, 5},
        {"Георгий"s, 2},
        {"Рюрик"s, 12}
    };
    cout << cat_ages << endl;
}
