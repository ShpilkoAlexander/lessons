#pragma once
#include "date.h"
#include "budget_manager.h"

#include <string_view>
#include <sstream>

enum QUERY {
    COMPUTEINCOME,
    EARN,
    PAYTAX,
    SPEND,
    ERROR
};

struct ParseInfo {
    QUERY query;
    QueryInfo query_info;
};

std::vector<std::string> ParseOnWord(std::string& s) {
    std::string delimiter = " ";

    size_t pos = 0;
    std::string token;
    std::vector<std::string> result;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        result.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    result.emplace_back(s);
    return result;
}

ParseInfo Parse(std::string& line) {

    ParseInfo parse_info;
    std::vector<std::string> parse = ParseOnWord(line);
    if (parse.at(0) == "Earn") {
        parse_info.query = QUERY::EARN;
    } else if (parse.at(0) == "ComputeIncome") {
        parse_info.query = QUERY::COMPUTEINCOME;
    } else if (parse.at(0) == "PayTax") {
        parse_info.query = QUERY::PAYTAX;
    } else if (parse.at(0) == "Spend") {
        parse_info.query = QUERY::SPEND;
    } else {
        parse_info.query = QUERY::ERROR;
    }



    parse_info.query_info.start_date = Date::FromString(parse.at(1));
    parse_info.query_info.end_date = Date::FromString(parse.at(2));
    if (parse_info.query != QUERY::COMPUTEINCOME) {
        parse_info.query_info.num = std::stod(parse.at(3));
    }

    return parse_info;
}

void ProccessQuery(BudgetManager& manager, ParseInfo& parse_info) {

    switch (parse_info.query) {
    case QUERY::COMPUTEINCOME:
        manager.ComputeIncome(parse_info.query_info);
        break;
    case QUERY::EARN:
        manager.Earn(parse_info.query_info);
        break;
    case QUERY::PAYTAX:
        manager.PayTax(parse_info.query_info);
        break;
    case QUERY::SPEND:
        manager.Spend(parse_info.query_info);
        break;
    case QUERY::ERROR:
        break;
    }
}


//class Query {
//    virtual void Procces() = 0;
//};

//class QueryRead : public Query {
//public:
//    QueryRead(const BudgetManager& manager, const QueryInfo& query_info)
//        : manager_(manager), query_info_(query_info) {
//    }

//    void Procces() override;

//private:
//    const BudgetManager& manager_;
//    const QueryInfo& query_info_;
//};

//class QueryModificate : public Query {
//public:
//    QueryModificate(BudgetManager& manager, const QueryInfo& query_info)
//        : manager_(manager), query_info_(query_info) {
//    }

//private:
//    BudgetManager& manager_;
//    const QueryInfo& query_info_;
//};

//class QueryComputeIncome : public QueryRead {
//public:
//    QueryComputeIncome(const BudgetManager& manager, const QueryInfo& query_info)
//        : QueryRead(manager, query_info) {
//    }

//    void Procces() override;
//};

//class QueryEarn : public QueryModificate {
//public:
//    QueryEarn(BudgetManager& manager, const QueryInfo& query_info)
//        : QueryModificate(manager, query_info) {
//    }

//    void Procces() override;

//};

//class QueryPayTax : public QueryModificate {
//public:
//    QueryPayTax(BudgetManager& manager, const QueryInfo& query_info)
//        : QueryModificate(manager, query_info) {
//    }

//    void Procces() override;

//};



