#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << " "s;
    }
    cout << endl;
}

template <typename Type>
class Stack {
public:
    void Push(const Type& element) {
        elements_.push_back(element);
    }
    void Pop() {
        elements_.pop_back();
    }
    const Type& Peek() const {
        return elements_.back();
    }
    Type& Peek() {
        return elements_.back();
    }
    void Print() const {
        PrintRange(elements_.begin(), elements_.end());
    }
    uint64_t Size() const {
        return elements_.size();
    }
    bool IsEmpty() const {
        return elements_.empty();
    }

private:
    vector<Type> elements_;
};

// используем струкруту из элемента
// и минимума в стеке до дна стека до текущего элемента включительно
template <typename Type>
struct PairWithMin {
    Type element;
    Type minimum;
};

template <typename Type>
ostream& operator<<(ostream& out, const PairWithMin<Type>& pair_to_print) {
    // будем выводить только сам элемент, выводить минимум не будем
    out << pair_to_print.element;
    return out;
}

template <typename Type>
class StackMin {
public:
    void Push(const Type& new_element) {
        // создадим структуру, которую будем класть в стек
        PairWithMin<Type> new_pair = {new_element, new_element};
        // проверим, что должно стать минимумом для текущего элемента -
        // он сам, или предыдущий минимум
        if (!elements_.IsEmpty() && new_pair.minimum > elements_.Peek().minimum) {
            new_pair.minimum = elements_.Peek().minimum;
        }
        // положим структуру в стек
        elements_.Push(new_pair);
    }
    void Pop() {
        elements_.Pop();
    }
    const Type& Peek() const {
        return elements_.Peek().element;
    }
    Type& Peek() {
        return elements_.Peek().element;
    }
    void Print() const {
        elements_.Print();
    }
    uint64_t Size() const {
        return elements_.size();
    }
    bool IsEmpty() const {
        return elements_.IsEmpty();
    }
    const Type& PeekMin() const {
        return elements_.Peek().minimum;
    }
    Type& PeekMin() {
        return elements_.Peek().minimum;
    }

private:
    // используем в качестве элемента стека нашу структуру
    Stack<PairWithMin<Type>> elements_;
};

template <typename Type>
class SortedStack {
public:
    void Push(const Type& element) {
        Stack<Type> buffer;
        for(;;) {
            if ((elements_.IsEmpty() || elements_.Peek() >= element)){
                elements_.Push(element);
                break;
            }
            else{
                buffer.Push(elements_.Peek());
                elements_.Pop();
            }
        }

        while(!buffer.IsEmpty()){
            elements_.Push(buffer.Peek());
            buffer.Pop();
        }
    }
    void Pop() {
        elements_.Pop();
    }
    const Type& Peek() const {
    return elements_.Peek();
    }
    Type& Peek() {
    return elements_.Peek();
    }
    void Print() const {
    elements_.Print();
    }
    uint64_t Size() const {
    return elements_.Size();
    }
    bool IsEmpty() const {
    return elements_.IsEmpty();
    }
private:
    Stack<Type> elements_;
};

int main() {
    SortedStack<int> stack;
    vector<int> values(5);

    // заполняем вектор для тестирования нашего стека
    iota(values.begin(), values.end(), 1);
    // перемешиваем значения
    random_shuffle(values.begin(), values.end());

    // заполняем стек и проверяем, что сортировка сохраняется после каждой вставки
    for (int i = 0; i < 5; ++i) {
        cout << "Вставляемый элемент = "s << values[i] << endl;
        stack.Push(values[i]);
        stack.Print();
    }
}
