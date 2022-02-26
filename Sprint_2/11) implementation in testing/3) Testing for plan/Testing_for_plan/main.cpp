#include <iostream>
#include <string>

using namespace std;


string DeleteSpace(string check_word){
    string no_space_word;
    for(char ch : check_word){
        if(ch != ' '){
            no_space_word += ch;
        }
    }
    return no_space_word;
}
// Определяет, будет ли слово палиндромом
// text может быть строкой, содержащей строчные символы английского алфавита и пробелы
// Пустые строки и строки, состоящие только из пробелов, — это не палиндромы
bool IsPalindrome(const string& text) {
    // Напишите недостающий код
    string text_without_space = DeleteSpace(text);
    if (text_without_space.empty()){
        return false;
    }
    unsigned long char_count = text_without_space.size();
    for(unsigned long i = 0; i < (char_count / 2); i++){
        if(text_without_space[i] != text_without_space[char_count - i - 1]){
            return false;
        }
    }

    return true;
}

int main() {
    string text;
    getline(cin, text);

    if (IsPalindrome(text)) {
        cout << "palindrome"s << endl;
    } else {
        cout << "not a palindrome"s << endl;
    }
}
