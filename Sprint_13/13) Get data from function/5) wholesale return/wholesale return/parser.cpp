#include "parser.h"

using namespace std;

namespace queries {
    class ComputeIncome : public ComputeQuery {
    public:
        using ComputeQuery::ComputeQuery;
        ReadResult Process(const BudgetManager& budget) const override {
            return {budget.ComputeSum(GetFrom(), GetTo())};
        }

        class Factory : public QueryFactory {
        public:
            std::unique_ptr<Query> Construct(std::string_view config) const override {
                auto parts = Split(config, ' ');
                return std::make_unique<ComputeIncome>(Date::FromString(parts[0]), Date::FromString(parts[1]));
            }
        };
    };

    template <int HELPER>
    class Alter : public ModifyQuery {
    public:

        Alter(Date from, Date to, double amount)
                : ModifyQuery(from, to)
                , amount_(amount) {
        }

        void Process(BudgetManager& budget) const override {
            double day_income = amount_ / (Date::ComputeDistance(GetFrom(), GetTo()) + 1);
            MoneyStruct daily_change;

            if (HELPER == 1) {
                daily_change = MoneyStruct{day_income, {}};
            } else {
                daily_change = MoneyStruct{{}, day_income};
            }

            budget.AddBulkOperation(GetFrom(), GetTo(), BulkMoneyAdder{daily_change});
        }

        class Factory : public QueryFactory {
        public:
            std::unique_ptr<Query> Construct(std::string_view config) const override {
                auto parts = Split(config, ' ');
                double payload = std::stod(std::string(parts[2]));
                if (HELPER == 1) {
                    return std::make_unique<Alter<1>>(Date::FromString(parts[0]), Date::FromString(parts[1]), payload);
                } else {
                    return std::make_unique<Alter<-1>>(Date::FromString(parts[0]), Date::FromString(parts[1]), payload);
                }
            }
        };

    private:
        double amount_;
    };

    class PayTax : public ModifyQuery {
    public:
        PayTax(Date from, Date to, double amount)
                : ModifyQuery(from, to)
                , amount_(amount) {
        }

        void Process(BudgetManager& budget) const override {
            budget.AddBulkOperation(GetFrom(), GetTo(), BulkTaxApplier{amount_});
        }

        class Factory : public QueryFactory {
        public:
            std::unique_ptr<Query> Construct(std::string_view config) const override {
                auto parts = Split(config, ' ');
                double payload = std::stod(std::string(parts[2]));
                return std::make_unique<PayTax>(Date::FromString(parts[0]), Date::FromString(parts[1]), payload);
            }
        };

    private:
        int amount_ = 0;
    };

}//end namespace queries

const QueryFactory& QueryFactory::GetFactory(std::string_view id) {
    static queries::ComputeIncome::Factory compute_income;
    static queries::Alter<1>::Factory earn;
    static queries::Alter<-1>::Factory spend;
    static queries::PayTax::Factory pay_tax;
    static std::unordered_map<std::string_view, const QueryFactory&> factories
            = {{"ComputeIncome"sv, compute_income}, {"Earn"sv, earn},  {"Spend"sv, spend}, {"PayTax"sv, pay_tax}};

    return factories.at(id);
}