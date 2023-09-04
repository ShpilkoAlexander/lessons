#pragma once

#include "entities.h"
#include "summing_segment_tree.h"

#include <cstdint>
#include <cmath>

struct BulkTaxApplier {
    double factor = 1.0;
    BulkTaxApplier(int _tax = 0) : factor(1.0 - double(_tax) / 100.0) {}
};

struct BulkMoneyAdder {
    MoneyStruct delta = {};
};

class BulkLinearUpdater {
public:
    BulkLinearUpdater() = default;
    BulkLinearUpdater(const BulkMoneyAdder& add) : add_(add) {}
    BulkLinearUpdater(const BulkTaxApplier& tax) : tax_(tax) {}

    void CombineWith(const BulkLinearUpdater& other) {
        tax_.factor *= other.tax_.factor;
        add_.delta.earnings = add_.delta.earnings * other.tax_.factor + other.add_.delta.earnings;
        add_.delta.spendings += other.add_.delta.spendings;
    }

    MoneyStruct Collapse(const MoneyStruct& origin, IndexSegment segment) const {
        return MoneyStruct{origin.earnings * tax_.factor, origin.spendings}
               + add_.delta * double(segment.length());
    }

private:
    BulkTaxApplier tax_;
    BulkMoneyAdder add_;
};