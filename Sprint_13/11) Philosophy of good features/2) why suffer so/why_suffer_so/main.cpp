#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum class Gender { FEMALE, MALE };

struct Person {
    int age;           // возраст
    Gender gender;     // пол
    bool is_employed;  // имеет ли работу
};

template <typename InputIt>
int ComputeMedianAge(InputIt range_begin, InputIt range_end) {
    if (range_begin == range_end) {
        return 0;
    }
    vector<typename InputIt::value_type> range_copy(range_begin, range_end);
    auto middle = range_copy.begin() + range_copy.size() / 2;
    nth_element(range_copy.begin(), middle, range_copy.end(), [](const Person& lhs, const Person& rhs) {
        return lhs.age < rhs.age;
    });
    return middle->age;
}

// напишите сигнатуру и реализацию функции PrintStats
void PrintStats(vector<Person>& persons) {

    cout << "Median age = "s << ComputeMedianAge(persons.begin(), persons.end()) << endl;

    auto it = std::partition(persons.begin(), persons.end(), [](Person& person){return person.gender == Gender::FEMALE;});
    vector<Person> females (persons.begin(), it);
    vector<Person> males (it, persons.end());

    it = std::partition(females.begin(), females.end(), [](Person& person){return person.is_employed == true;});
    vector<Person> employed_females (females.begin(), it);
    vector<Person> unemployed_females (it, females.end());

    it = std::partition(males.begin(), males.end(), [](Person& person){return person.is_employed == true;});
    vector<Person> employed_males (males.begin(), it);
    vector<Person> unemployed_males (it, males.end());


    cout << "Median age for females = "s << ComputeMedianAge(females.begin(), females.end()) << endl;
    cout << "Median age for males = "s << ComputeMedianAge(males.begin(), males.end()) << endl;
    cout << "Median age for employed females = "s << ComputeMedianAge(employed_females.begin(), employed_females.end()) << endl;
    cout << "Median age for unemployed females = "s << ComputeMedianAge(unemployed_females.begin(), unemployed_females.end())<< endl;
    cout << "Median age for employed males = "s << ComputeMedianAge(employed_males.begin(), employed_males.end()) << endl;
    cout << "Median age for unemployed males = "s << ComputeMedianAge(unemployed_males.begin(), unemployed_males.end())<< endl;
}

int main() {
    vector<Person> persons = {
        {31, Gender::MALE, false},   {40, Gender::FEMALE, true},  {24, Gender::MALE, true},
        {20, Gender::FEMALE, true},  {80, Gender::FEMALE, false}, {78, Gender::MALE, false},
        {10, Gender::FEMALE, false}, {55, Gender::MALE, true},
    };
    PrintStats(persons);
}
