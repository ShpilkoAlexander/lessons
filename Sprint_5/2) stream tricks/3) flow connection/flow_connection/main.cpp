#include "log_duration.h"

#include <iostream>

using namespace std;

class StreamUntier {
public:
    StreamUntier(istream& input)
                :input_(input) {
        tied_before_ = input_.tie(nullptr);
    }

    ~StreamUntier() {
        input_.tie(tied_before_);
    }

private:
    ostream* tied_before_;
    istream& input_;
};

int main() {
    LOG_DURATION("\\n with tie"s);

    StreamUntier guard(cin);
    int i;
    while (cin >> i) {
        cout << i * i << "\n"s;
    }

    return 0;
}
