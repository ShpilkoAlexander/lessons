#include "ini.h"
#include <string>
#include <unordered_map>

using namespace std;

// место для реализаций методов и функций библиотеки ini
// не забудьте, что они должны быть помещены в namespace ini

namespace ini {
Section& Document::AddSection(std::string name) {
    return sections_[name];
}

const Section& Document::GetSection(const std::string& name) const {
    const auto finded_section = sections_.find(name);

    if (finded_section != sections_.end()) {
        return sections_.at(name);;
    }

    else {
        static Section sec;
        return sec;
    }


}

std::size_t Document::GetSectionCount() const {
    return sections_.size();
}

std::string DeleteSpaces(std::string str) {
    auto start_str = str.find_first_not_of(' ');
    size_t size_str = str.find_last_not_of(' ') - start_str;
    return str.substr(start_str, size_str + 1);
}

std::string NameSection(std::string str) {
    str = DeleteSpaces(str);
    size_t size_string = str.find_last_not_of(']') - str.find_first_not_of('[');
    string name_section = str.substr(str.find_first_not_of('['), size_string + 1);

    return name_section;

}

std::pair<std::string, std::string> KeyAndValue(std::string str) {
    auto start_key = str.find_first_not_of(' ');
    size_t size_key = str.find('=') - start_key;
    std::string key = str.substr(start_key, size_key);
    key = DeleteSpaces(key);

    std::string value = str.substr(start_key + size_key + 1, str.size() - size_key - 1);
    value = DeleteSpaces(value);

    return {key, value};



}

bool IsEmptyString(std::string str) {
    if (str == "") {
        return true;
    }

    for (const char ch : str) {
        if (ch != ' ') {
            return false;
        }
    }

    return true;
}

Document Load(std::istream& input) {
    Document result;
    std::string readed_string;
    Section* section;



    while (getline(input, readed_string)) {
        if (IsEmptyString(readed_string)) {
            continue;
        }

        if (readed_string.find('[') != string::npos) {
            section = &result.AddSection(NameSection(readed_string));
            continue;
        }

        auto key_and_value = KeyAndValue(readed_string);
        section->operator [](key_and_value.first) = key_and_value.second;

    }

    return result;
}

}


