#pragma once
#include "date.h"

#include <vector>
#include <iostream>

struct DayInfo {
    double income = 0.0;
    double spendind = 0.0;
};

struct QueryInfo {
    Date start_date;
    Date end_date;
    double num = 0.0;
};

class BudgetManager {
public:
    inline static const Date START_DATE{2000, 1, 1};
    inline static const Date END_DATE{2100, 1, 1};

    BudgetManager()
        : days_(std::vector<DayInfo>(Date::ComputeDistance(START_DATE, END_DATE))) {
    }

    void ComputeIncome(const QueryInfo& query_info); // вычисляет чистую прибыль за данный диапазон дат и выводит результат в cout

    void Earn(const QueryInfo& query_info); //учесть, что в этот период равномерно по дням была заработана указанная сумма. Прибыль — произвольное положительное число double.

    void PayTax(const QueryInfo& query_info); // — заплатить налог 13% в каждый день указанного диапазона. Это означает простое умножение всей прибыли в диапазоне на 0,87, независимо от того, отдавался ли уже налог за какой-либо из указанных дней. Прибыль за эти дни, которая обнаружится позже, налогами из прошлого не облагается.

    void Spend(const QueryInfo& query_info); //  учесть, что в этот период равномерно по дням была потрачена указанная сумма.
private:
    std::vector<DayInfo> days_;
};
