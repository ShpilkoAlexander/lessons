#include "budget_manager.h"


void BudgetManager::ComputeIncome(const QueryInfo& query_info) { // вычисляет чистую прибыль за данный диапазон дат и выводит результат в cout
    double income = 0.0;
    size_t index = Date::ComputeDistance(START_DATE, query_info.start_date);
    int count_days = Date::ComputeDistance( query_info.start_date, query_info.end_date);

    for(;count_days >= 0; --count_days) {
        income += days_.at(index).income - days_.at(index).spendind;
        ++index;
    }

    std::cout << income << std::endl;
}
void BudgetManager::Earn(const QueryInfo& query_info) { //учесть, что в этот период равномерно по дням была заработана указанная сумма. Прибыль — произвольное положительное число double.
    size_t index = Date::ComputeDistance(START_DATE, query_info.start_date);
    int count_days = Date::ComputeDistance( query_info.start_date, query_info.end_date) + 1;
    double add = query_info.num / count_days;
    for(;count_days > 0; --count_days) {
        days_.at(index++).income += add;
    }
}

void BudgetManager::PayTax(const QueryInfo& query_info) { // — заплатить налог 13% в каждый день указанного диапазона. Это означает простое умножение всей прибыли в диапазоне на 0,87, независимо от того, отдавался ли уже налог за какой-либо из указанных дней. Прибыль за эти дни, которая обнаружится позже, налогами из прошлого не облагается.
    size_t index = Date::ComputeDistance(START_DATE, query_info.start_date);
    int count_days = Date::ComputeDistance(query_info.start_date, query_info.end_date);
    double percent = 1 - (query_info.num / 100);
    for(;count_days >= 0; --count_days) {
        days_.at(index++).income *= percent;
    }
}

void BudgetManager::Spend(const QueryInfo& query_info) { //  учесть, что в этот период равномерно по дням была потрачена указанная сумма.
    size_t index = Date::ComputeDistance(START_DATE, query_info.start_date);
    int count_days = Date::ComputeDistance( query_info.start_date, query_info.end_date) + 1;
    double spend = query_info.num / count_days;
    for(;count_days > 0; --count_days) {
        days_.at(index++).spendind += spend;
    }


}

