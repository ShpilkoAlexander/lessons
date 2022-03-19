#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

template <typename T>
void RunTestImpl(const T& func, const string& str_func) {
    /* Напишите недостающий код */
    func();
    cerr << str_func << " OK" << endl;


}

#define RUN_TEST(func) RunTestImpl((func), #func) // напишите недостающий код

int Test1() {

    return 1;
}

int main() {
    RUN_TEST(Test1);
}
