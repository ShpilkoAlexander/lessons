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


// Урок 4 задание 1
//#include <vector>
//#include <string>
//#include <map>
//#include <iostream>
//#include <algorithm>
//using namespace std;

//int CountAndAddNewDogs(const vector<string>& new_dogs,
//    const map<string, int>& max_amount, map<string, int>& shelter) {
//    int new_dogs_count = 0;
//    new_dogs_count = count_if(new_dogs.begin(), new_dogs.end(),
//        [&max_amount, &shelter] (const string& dog){
//        if(shelter[dog] < max_amount.at(dog)){
//            ++shelter[dog];
//            return true;
//        }
//        else{
//            return false;
//        }
//    });
//    return new_dogs_count;
//    //...
//}

//int main() {
//    vector<string> new_dogs = {"landseer", "otterhound", "otterhound", "otterhound", "pointer"};
//    map<string, int> max_amount = {{"landseer", 1}, {"otterhound", 0}, {"pekingese", 0}, {"pointer", 1}};
//    map<string, int> shelter = {{"landseer", 1}, {"otterhound", 2}, {"pekingese", 2}, {"pointer", 3}};
//    cout << CountAndAddNewDogs(new_dogs, max_amount, shelter);
//    return 0;
//}


// Урок 6 задание 1

//#include <algorithm>
//#include <iostream>
//#include <set>
//#include <string>
//#include <utility>
//#include <vector>

//using namespace std;

//const int MAX_RESULT_DOCUMENT_COUNT = 5;

//string ReadLine() {
//    string s;
//    getline(cin, s);
//    return s;
//}

//int ReadLineWithNumber() {
//    int result = 0;
//    cin >> result;
//    ReadLine();
//    return result;
//}

//vector<string> SplitIntoWords(const string& text) {
//    vector<string> words;
//    string word;
//    for (const char c : text) {
//        if (c == ' ') {
//            if (!word.empty()) {
//                words.push_back(word);
//                word.clear();
//            }
//        } else {
//            word += c;
//        }
//    }
//    if (!word.empty()) {
//        words.push_back(word);
//    }

//    return words;
//}

//struct Document {
//    int id;
//    int relevance;
//};

//class SearchServer {
//public:
//    void SetStopWords(const string& text) {
//        for (const string& word : SplitIntoWords(text)) {
//            stop_words_.insert(word);
//        }
//    }

//    void AddDocument(int document_id, const string& document) {
//        const vector<string> words = SplitIntoWordsNoStop(document);
//        documents_.push_back({document_id, words});
//    }

//    vector<Document> FindTopDocuments(const string& raw_query) const {
//        const Query query_words = ParseQuery(raw_query);
//        auto matched_documents = FindAllDocuments(query_words);

//        sort(matched_documents.begin(), matched_documents.end(),
//             [](const Document& lhs, const Document& rhs) {
//                 return lhs.relevance > rhs.relevance;
//             });
//        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
//            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
//        }
//        return matched_documents;
//    }

//private:
//    struct DocumentContent {
//        int id = 0;
//        vector<string> words;
//    };

//    vector<DocumentContent> documents_;

//    set<string> stop_words_;

//    bool IsStopWord(const string& word) const {
//        return stop_words_.count(word) > 0;
//    }

//    vector<string> SplitIntoWordsNoStop(const string& text) const {
//        vector<string> words;
//        string check_word;
//        for (const string& word : SplitIntoWords(text)) {
//            check_word = word;
//            if (check_word[0] == '-'){
//            check_word = check_word.substr(1);
//            }
//            if (!IsStopWord(check_word)) {
//                words.push_back(word);
//            }
//        }
//        return words;
//    }

//    struct Query{
//        set<string> plus_word;
//        set<string> minus_word;
//    };

//    Query ParseQuery(const string& text) const {
//        Query query_words;
//        for (const string& word : SplitIntoWordsNoStop(text)) {
//            if (word[0] == '-'){
//                query_words.minus_word.insert(word.substr(1));
//            }
//            else {
//                query_words.plus_word.insert(word);
//            }
//        }
//        return query_words;
//    }

//    vector<Document> FindAllDocuments(const Query& query_words) const {
//        vector<Document> matched_documents;
//        for (const auto& document : documents_) {
//            const int relevance = MatchDocument(document, query_words);
//            if (relevance > 0) {
//                matched_documents.push_back({document.id, relevance});
//            }
//        }
//        return matched_documents;
//    }

