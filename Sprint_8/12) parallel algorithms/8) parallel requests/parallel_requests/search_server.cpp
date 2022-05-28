#include "search_server.h"
#include <string>
#include "string_processing.h"
#include "document.h"
#include <vector>
#include <stdexcept>
#include <tuple>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <set>


SearchServer::SearchServer(const std::string& stop_words_text)
    : SearchServer(SplitIntoWords(stop_words_text)) {
}

void SearchServer::AddDocument(int document_id, const std::string& document, DocumentStatus status, const std::vector<int>& ratings) {
    if ((document_id < 0) || (documents_.count(document_id) > 0)) {
        throw std::invalid_argument("Invalid document_id");
    }
    const auto words = SearchServer::SplitIntoWordsNoStop(document);

    const double inv_word_count = 1.0 / words.size();
    for (const std::string& word : words) {
        SearchServer::word_to_document_freqs_[word][document_id] += inv_word_count;
        SearchServer::document_ids_with_word_[document_id][word] += inv_word_count;
    }
    SearchServer::documents_.emplace(document_id, SearchServer::DocumentData{ComputeAverageRating(ratings), status});
    SearchServer::document_ids_.insert(document_id);
}

std::vector<Document> SearchServer::FindTopDocuments(const std::string& raw_query, DocumentStatus status) const {
    return SearchServer::FindTopDocuments(raw_query, [status](int document_id, DocumentStatus document_status, int rating) {
        return document_status == status;
    });
}

std::vector<Document> SearchServer::FindTopDocuments(const std::string& raw_query) const {
    return SearchServer::FindTopDocuments(raw_query, DocumentStatus::ACTUAL);
}

int SearchServer::GetDocumentCount() const {
    return SearchServer::documents_.size();
}

//int SearchServer::GetDocumentId(int index) const {
//    return SearchServer::document_ids_.at(index);
//}

std::tuple<std::vector<std::string>, DocumentStatus> SearchServer::MatchDocument(const std::string& raw_query, int document_id) const {
    const auto query = SearchServer::ParseQuery(raw_query);

    std::vector<std::string> matched_words;
    for (const std::string& word : query.plus_words) {
        if (SearchServer::word_to_document_freqs_.count(word) == 0) {
            continue;
        }
        if (SearchServer::word_to_document_freqs_.at(word).count(document_id)) {
            matched_words.push_back(word);
        }
    }
    for (const std::string& word : query.minus_words) {
        if (SearchServer::word_to_document_freqs_.count(word) == 0) {
            continue;
        }
        if (SearchServer::word_to_document_freqs_.at(word).count(document_id)) {
            matched_words.clear();
            break;
        }
    }
    return {matched_words, SearchServer::documents_.at(document_id).status};
}

bool SearchServer::IsStopWord(const std::string& word) const {
    return SearchServer::stop_words_.count(word) > 0;
}

bool SearchServer::IsValidWord(const std::string& word) {
    return std::none_of(word.begin(), word.end(), [](char c) {
        return c >= '\0' && c < ' ';
    });
}

std::vector<std::string> SearchServer::SplitIntoWordsNoStop(const std::string& text) const {
    std::vector<std::string> words;
    for (const std::string& word : SplitIntoWords(text)) {
        if (!SearchServer::IsValidWord(word)) {
            throw std::invalid_argument("Word " + word + " is invalid");
        }
        if (!SearchServer::IsStopWord(word)) {
            words.push_back(word);
        }
    }
    return words;
}

int SearchServer::ComputeAverageRating(const std::vector<int>& ratings) {
    if (ratings.empty()) {
        return 0;
    }
    int rating_sum = accumulate(ratings.begin(), ratings.end(), 0);

    return rating_sum / static_cast<int>(ratings.size());
}

SearchServer::QueryWord SearchServer::ParseQueryWord(const std::string& text) const {
    if (text.empty()) {
        throw std::invalid_argument("Query word is empty");
    }
    std::string word = text;
    bool is_minus = false;
    if (word[0] == '-') {
        is_minus = true;
        word = word.substr(1);
    }
    if (word.empty() || word[0] == '-' || !SearchServer::IsValidWord(word)) {
        throw std::invalid_argument("Query word " + text + " is invalid");
    }

    return {word, is_minus, SearchServer::IsStopWord(word)};
}


SearchServer::Query SearchServer::ParseQuery(const std::string& text) const {
    SearchServer::Query result;
    for (const std::string& word : SplitIntoWords(text)) {
        const auto query_word = SearchServer::ParseQueryWord(word);
        if (!query_word.is_stop) {
            if (query_word.is_minus) {
                result.minus_words.insert(query_word.data);
            } else {
                result.plus_words.insert(query_word.data);
            }
        }
    }
    return result;
}


double SearchServer::ComputeWordInverseDocumentFreq(const std::string& word) const {
    return std::log(SearchServer::GetDocumentCount() * 1.0 / SearchServer::word_to_document_freqs_.at(word).size());
}


std::set<int>::iterator SearchServer::begin() {
    return document_ids_.begin();
}


std::set<int>::iterator SearchServer::end() {
    return document_ids_.end();
}

const std::map<std::string, double>& SearchServer::GetWordFrequencies(int document_id) const {
    static std::map<std::string, double> word_frequencies_;    

    auto found_document = document_ids_with_word_.find(document_id);

    if (found_document == document_ids_with_word_.end()) { //invalid id
        return word_frequencies_;
    }
    else {
        word_frequencies_ = found_document->second;
        return word_frequencies_;
    }
}

void SearchServer::RemoveDocument(int document_id) {
    for (const auto& [word, freq] : document_ids_with_word_.at(document_id)) {
        word_to_document_freqs_.at(word).erase(document_id);
    }
    document_ids_.erase(document_id);
    document_ids_with_word_.erase(document_id);
    documents_.erase(document_id);
}


