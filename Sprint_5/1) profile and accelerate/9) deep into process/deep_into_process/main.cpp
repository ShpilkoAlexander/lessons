#include "log_duration.h"

#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>

using namespace std;



vector<float> ComputeAvgTemp(const vector<vector<float>>& measures) {
    if (measures.empty()) {
        return {};
    }

    size_t measures_count = measures[0].size();
    vector<float> avg_temp(measures_count);
    vector<int> positive_temp_count(measures_count);

    for (const auto& measure : measures) {
        for(size_t i = 0; i < measures_count; ++i) {
            avg_temp[i] += (measure[i] > 0.f ? measure[i] : 0.f);
            positive_temp_count[i] += (measure[i] > 0 ? 1 : 0);
        }
    }

    for(size_t i = 0; i < measures_count; ++i) {
        avg_temp[i] = positive_temp_count[i] > 0 ? (avg_temp[i] / positive_temp_count[i]) : 0;
    }

    return avg_temp;
}

vector<float> GetRandomVector(size_t size) {
    static mt19937 engine;
    uniform_real_distribution<float> d(-100, 100);

    vector<float> res(size);
    for (size_t i = 0; i < size; ++i) {
        res[i] = d(engine);
    }

    return res;
}

int main() {
    vector<vector<float>> data;
    data.reserve(5000);

    for (int i = 0; i < 5000; ++i) {
        data.push_back(GetRandomVector(5000));
    }

    vector<float> avg;
    {
        LOG_DURATION("ComputeAvgTemp"s);
        avg = ComputeAvgTemp(data);
    }

    cout << "Total mean: "s << accumulate(avg.begin(), avg.end(), 0.f) / avg.size() << endl;
}
