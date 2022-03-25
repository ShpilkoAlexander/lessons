#include <iostream>
#include <set>
#include <vector>



using namespace std;

template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << " "s;
    }
    cout << endl;
}

template <typename Container, typename It>
void EraseAndPrint(Container& container, It it) {
    auto it_deleted_el = container.erase(it);
    PrintRange(container.begin(), it_deleted_el );
    PrintRange(it_deleted_el, container.end());
}

template <typename Container>
void EraseAndPrint(Container& container, int position_to_delete) {
    auto it_deleted_el = container.erase(container.begin() + position_to_delete);
    PrintRange(container.begin(), it_deleted_el );
    PrintRange(it_deleted_el, container.end());
}

template <typename Container>
void EraseAndPrint(Container& container, int position_to_del, int range_begin, int range_end) {
    container.erase(container.begin() + position_to_del);
    PrintRange(container.begin(), container.end());

    container.erase(container.begin() + range_begin,container.begin() + range_end);
    PrintRange(container.begin(), container.end());
}


int main() {
    vector<string> langs = {"Python"s, "Java"s, "C#"s, "Ruby"s, "C++"s};
    EraseAndPrint(langs, 0, 0, 2);
}
