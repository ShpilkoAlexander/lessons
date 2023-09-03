#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <algorithm>
#include <cassert>
#include <sstream>

using namespace std;

class Domain {

public:
    Domain(string name)
        : name_(name) {
        reverse(name_.begin(), name_.end());
        name_ += '.';
    }

    bool operator==(const Domain& other) const {
        return name_ == other.name_;
    }

    bool operator<(const Domain& r) const {
    // сравниваем лексикографически, считая точку самым маленьким символом
    return std::lexicographical_compare(name_.begin(), name_.end(), r.name_.begin(), r.name_.end(), [](char l, char r) {
      return (r != '.') && (l < r || l == '.');});
    }


    string GetName() const {
        return name_;
    }

    // IsSubdomain принимает другой домен и возвращает true, если this его поддомен
    bool IsSubdomain(const Domain& other) const {
        auto mismatched_pair = mismatch(other.name_.begin(), other.name_.end(), name_.begin()); // возвращает первую пару не совпадающих значений
        return mismatched_pair.first == other.name_.end() ? true : false;
    }

private:
    string name_;
};

class DomainChecker {
public:
    template<typename It>
    DomainChecker(It begin, It end)
        : forbidden_domains_(begin, end) {

        sort(forbidden_domains_.begin(), forbidden_domains_.end());
        auto unique = std::unique(forbidden_domains_.begin(), forbidden_domains_.end(),
                        [](const Domain& lhs, const Domain& rhs) {
                        return rhs.IsSubdomain(lhs);
        });
        forbidden_domains_.erase(unique, forbidden_domains_.end());
    }

    vector<Domain> GetDomains() {
        return forbidden_domains_;
    }

    // разработайте метод IsForbidden, возвращающий true, если домен запрещён
    bool IsForbidden(const Domain& domain) {

        vector<Domain>::const_iterator it = upper_bound(forbidden_domains_.begin(), forbidden_domains_.end(), domain);
        return it == forbidden_domains_.begin() ? false : domain.IsSubdomain(*(--it));

    }

private:
    vector<Domain> forbidden_domains_;
};

// разработайте функцию ReadDomains, читающую заданное количество доменов из стандартного входа
std::vector<Domain> ReadDomains(istream& input, size_t count) {
    vector<Domain> domains;
    string line;
    for (size_t i = 0; i < count; ++i) {
        getline(input, line);
        domains.emplace_back(line);
    }
    return domains;
}

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

void TestDomainConstructor() {
    Domain domain("abc.ru");
    assert(domain.GetName() == "ur.cba.");
}

void TestIsSubdomain() {
    {
        Domain domain("ru");
        Domain subdomain_1("abc.ru");
        Domain subdomain_2("ru");

        assert(subdomain_1.IsSubdomain(domain));
        assert(subdomain_2.IsSubdomain(domain));
    }

    {
        Domain domain("gdz.ru");
        Domain subdomain_1("m.gdz.ru");
        Domain subdomain_2("z.ru");

        assert(subdomain_1.IsSubdomain(domain));
        assert(!subdomain_2.IsSubdomain(domain));
    }
}


void TestDomainCheckerConstructor() {
    vector<Domain> forbidden_domains = {Domain("gdz.ru"), Domain("maps.me"), Domain("m.gdz.ru"), Domain("com"), Domain("google.com")};
    vector<Domain> result_domains = {Domain("maps.me"), Domain("com"), Domain("gdz.ru")};
    DomainChecker cheker(forbidden_domains.begin(), forbidden_domains.end());

    assert(cheker.GetDomains() == result_domains);
}

void TestIsForbidden() {
    vector<Domain> forbidden_domains = {Domain("gdz.ru"), Domain("maps.me"), Domain("m.gdz.ru"), Domain("com")};
    vector<Domain> forbidden_domains_2 = {Domain("maps.me"), Domain("gdz.rma")};

    DomainChecker cheker(forbidden_domains.begin(), forbidden_domains.end());
    DomainChecker cheker_2(forbidden_domains_2.begin(), forbidden_domains_2.end());
    Domain check_1("gdz.ru");
    Domain check_2("gdz.com");
    Domain check_3("m.maps.me");
    Domain check_4("alg.m.gdz.ru");
    Domain check_5("maps.com");
    Domain check_6("maps.ru");
    Domain check_7("gdz.ua");

    assert(cheker.IsForbidden(check_1));
    assert(cheker.IsForbidden(check_2));
    assert(cheker.IsForbidden(check_3));
    assert(cheker.IsForbidden(check_4));
    assert(cheker.IsForbidden(check_5));
    assert(!cheker.IsForbidden(check_6));
    assert(!cheker.IsForbidden(check_7));

    assert(cheker_2.IsForbidden(check_3));

    vector<Domain> empty;
    DomainChecker null_cheker(empty.begin(), empty.end());
    assert(!null_cheker.IsForbidden(check_1));

    vector<Domain> one_el;
    one_el.emplace_back(("ru"));
    DomainChecker one_el_cheker(one_el.begin(), one_el.end());
    assert(one_el_cheker.IsForbidden(check_1));

}

void TestReadDomains() {
    stringstream input;
    input << "gdz.ru\n" << "maps.me\n" << "m.gdz.ru\n" << "com\n";
    vector<Domain> forbidden_domains = {Domain("gdz.ru"), Domain("maps.me"), Domain("m.gdz.ru"), Domain("com")};
    assert(forbidden_domains == ReadDomains(input, 4));

}
int main() {
//    TestDomainConstructor();
//    TestIsSubdomain();
//    TestDomainCheckerConstructor();
//    TestIsForbidden();
//    TestReadDomains();


    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());
    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}
