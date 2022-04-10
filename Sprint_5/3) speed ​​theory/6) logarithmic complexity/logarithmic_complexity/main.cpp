#include <iostream>

using namespace std;

template <typename F>
int FindFloor(int n, F drop) {
    // Переделайте этот алгоритм, имеющий линейную сложность.
    // В итоге должен получится логарифмический алгоритм.

    int a = 1;
    int average_floor = 0;
    while (a != n) {
        average_floor = (n + a) / 2;
        if (drop(average_floor)) {
            n = average_floor;
        }
        else {
            a = average_floor + 1;
        }
    }

    return n;
}

int main() {
    int n,t;
    cout << "Enter n and target floor number: "s << endl;
    cin >> n >> t;

    int count = 0;
    int found = FindFloor(n, [t, &count](int f) {
        ++count;
        return f >= t;
    });

    cout << "Found floor "s << found << " after "s << count << " drops"s;

    return 0;
}
