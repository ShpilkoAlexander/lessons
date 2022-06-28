#include <algorithm>
#include <cstdlib>
#include <future>
#include <map>
#include <numeric>
#include <random>
#include <string>
#include <vector>
#include <mutex>

#include "log_duration.h"
#include "test_framework.h"

using namespace std::string_literals;

template <typename Key, typename Value>
class ConcurrentMap {
public:
    using MapType = std::map<Key, Value>;
    static_assert(std::is_integral_v<Key>, "ConcurrentMap supports only integer keys"s);

    struct Access {

        ~Access() {
            m_.unlock();
        }
        Value& ref_to_value;
        std::mutex& m_;
        //std::lock_guard<std::mutex> guard;
    };

    explicit ConcurrentMap(size_t bucket_count)
                : buckets_(bucket_count),
                  value_mutexes_(bucket_count),
                  bucket_mutexes_(bucket_count)
    {
    }

    Access operator[](const Key& key) {
        size_t index = GetBucketIndex(static_cast<uint64_t>(key));
        //std::lock_guard<std::mutex> guard(bucket_mutexes_[index]);
        bucket_mutexes_[index].lock();
        return {buckets_[index][key], bucket_mutexes_[index]};
    }

    MapType BuildOrdinaryMap() {
        MapType result;
        for (auto& i : bucket_mutexes_) {
            i.lock();
        }
        for(auto bucket_ = buckets_.begin(), mutex_ = bucket_mutexes_.begin(); bucket_ < buckets_.end(); ++bucket_, ++mutex_) {
            result.merge(*bucket_);
        }
        for (auto& i : bucket_mutexes_) {
            i.unlock();
        }
        return result;
    }

private:
    std::vector<MapType> buckets_;
    std::vector<std::map<Key, std::mutex>> value_mutexes_;
    std::vector<std::mutex> bucket_mutexes_;

    uint64_t GetBucketIndex(uint64_t key) {
        return key % static_cast<uint64_t>(buckets_.size());
    }
};


using namespace std;

void RunConcurrentUpdates(ConcurrentMap<int, int>& cm, size_t thread_count, int key_count) {
    auto kernel = [&cm, key_count](int seed) {
        vector<int> updates(key_count);
        iota(begin(updates), end(updates), -key_count / 2);
        shuffle(begin(updates), end(updates), mt19937(seed));

        for (int i = 0; i < 2; ++i) {
            for (auto key : updates) {
                ++cm[key].ref_to_value;
            }
        }
    };

    vector<future<void>> futures;
    for (size_t i = 0; i < thread_count; ++i) {
        futures.push_back(async(kernel, i));
    }
}

void TestConcurrentUpdate() {
    constexpr size_t THREAD_COUNT = 3;
    constexpr size_t KEY_COUNT = 50000;

    ConcurrentMap<int, int> cm(THREAD_COUNT);
    RunConcurrentUpdates(cm, THREAD_COUNT, KEY_COUNT);

    const auto result = cm.BuildOrdinaryMap();
    ASSERT_EQUAL(result.size(), KEY_COUNT);
    for (auto& [k, v] : result) {
        AssertEqual(v, 6, "Key = " + to_string(k));
    }
}

void TestReadAndWrite() {
    ConcurrentMap<size_t, string> cm(5);

    auto updater = [&cm] {
        for (size_t i = 0; i < 50000; ++i) {
            cm[i].ref_to_value.push_back('a');
        }
    };
    auto reader = [&cm] {
        vector<string> result(50000);
        for (size_t i = 0; i < result.size(); ++i) {
            result[i] = cm[i].ref_to_value;
        }
        return result;
    };

    auto u1 = async(updater);
    auto r1 = async(reader);
    auto u2 = async(updater);
    auto r2 = async(reader);

    u1.get();
    u2.get();

    for (auto f : {&r1, &r2}) {
        auto result = f->get();
        ASSERT(all_of(result.begin(), result.end(), [](const string& s) {
            return s.empty() || s == "a" || s == "aa";
        }));
    }
}

void TestSpeedup() {
    {
        ConcurrentMap<int, int> single_lock(1);

        LOG_DURATION("Single lock");
        RunConcurrentUpdates(single_lock, 4, 50000);
    }
    {
        ConcurrentMap<int, int> many_locks(100);

        LOG_DURATION("100 locks");
        RunConcurrentUpdates(many_locks, 4, 50000);
    }
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestConcurrentUpdate);
    RUN_TEST(tr, TestReadAndWrite);
    RUN_TEST(tr, TestSpeedup);
}
