#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

void Assert(bool value, const string& str_func,
            const string& file, const string& function,
            const unsigned line,
            const string& hint){
    if(!value){
        cout << file <<"("s << line << "): "s << function << ": "s;
        cout << "ASSERT(" << str_func << ") failed."s;
        if(!hint.empty()){
            cout << " Hint: "s << hint;
        }
        cout << endl;
        abort();
    }
}

#define ASSERT(expr) Assert(expr, #expr, __FILE__, __FUNCTION__, __LINE__, ""s)
/* реализовать самостоятельно */

#define ASSERT_HINT(expr, hint) Assert(expr, #expr, __FILE__, __FUNCTION__, __LINE__, hint)
/* реализовать самостоятельно */

int main() {
    string hello = "hello"s;
    ASSERT(!hello.empty());
    ASSERT_HINT(2 + 2 == 5, "This will fail"s);
}


