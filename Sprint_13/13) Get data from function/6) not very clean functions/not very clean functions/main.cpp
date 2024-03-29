#include <algorithm>
#include <cassert>
#include <optional>
#include <string_view>
#include <utility>
#include <vector>

using namespace std;

// �������� ������� ComputeStatistics, ����������� 5 ����������:
// ��� ���������, �������� �������� ��� �����, ����� ��������� � ������������� ��������
template <typename InputIt, typename OutSum, typename OutSqSum, typename OutMax>
void ComputeStatistics(InputIt first, InputIt last, OutSum& out_sum, OutSqSum& out_sq_sum, OutMax& out_max) {
    using Elem = std::decay_t<decltype(*first)>;

    constexpr bool need_sum = !is_same_v<OutSum, const nullopt_t>;
    constexpr bool need_sq_sum = !is_same_v<OutSqSum, const nullopt_t>;
    constexpr bool need_max = !is_same_v<OutMax, const nullopt_t>;

    if (first == last) {
        return;
    }

    auto local_sum = Elem{};
    auto local_sqsum = Elem{};
    auto local_max = Elem{};

    if constexpr (need_sum) {
        local_sum = *first;
    }
    if constexpr (need_sq_sum) {
        local_sqsum = *first * *first;
    }
    if constexpr (need_max) {
        local_max = *first;
    }

    while (first < last - 1) {
        ++first;
        if constexpr (need_sum) {
            local_sum += *first;
        }
        if constexpr (need_sq_sum) {
            local_sqsum += *first * *first;
        }
        if constexpr (need_max) {
            local_max = local_max < *first ? *first : local_max;
        }
    }
    if constexpr (need_sum) {
        out_sum = local_sum;
    }
    if constexpr (need_sq_sum) {
        out_sq_sum = local_sqsum;
    }
    if constexpr (need_max) {
        out_max = local_max;
    }
}

struct OnlySum {
    int value;
};

OnlySum operator+(OnlySum l, OnlySum r) {
    return { l.value + r.value };
}
OnlySum& operator+=(OnlySum& l, OnlySum r) {
    return l = l + r;
}

int main() {
    vector input = { 1, 2, 3, 4, 5, 6 };
    int sq_sum;
    std::optional<int> max;

    // �������� �������� ��������� ������ ����� - std::nullopt_t, int � std::optional<int>
    ComputeStatistics(input.begin(), input.end(), nullopt, sq_sum, max);

    assert(sq_sum == 91 && max && *max == 6);

    vector<OnlySum> only_sum_vector = { {100}, {-100}, {20} };
    OnlySum sum;

    // ������ �������� �������������� ������ ������������, �� ��������� ������ �����
    ComputeStatistics(only_sum_vector.begin(), only_sum_vector.end(), sum, nullopt, nullopt);

    assert(sum.value == 20);
}