//    static int MatchDocument(const DocumentContent& content, const Query& query_words) {
//        if (query_words.plus_word.empty()) {
//            return 0;
//        }
//        set<string> matched_words;
//        for (const string& word : content.words) {
//            if (query_words.minus_word.count(word)){
//                return 0;
//            }
//            if (matched_words.count(word) != 0) {
//                continue;
//            }
//            if (query_words.plus_word.count(word) != 0) {
//                matched_words.insert(word);
//            }
//        }
//        return static_cast<int>(matched_words.size());
//    }
//};

//SearchServer CreateSearchServer() {
//    SearchServer search_server;
//    search_server.SetStopWords(ReadLine());

//    const int document_count = ReadLineWithNumber();
//    for (int document_id = 0; document_id < document_count; ++document_id) {
//        search_server.AddDocument(document_id, ReadLine());
//    }

//    return search_server;
//}

//int main() {
//    const SearchServer search_server = CreateSearchServer();

//    const string query = ReadLine();
//    for (const auto& [document_id, relevance] : search_server.FindTopDocuments(query)) {
//        cout << "{ document_id = "s << document_id << ", "
//             << "relevance = "s << relevance << " }"s << endl;
//    }
//}


#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <map>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result = 0;
    cin >> result;
    ReadLine();
    return result;
}

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}

struct Document {
    int id;
    int relevance;
};

class SearchServer {
public:
    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }

    void AddDocument(int document_id, const string& document) {
        const vector<string> words = SplitIntoWordsNoStop(document);
        for(const string& word : words){
            word_to_documents_[word].insert(document_id);
        }
    }

    vector<Document> FindTopDocuments(const string& raw_query) const {
        const Query query_words = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query_words);

        sort(matched_documents.begin(), matched_documents.end(),
             [](const Document& lhs, const Document& rhs) {
                 return lhs.relevance > rhs.relevance;
             });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }

private:

    map<string, set<int>>word_to_documents_;

    set<string> stop_words_;

    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }

    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        string check_word;
        for (const string& word : SplitIntoWords(text)) {
            check_word = word;
            if (check_word[0] == '-'){
            check_word = check_word.substr(1);
            }
            if (!IsStopWord(check_word)) {
                words.push_back(word);
            }
        }
        return words;
    }

    struct Query{
        set<string> plus_word;
        set<string> minus_word;
    };

    Query ParseQuery(const string& text) const {
        Query query_words;
        for (const string& word : SplitIntoWordsNoStop(text)) {
            if (word[0] == '-'){
                query_words.minus_word.insert(word.substr(1));
            }
            else {
                query_words.plus_word.insert(word);
            }
        }
        return query_words;
    }

    set<int> DocumentsIdWithWord(const string&  word) const{
        return word_to_documents_.at(word);
    }

    vector<Document> FindAllDocuments(const Query& query_words) const {
        vector<Document> matched_documents;
        map<int, int> document_to_relevance;

        // adding documents by plus words
        for(const string& p_word : query_words.plus_word){
            if (word_to_documents_.find(p_word) != word_to_documents_.end()){
                for(const int& document_id : DocumentsIdWithWord(p_word)){
                    ++document_to_relevance[document_id];
                }
            }
        }

        // deleting documents by minus words
        for(const string& m_word : query_words.minus_word){
            if (word_to_documents_.find(m_word) != word_to_documents_.end()){
                for(const int& document_id : DocumentsIdWithWord(m_word)){
                    document_to_relevance.erase(document_id);
                }
            }
        }


        for(const auto& [id_document, relevance]: document_to_relevance ){
            matched_documents.push_back({id_document, relevance});
        }


        return matched_documents;
    }

//    static int MatchDocument(const DocumentContent& content, const Query& query_words) {
//        if (query_words.plus_word.empty()) {
//            return 0;
//        }
//        set<string> matched_words;
//        for (const string& word : content.words) {
//            if (query_words.minus_word.count(word)){
//                return 0;
//            }
//            if (matched_words.count(word) != 0) {
//                continue;
//            }
//            if (query_words.plus_word.count(word) != 0) {
//                matched_words.insert(word);
//            }
//        }
//        return static_cast<int>(matched_words.size());
//    }
};

SearchServer CreateSearchServer() {
    SearchServer search_server;
    search_server.SetStopWords(ReadLine());

    const int document_count = ReadLineWithNumber();
    for (int document_id = 0; document_id < document_count; ++document_id) {
        search_server.AddDocument(document_id, ReadLine());
    }

    return search_server;
}

int main() {
    const SearchServer search_server = CreateSearchServer();

    const string query = ReadLine();
    for (const auto& [document_id, relevance] : search_server.FindTopDocuments(query)) {
        cout << "{ document_id = "s << document_id << ", "
             << "relevance = "s << relevance << " }"s << endl;
    }
}


