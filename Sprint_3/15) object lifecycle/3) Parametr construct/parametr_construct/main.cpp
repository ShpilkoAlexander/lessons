//Task 1
//#include <string>

//using namespace std;

//enum class AnimalType {
//    Cat,
//    Dog,
//    Mouse,
//};

//class Animal {
//public:
//    Animal() = default;

//    Animal(AnimalType type, const string& name, const string& owner_name){
//        type_ = type;
//        name_ = name;
//        owner_name_ = owner_name;
//    }

//    AnimalType GetType() const {
//        return type_;
//    }

//    const string& GetName() const {
//        return name_;
//    }

//    const string& GetOwnerName() const {
//        return owner_name_;
//    }

//private:
//    AnimalType type_;
//    string name_;
//    string owner_name_;
//};


//Task2
#include <vector>
#include <iostream>
#include <numeric>

using namespace std;



class Rational {
public:
    Rational(){
        Rational(0);
    }

    Rational(int numerator){
        numerator_ = numerator;
        denominator_ = 1;
    }

    Rational(int numerator, int denominator){
        if(denominator < 0){
            numerator = - numerator;
            denominator = - denominator;
        }
        numerator_ = numerator / abs(gcd(numerator, denominator));
        denominator_ = denominator / abs(gcd(numerator, denominator));
    }

    int Numerator() const {
        return numerator_;
    }

    int Denominator() const {
        return denominator_;
    }

private:
    int numerator_ = 0;
    int denominator_ = 1;
};

Rational Add(Rational r1, Rational r2) {
    int numerator = r1.Numerator() * r2.Denominator() + r2.Numerator() * r1.Denominator();
    int denominator = r1.Denominator() * r2.Denominator();

    // Создаём и возвращаем дробь с заданным числителем и знаменателем
    return Rational{numerator, denominator};
}

int main() {
    Rational zero;     // Дробь 0/1 = 0
    const Rational seven(7); // Дробь 7/1 = 7
    const Rational one_third(1, 3); // Дробь 1/3

    vector<Rational> numbers;
    numbers.push_back(Rational{7, 8});

    // Следующие 2 строки эквивалентны - добавляют в numbers дробь 3/1
    numbers.push_back(Rational{3});
    numbers.push_back(3);

    Rational sum = Add(Rational{1,6}, one_third);
    // Выведет 1/2
    cout << sum.Numerator() << "/" << sum.Denominator();
}
