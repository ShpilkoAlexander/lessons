#pragma once

struct MoneyStruct {
    double earnings;
    double spendings;

    MoneyStruct& operator+=(const MoneyStruct& other) {
        earnings += other.earnings;
        spendings += other.spendings;

        return *this;
    }

    MoneyStruct operator+(const MoneyStruct& other) const {
        return MoneyStruct(*this) += other;
    }

    MoneyStruct operator*(double factor) const {
        return {earnings * factor,
                spendings * factor};
    }

    operator double() const {
        return earnings - spendings;
    }
};