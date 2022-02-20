#include <iostream>
#include <vector>
#include <set>


using namespace std;



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
    Print(out, container);
    return out;
}

template <typename Element>
ostream& operator<<(ostream& out, const set<Element> container){
    Print(out, container);
    return out;
}

int main() {
    const vector<int> ages = {10, 5, 2, 12};
    cout << ages << endl;
}
