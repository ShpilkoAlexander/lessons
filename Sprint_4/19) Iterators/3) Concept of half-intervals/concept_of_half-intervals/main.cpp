#include <iostream>
#include <set>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << " "s;
    }
    cout << endl;
}

template <typename Container, typename Element>
void FindAndPrint(const Container& container, const Element& element){
    auto find_element = find_if(container.begin(), container.end(), [element](const Element& el){
            return el == element;
    });
    PrintRange(container.begin(), find_element);
    PrintRange(find_element, container.end());
}

int main() {
    set<int> test = {1, 1, 1, 2, 3, 4, 5, 5};
    cout << "Test1"s << endl;
    FindAndPrint(test, 3);
    cout << "Test2"s << endl;
    FindAndPrint(test, 0); // элемента 0 нет в контейнере
    cout << "End of tests"s << endl;
}
