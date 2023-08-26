#include <vector>
#include <cstdint>
#include <iostream>
#include <algorithm>
#include <set>
#include <sstream>
#include <iomanip>

static const size_t MAX_PAGES_COUNT =  1001;
static const size_t MAX_USER_COUNT =  100001;

void CoutCheer(double stats) {
    std::cout << std::setprecision(6) << stats << std::endl;
}

class EBook
{
public:
    EBook()
    {
        pages_read_.resize(MAX_USER_COUNT);
        read_stat_.resize(MAX_PAGES_COUNT);
    }

    void Read(size_t user_id, size_t page_count)
    {
        if (pages_read_.at(user_id) == 0) {
            ++users_count_;
        }

        auto iter_pages_read = read_stat_.begin() + pages_read_[user_id] + 1; // итератор на первый число страниц, которое необходимо прибавлять
        size_t add_pages_count = page_count - pages_read_[user_id]; // количество новых прочитанных страниц

        std::for_each(iter_pages_read, iter_pages_read + add_pages_count,
                      [](size_t& user_count) {++user_count;}
        );

        pages_read_[user_id] = page_count;
    }

    double Cheer(size_t user_id)
    {
        if (pages_read_.at(user_id) == 0)
        {
            return 0.0;
        }

        if (users_count_== 1) {
            return 1.0;
        }

        size_t num_pages_read = pages_read_[user_id]; // количество прочитанных страниц пользователем
        size_t users_read_less_count = users_count_ - read_stat_[num_pages_read]; // количество пользователей прочитавших меньше
        return users_read_less_count * 1.0 / (users_count_ - 1);
    }


private:
    std::vector<size_t> pages_read_; //по индексу n хранится номер страницы, до которой дочитал пользователь id, равным n.
    std::vector<size_t> read_stat_; //по каждому индексу m хранится количество пользователей, дочитавших как минимум до страницы m.
    size_t users_count_ = 0; //количество пользователей
};

void Read(std::istream& input) {
    EBook ebook;
    std::string line, command_name;
    size_t page, user_id;
    size_t repeater;

    std::string count_to_int;
    getline(input, count_to_int);
    repeater = stoi(count_to_int);

    std::stringstream ss;
    for (size_t i = 0; i < repeater; i++) {
        getline(input, line);
        ss.clear();
        ss << line;
        ss >> command_name;

        if (command_name == "READ") {
            ss >> user_id >> page;
            ebook.Read(user_id, page);
        }

        else if (command_name == "CHEER") {
            ss >> user_id;
            CoutCheer(ebook.Cheer(user_id));
        }
    }
}

int main()
{
    Read(std::cin);
}
