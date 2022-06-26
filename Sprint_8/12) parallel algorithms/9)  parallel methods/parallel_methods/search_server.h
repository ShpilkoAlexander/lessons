#pragma once

#include "string_processing.h"
#include <stdexcept>
#include <algorithm>
#include <string>
#include "document.h"
#include <vector>
#include <cmath>
#include <tuple>
#include <set>
#include <map>
#include <execution>

const int MAX_RESULT_DOCUMENT_COUNT = 5;
const double EPSILON = 1e-6; // точность сравнения релевантности (double)


class SearchServer {
public:
    template <typename StringContainer>
    explicit SearchServer(const StringContainer& stop_words) ;
    explicit SearchServer(const std::string& stop_words_text) ;
    explicit SearchServer(const std::string_view stop_words_text) ;

    void AddDocument(int document_id, std::string_view document, DocumentStatus status, const std::vector<int>& ratings) ;

    template <typename DocumentPredicate>
    std::vector<Document> FindTopDocuments(std::string_view raw_query, DocumentPredicate document_predicate) const ;
    std::vector<Document> FindTopDocuments(std::string_view raw_query, DocumentStatus status) const ;
    std::vector<Document> FindTopDocuments(std::string_view raw_query) const ;

    int GetDocumentCount() const ;



    std::tuple<std::vector<std::string_view>, DocumentStatus> MatchDocument(std::string_view raw_query, int document_id) const ;
    std::tuple<std::vector<std::string_view>, DocumentStatus> MatchDocument(const std::execution::sequenced_policy& sequen, std::string_view raw_query, int document_id) const ;
    std::tuple<std::vector<std::string_view>, DocumentStatus> MatchDocument(const std::execution::parallel_policy&  paral, std::string_view raw_query, int document_id) const ;

    std::set<int>::iterator begin();

    std::set<int>::iterator end();

    const std::map<std::string, double>& GetWordFrequencies(int document_id) const;

    void RemoveDocument(int document_id);

    template <typename Policy>
    void RemoveDocument(Policy policy, int document_id);

private:

    struct DocumentData {
        int rating;
        DocumentStatus status;
    };


    const std::set<std::string_view> stop_words_;
    std::map<std::string, std::map<int, double>> word_to_document_freqs_;

    std::map<int, DocumentData> documents_;
    std::set<int> document_ids_;
    std::map<int, std::map<std::string, double>> document_ids_with_word_;

    bool IsStopWord(std::string_view word) const ;

    static bool IsValidWord(std::string_view word) ;

    std::vector<std::string_view> SplitIntoWordsNoStop(std::string_view text) const ;

    static int ComputeAverageRating(const std::vector<int>& ratings) ;

    struct QueryWord {
        std::string_view data;
        bool is_minus;
        bool is_stop;
    };

    QueryWord ParseQueryWord(std::string_view text) const ;

//    struct Query {
//        std::set<std::string> plus_words;
//        std::set<std::string> minus_words;
//    };

    struct Query {
        std::vector<std::string_view> plus_words;
        std::vector<std::string_view> minus_words;
    };

    Query ParseQuery(std::string_view text, bool skip_sort = false) const ;

//    QueryVector ParseQueryVector(const std::string& text) const ;

    double ComputeWordInverseDocumentFreq(const std::string_view word) const ;

    template <typename DocumentPredicate>
    std::vector<Document> FindAllDocuments(const Query& query, DocumentPredicate document_predicate) const ;

};

template <typename StringContainer>
SearchServer::SearchServer(const StringContainer& stop_words)
        : stop_words_(MakeUniqueNonEmptyStrings(stop_words)) {
    if (!std::all_of(stop_words_.begin(), stop_words_.end(), IsValidWord)) {
        throw std::invalid_argument("Some of stop words are invalid");
    }
}

template <typename DocumentPredicate>
std::vector<Document> SearchServer::FindTopDocuments(std::string_view raw_query, DocumentPredicate document_predicate) const {
    const auto query = ParseQuery(raw_query);

    auto matched_documents = FindAllDocuments(query, document_predicate);

    std::sort(matched_documents.begin(), matched_documents.end(), [](const Document& lhs, const Document& rhs) {
        if (std::abs(lhs.relevance - rhs.relevance) < EPSILON) {
            return lhs.rating > rhs.rating;
        } else {
            return lhs.relevance > rhs.relevance;
        }
    });
    if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
        matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
    }

    return matched_documents;
}

template <typename DocumentPredicate>
std::vector<Document> SearchServer::FindAllDocuments(const Query& query, DocumentPredicate document_predicate) const {
    std::map<int, double> document_to_relevance;
    for (std::string_view word : query.plus_words) {
        if (word_to_document_freqs_.count(std::string(word)) == 0) {
            continue;
        }
        const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
        for (const auto [document_id, term_freq] : word_to_document_freqs_.at(std::string(word))) {
            const auto& document_data = documents_.at(document_id);
            if (document_predicate(document_id, document_data.status, document_data.rating)) {
                document_to_relevance[document_id] += term_freq * inverse_document_freq;
            }
        }
    }

    for (const std::string_view word : query.minus_words) {
        if (word_to_document_freqs_.count(std::string(word)) == 0) {
            continue;
        }
        for (const auto [document_id, _] : word_to_document_freqs_.at(std::string(word))) {
            document_to_relevance.erase(document_id);
        }
    }

    std::vector<Document> matched_documents;
    for (const auto [document_id, relevance] : document_to_relevance) {
        matched_documents.push_back({document_id, relevance, documents_.at(document_id).rating});
    }
    return matched_documents;
}


template <typename Policy>
void SearchServer::RemoveDocument(Policy policy, const int document_id){
    if (!document_ids_.count(document_id)) {
             return;
    }

    std::vector<const std::string*> words_to_delete(document_ids_with_word_.at(document_id).size());
    std::transform(policy,
                   document_ids_with_word_.at(document_id).cbegin(), document_ids_with_word_.at(document_id).cend(),
                   words_to_delete.begin(),
                    [] (const std::pair<const std::string, double>& words_freq) {
                     return &words_freq.first;
                    }
    );

    std::for_each(policy,
                  words_to_delete.cbegin(), words_to_delete.cend(),
                  [this, document_id] (const auto& word) {
         word_to_document_freqs_.at(*word).erase(document_id);
    });

    document_ids_.erase(document_id);
    document_ids_with_word_.erase(document_id);
    documents_.erase(document_id);
}
