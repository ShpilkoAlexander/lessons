#include <cstdint>
#include <iostream>
#include <optional>
#include <limits>


using namespace std;


template <typename T>
optional<T> SafeAdd(T a, T b) {
    if ((a > 0 && b > 0) && a > numeric_limits<T>::max() - b) {
        return nullopt;
    }

    if ((a < 0 && b < 0) && a < numeric_limits<T>::min() - b) {
        return nullopt;
    }
    return a + b;

}

int main() {
    int64_t a;
    int64_t b;
    cin >> a >> b;
    auto sum = SafeAdd<int64_t>(a, b);
    if (sum == nullopt) {
        cout << "Overflow!" << endl;
    }
    else {
        cout << a + b << endl;
    }

}
