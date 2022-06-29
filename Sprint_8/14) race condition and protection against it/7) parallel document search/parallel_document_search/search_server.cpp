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
#include <execution>

SearchServer::SearchServer(const std::string& stop_words_text)
    : SearchServer(SplitIntoWords(stop_words_text)) {
}

SearchServer::SearchServer(const std::string_view stop_words_text)
    : SearchServer(SplitIntoWords(stop_words_text)) {
}

void SearchServer::AddDocument(int document_id, std::string_view document, DocumentStatus status, const std::vector<int>& ratings) {
    if ((document_id < 0) || (documents_.count(document_id) > 0)) {
        throw std::invalid_argument("Invalid document_id");
    }
    const auto words = SplitIntoWordsNoStop(document);

    const double inv_word_count = 1.0 / words.size();
    for (const std::string_view word : words) {
        auto it = words_.insert(std::string(word));
        std::string_view view_word = *(it.first);

        word_to_document_freqs_[view_word][document_id] += inv_word_count;
        document_ids_with_word_[document_id][view_word] += inv_word_count;
    }
    documents_.emplace(document_id, DocumentData{ComputeAverageRating(ratings), status});
    document_ids_.insert(document_id);
}

std::vector<Document>
SearchServer::FindTopDocuments(const std::string_view raw_query, DocumentStatus status) const {
    return FindTopDocuments(raw_query,
           [status](int document_id, DocumentStatus document_status,int rating) {
                        return document_status == status;
                    });
}

std::vector<Document>
SearchServer::FindTopDocuments(
              const std::string_view raw_query) const {
    return FindTopDocuments(raw_query, DocumentStatus::ACTUAL);
}

int SearchServer::GetDocumentCount() const {
    return SearchServer::documents_.size();
}

std::tuple<std::vector<std::string_view>, DocumentStatus> SearchServer::MatchDocument(std::string_view raw_query, int document_id) const {
    return MatchDocument(std::execution::seq, raw_query, document_id);
}


std::tuple<std::vector<std::string_view>, DocumentStatus> SearchServer::MatchDocument(const std::execution::sequenced_policy&, std::string_view raw_query, int document_id) const {
    const auto query = ParseQuery(std::execution::seq, raw_query);

    const auto status = documents_.at(document_id).status;

    for (const std::string_view word : query.minus_words) {
        if (word_to_document_freqs_.count(std::string(word)) == 0) {
            continue;
        }
        if (word_to_document_freqs_.at(std::string(word)).count(document_id)) {
            return {{}, status};
        }
    }

    std::vector<std::string_view> matched_words;
    for (const std::string_view word : query.plus_words) {
        if (word_to_document_freqs_.count(std::string(word)) == 0) {
            continue;
        }
        if (word_to_document_freqs_.at(std::string(word)).count(document_id)) {
            matched_words.push_back(word);
        }
    }
    return {matched_words, status};
}

std::tuple<std::vector<std::string_view>, DocumentStatus> SearchServer::MatchDocument(const std::execution::parallel_policy&, std::string_view raw_query, int document_id) const {
    const auto query = ParseQuery(std::execution::par, raw_query, true);

    const auto status = documents_.at(document_id).status;

    const auto word_checker =
        [this, document_id](const std::string_view word) {
            const auto it = word_to_document_freqs_.find(std::string(word));
            return it != word_to_document_freqs_.end() && it->second.count(document_id);
        };
    if (std::any_of(std::execution::par, query.minus_words.begin(), query.minus_words.end(), word_checker)) {
        return {{}, status};
    }

    std::vector<std::string_view> matched_words(query.plus_words.size());
    auto words_end = std::copy_if(
        std::execution::par,
        query.plus_words.begin(), query.plus_words.end(),
        matched_words.begin(),
        word_checker
    );
    std::sort(std::execution::par, matched_words.begin(), words_end);
    words_end = std::unique(matched_words.begin(), words_end);
    matched_words.erase(words_end, matched_words.end());

    return {matched_words, status};


}

bool SearchServer::IsStopWord(std::string_view word) const {
    return SearchServer::stop_words_.count(word) > 0;
}

bool SearchServer::IsValidWord(std::string_view word) {
    return std::none_of(word.begin(), word.end(), [](char c) {
        return c >= '\0' && c < ' ';
    });
}

std::vector<std::string_view> SearchServer::SplitIntoWordsNoStop(std::string_view text) const {
    std::vector<std::string_view> words;
    for (const std::string_view word : SplitIntoWords(text)) {
        if (!SearchServer::IsValidWord(word)) {
            throw std::invalid_argument("Word " + std::string(word) + " is invalid");
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

SearchServer::QueryWord SearchServer::ParseQueryWord(std::string_view word) const {
    if (word.empty()) {
        throw std::invalid_argument("Query word is empty");
    }
    bool is_minus = false;
    if (word[0] == '-') {
        is_minus = true;
        word.remove_prefix(1);
    }
    if (word.empty() || word[0] == '-' || !SearchServer::IsValidWord(word)) {
        throw std::invalid_argument("Query word " + std::string(word) + " is invalid");
    }

    return {word, is_minus, SearchServer::IsStopWord(word)};
}



//SearchServer::Query SearchServer::ParseQuery(std::string_view text, bool skip_sort) const {
//    Query result;
//    for (std::string_view word : SplitIntoWords(text)) {
//        const auto query_word = ParseQueryWord(word);
//        if (!query_word.is_stop) {
//            if (query_word.is_minus) {
//                result.minus_words.push_back(query_word.data);
//            } else {
//                result.plus_words.push_back(query_word.data);
//            }
//        }
//    }
//    if (!skip_sort) {
//        for (auto* words : {&result.plus_words, &result.minus_words}) {
//            std::sort(words->begin(), words->end());
//            words->erase(unique(words->begin(), words->end()), words->end());
//        }
//    }
//    return result;
//}




double SearchServer::ComputeWordInverseDocumentFreq(std::string_view word) const {
    return std::log(SearchServer::GetDocumentCount() * 1.0 / SearchServer::word_to_document_freqs_.at(std::string(word)).size());
}


std::set<int>::iterator SearchServer::begin() {
    return document_ids_.begin();
}


std::set<int>::iterator SearchServer::end() {
    return document_ids_.end();
}

const std::map<std::string_view, double>& SearchServer::GetWordFrequencies(int document_id) const {
    static std::map<std::string_view, double> result;
    if (documents_.count(document_id) > 0) {
        result = document_ids_with_word_.at(document_id);
    } else {
        result.clear();
    }
    return result;
}

void SearchServer::RemoveDocument(int document_id) {
    if (!document_ids_.count(document_id)) {
             return;
    }

    for (const auto& [word, freq] : document_ids_with_word_.at(document_id)) {
        word_to_document_freqs_.at(word).erase(document_id);
    }
    document_ids_.erase(document_id);
    document_ids_with_word_.erase(document_id);
    documents_.erase(document_id);
}




