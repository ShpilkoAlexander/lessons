//Task_2
#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

// не меняйте файлы xml.h и xml.cpp
#include "xml.h"

using namespace std;

struct Spending {
    string category;
    int amount;
};

int CalculateTotalSpendings(const vector<Spending>& spendings) {
    return accumulate(spendings.begin(), spendings.end(), 0,
                      [](int current, const Spending& spending) {
                          return current + spending.amount;
                      });
}

string FindMostExpensiveCategory(const vector<Spending>& spendings) {
    assert(!spendings.empty());
    auto compare_by_amount = [](const Spending& lhs, const Spending& rhs) {
        return lhs.amount < rhs.amount;
    };
    return max_element(begin(spendings), end(spendings), compare_by_amount)->category;
}

vector<Spending> LoadFromXml(istream& input) {
    // место для вашей реализации
    // пример корректного XML-документа в условии

    vector<Spending> spendings;
    Spending spend;

    Document document = Load(input);
    for(const auto& children : document.GetRoot().Children()) {
        spend.amount = children.AttributeValue<int>("amount");
        spend.category = children.AttributeValue<string>("category");
        spendings.push_back(spend);
    }

    return spendings;

}

int main() {
    const vector<Spending> spendings = LoadFromXml(cin);
    cout << "Total "sv << CalculateTotalSpendings(spendings) << '\n';
    cout << "Most expensive is "sv << FindMostExpensiveCategory(spendings) << '\n';
}




























//Task_1
//#include <algorithm>
//#include <cassert>
//#include <iostream>
//#include <numeric>
//#include <vector>

//// не меняйте файлы json.h и json.cpp
//#include "json.h"

//using namespace std;

//struct Spending {
//    string category;
//    int amount;
//};

//int CalculateTotalSpendings(const vector<Spending>& spendings) {
//    return accumulate(
//        spendings.begin(), spendings.end(), 0, [](int current, const Spending& spending){
//            return current + spending.amount;
//        });
//}

//string FindMostExpensiveCategory(const vector<Spending>& spendings) {
//    assert(!spendings.empty());
//    auto compare_by_amount = [](const Spending& lhs, const Spending& rhs) {
//        return lhs.amount < rhs.amount;
//    };
//    return max_element(begin(spendings), end(spendings), compare_by_amount)->category;
//}

//vector<Spending> LoadFromJson(istream& input) {
//    // место для вашей реализации
//    // пример корректного JSON-документа в условии
//    vector<Spending> spendings;
//    Spending spend;

//    Document document = Load(input);
//    for(const auto& el_map : document.GetRoot().AsArray()) {
//        spend.amount = move(el_map.AsMap().at("amount").AsInt());
//        spend.category = move(el_map.AsMap().at("category").AsString());
//        spendings.push_back(spend);
//    }

//    return spendings;

//}

////int main() {
////    const vector<Spending> spendings = {{"food"s, 2500},    {"transport"s, 1150}, {"restaurants"s, 5780},
////                                        {"clothes"s, 7500}, {"travel"s, 23740},   {"sport"s, 12000}};
////    cout << "Total "s << CalculateTotalSpendings(spendings) << '\n';
////    cout << "Most expensive is "s << FindMostExpensiveCategory(spendings) << '\n';
////}

//int main() {
//    // не меняйте main
//    const vector<Spending> spendings = LoadFromJson(cin);
//    cout << "Total "sv << CalculateTotalSpendings(spendings) << '\n';
//    cout << "Most expensive is "sv << FindMostExpensiveCategory(spendings) << '\n';
//}
