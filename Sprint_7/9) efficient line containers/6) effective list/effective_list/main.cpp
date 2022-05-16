#include <iostream>
#include <list>

using namespace std;

class Editor {
public:
    Editor() = default;

    // сдвинуть курсор влево
    void Left() {
        advance(pointer_, -1);
    }

    // сдвинуть курсор вправо
    void Right() {
        if (pointer_ != text.end()){
        advance(pointer_, 1);
        }
    }

    // вставить символ token
    void Insert(char token) {
        text.insert(pointer_, token);
    }

    // вырезать не более tokens символов, начиная с текущей позиции курсора
    void Cut(size_t tokens = 1) {
        if (tokens > 0) {
            auto begin_delete = next(pointer_, -1);
            Copy(tokens);
            advance(pointer_, buffer.size());
            text.erase(next(begin_delete), pointer_);
        }
    }

    // cкопировать не более tokens символов, начиная с текущей позиции курсора
    void Copy(size_t tokens = 1) {
        if (tokens > 0) {
            auto begin_copy = pointer_;
            buffer.clear();
            for (;(tokens != 0) && (begin_copy != text.end()); advance(begin_copy, 1), --tokens) {
                buffer.push_back(*begin_copy);
            }

        }

    }

    // вставить содержимое буфера в текущую позицию курсора
    void Paste() {
       text.insert(pointer_, buffer.begin(), buffer.end());

    }

    // получить текущее содержимое текстового редактора
    string GetText() const {
        string out_text;
        for (const char symbol : text) {
            out_text.push_back(symbol);
        }
        return out_text;
    }

private:
    list<char> text;
    list<char>::iterator pointer_ = text.begin();
    list<char> buffer;
};


int main() {
    Editor editor;
    const string text = "hello, world"s;
    for (char c : text) {
        editor.Insert(c);
    }

    // Текущее состояние редактора: `hello, world|`
    for (size_t i = 0; i < text.size(); ++i) {
        editor.Left();
    }
    // Текущее состояние редактора: `|hello, world`
    editor.Cut(7);
    // Текущее состояние редактора: `|world`
    // в буфере обмена находится текст `hello, `
    for (size_t i = 0; i < 5; ++i) {
        editor.Right();
    }
    // Текущее состояние редактора: `world|`
    editor.Insert(',');
    editor.Insert(' ');
    // Текущее состояние редактора: `world, |`
    editor.Paste();
    // Текущее состояние редактора: `world, hello, |`
    editor.Left();
    editor.Left();
    //Текущее состояние редактора: `world, hello|, `
    editor.Cut(3);  // Будут вырезаны 2 символа
    // Текущее состояние редактора: `world, hello|`
    cout << editor.GetText();
    return 0;
}
