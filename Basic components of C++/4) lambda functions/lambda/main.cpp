// Урок 2 задание 2
/*
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

void ToLowerString(string& str){
    for(char& c : str){
        c = (char)tolower(c);
    }
}

int main() {
    // считайте входные данные и сформируйте вывод программы
    // ...

    int words_count = 0;
    cin >> words_count;

    string word;
    vector<string> words;
    for(int i = 0; i < words_count; ++i){
        cin >> word;
        words.push_back(word);
    }

    sort(words.begin(), words.end(),
         [] (string l_str, string r_str) {
        ToLowerString(l_str);
        ToLowerString(r_str);
        return lexicographical_compare(l_str.begin(), l_str.end(), r_str.begin(), r_str.end());
    });
    for(const string& word : words){
        cout << word << " ";
    }
    cout << endl;


}
*/


// Урок 2 задание 3
/*
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

vector<int> ReadNumbers() {
    int count;
    cin >> count;

    vector<int> numbers;
    for (int i = 0; i < count; ++i) {
        int number;
        cin >> number;
        numbers.push_back(number);
    }

    return numbers;
}

void PrintNumbers(const vector<int>& numbers) {
    for (auto number : numbers) {
        cout << number << " ";
    }
}

bool isEven(int num){
    return (num % 2 == 0);
}

int main() {
    vector<int> numbers = ReadNumbers();

    sort(numbers.begin(), numbers.end(),
        [] (int n1, int n2) {

        if(isEven(n1) && isEven(n2)){
            return n1 < n2;
        }
        else if (isEven(n1) && !isEven(n2)) {
            return true;
        }
        else if (!isEven(n1) && !isEven(n2)) {
            return n1 > n2;
        }
        else if (!isEven(n1) && isEven(n2)) {
            return false;
        }
        });

    PrintNumbers(numbers);
}
*/


// Урок 3 задание 1
//#include <algorithm>
//#include <iostream>
//#include <vector>
//#include <string>
//#include <set>

//using namespace std;

//string ReadLine() {
//    string s;
//    getline(cin, s);
//    return s;
//}

//int ReadLineWithNumber() {
//    int result;
//    cin >> result;
//    ReadLine();
//    return result;
//}

//set<string> StringIntoWord(const string& str){
//    string word;
//    set <string> words;
//    for(const char& c : str){
//        if(c == ' '){
//            words.insert(word);
//            word = "";
//            continue;
//        }
//        word += c;
//    }
//    words.insert(word);
//    return words;
//}


//int main() {
//    const int queryCount = ReadLineWithNumber();

//    vector<string> queries(queryCount);
//    for (string& query : queries) {
//        query = ReadLine();
//    }
//    const string buzzword = ReadLine();

//    cout << count_if(queries.begin(), queries.end(), [buzzword](const string& query) {
//        // Реализуйте эту лямбда-функцию
//        // Верните true, если query содержит слово buzzword
//        set<string> query_words = StringIntoWord(query);
//        return query_words.count(buzzword);
//    });
//    cout << endl;
//}


#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <algorithm>
using namespace std;

int CountAndAddNewDogs(const vector<string>& new_dogs,
    const map<string, int>& max_amount, map<string, int>& shelter) {
    int new_dogs_count = 0;
    new_dogs_count = count_if(new_dogs.begin(), new_dogs.end(),
        [&max_amount, &shelter] (const string& dog){
        if(shelter[dog] < max_amount.at(dog)){
            ++shelter[dog];
            return true;
        }
        else{
            return false;
        }
    });
    return new_dogs_count;
    //...
}

int main() {
    vector<string> new_dogs = {"landseer", "otterhound", "otterhound", "otterhound", "pointer"};
    map<string, int> max_amount = {{"landseer", 1}, {"otterhound", 0}, {"pekingese", 0}, {"pointer", 1}};
    map<string, int> shelter = {{"landseer", 1}, {"otterhound", 2}, {"pekingese", 2}, {"pointer", 3}};
    cout << CountAndAddNewDogs(new_dogs, max_amount, shelter);
    return 0;
}
