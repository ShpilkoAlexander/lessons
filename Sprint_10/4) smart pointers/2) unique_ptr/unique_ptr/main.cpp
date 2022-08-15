//#include <cassert>
//#include <iostream>
//#include <memory>
//#include <string>

//using namespace std;

//struct Cat {
//    Cat(const string& name, int age)
//        : name_(name)
//        , age_(age)  //
//    {
//    }
//    const string& GetName() const noexcept {
//        return name_;
//    }
//    int GetAge() const noexcept {
//        return age_;
//    }
//    ~Cat() {
//    }
//    void Speak() const {
//        cout << "Meow!"s << endl;
//    }

//private:
//    string name_;
//    int age_;
//};

//// Функция создаёт двухлетних котов
//unique_ptr<Cat> CreateCat(const string& name) {
//    return make_unique<Cat>(name, 2);
//}

//class Witch {
//public:
//    explicit Witch(const string& name)
//        : name_(name) {
//    }

//    Witch(Witch&&) = default;
//    Witch& operator=(Witch&&) = default;

//    const string& GetName() const noexcept {
//        return name_;
//    }
//    void SetCat(unique_ptr<Cat>&& cat) noexcept {
//        cat_ = std::move(cat);
//    }
//    unique_ptr<Cat> ReleaseCat() noexcept {
//        return std::move(cat_);
//    }

//    Witch(const Witch& other) {
//        name_ = other.name_;
//        cat_ = make_unique<Cat>(other.cat_->GetName(), other.cat_->GetAge());
//    }
//    Witch& operator=(const Witch& other) {
//        Witch temp(other);
//        name_.swap(temp.name_);
//        cat_.swap(temp.cat_);

//        return *this;
//    }



//private:
//    string name_;
//    unique_ptr<Cat> cat_;
//};

//void Test() {
//    // Объекты Witch можно перемещать
//    {
//        Witch witch("Hermione"s);
//        auto cat = CreateCat("Crookshanks"s);
//        Cat* raw_cat = cat.get();
//        assert(raw_cat);
//        witch.SetCat(move(cat));

//        Witch moved_witch(std::move(witch));
//        auto released_cat = moved_witch.ReleaseCat();
//        assert(released_cat.get() == raw_cat);  // Кот переместился от witch к moved_witch
//    }

//    // Можно использовать перемещающий оператор присваивания
//    {
//        Witch witch("Hermione"s);
//        auto cat = CreateCat("Crookshanks"s);
//        Cat* raw_cat = cat.get();
//        witch.SetCat(move(cat));

//        Witch witch2("Minerva McGonagall");
//        witch2 = move(witch);
//        auto released_cat = witch.ReleaseCat();
//        assert(!released_cat);
//        released_cat = witch2.ReleaseCat();
//        assert(released_cat.get() == raw_cat);
//    }

//    // Можно копировать волшебниц
//    {
//        Witch witch("Hermione");
//        auto cat = CreateCat("Crookshanks"s);
//        witch.SetCat(move(cat));

//        Witch witch_copy(witch);
//        assert(!cat);
//        cat = witch.ReleaseCat();
//        assert(cat);  // У первой волшебницы кот никуда не делся

//        auto cat_copy = witch_copy.ReleaseCat();
//        assert(cat_copy != nullptr && cat_copy != cat);
//        assert(cat_copy->GetName() == cat->GetName());  // Копия волшебницы содержит копию кота
//    }

//    // Работает копирующее присваивание волшебниц
//    {
//        Witch witch("Hermione"s);
//        auto cat = CreateCat("Crookshanks"s);
//        witch.SetCat(move(cat));

//        Witch witch2("Minerva McGonagall"s);
//        witch2 = witch;

//        assert(!cat);
//        cat = witch.ReleaseCat();
//        assert(cat);  // У первой волшебницы кот никуда не делся

//        auto cat_copy = witch2.ReleaseCat();
//        assert(cat_copy != nullptr && cat_copy != cat);
//        assert(cat_copy->GetName() == cat->GetName());  // При присваивании скопировался кот
//    }
//}

//int main() {
//    Test();
//}


#include <cassert>
#include <iostream>
#include <memory>

template <typename T>
struct TreeNode;

template <typename T>
using TreeNodePtr = std::unique_ptr<TreeNode<T>>;

template <typename T>
struct TreeNode {
    // Используйте TreeNodePtr<T> вместо сырых указателей
    // Примите умные указатели по rvalue-ссылке
    TreeNode(T val, TreeNodePtr<T>&& left, TreeNodePtr<T>&& right)
        : value(std::move(val))
        , left(move(left))
        , right(move(right)) {
    }

    T value;
    TreeNodePtr<T> left;  // Замените TreeNode* на TreeNodePtr<T>
    TreeNodePtr<T> right; // Замените TreeNode* на TreeNodePtr<T>
    TreeNode* parent = nullptr;

    TreeNode(const TreeNode& other) {
            value = other.value;
            left = TreeNodePtr<T>(other.left.get());
            right = TreeNodePtr<T>(other.right.get());
            parent = other.parent;
    }

    TreeNode(TreeNode&&) = default;
    TreeNode& operator=(TreeNode&&) = default;

    TreeNode& operator=(const TreeNode& other) {
        TreeNode temp(other);
        value.swap(temp.value);
        left.swap(temp.left);
        right.swap(temp.right);
        parent.swap(parent.left);

        return *this;
    }
};



template <typename T>
bool CheckMinMax(const TreeNode<T>* node, const T* min, const T* max) {
    int a = -1;
    int b = 1000000;

    const int* min_c = min == nullptr ? &a : min;
    const int* max_c = max == nullptr ? &b : max;

    return node->value > *min_c && node->value < *max_c;
}

template <typename T>
bool CheckTreeProperty(const TreeNode<T>* node, const T* min, const T* max) {
    if (node == nullptr) {
        return true;
    }

    if (node->left == nullptr) {
        if (node->right == nullptr) {
            return true;
        }

        else {
            if (!CheckMinMax<T>(node->right.get(), &node->value, max)) {
                return false;
            }
            return CheckTreeProperty<T>(node->right.get(), &node->value, max);

        }
    }

    else {
        if (node->right != nullptr) {
            if (!CheckMinMax(node->right.get(), &node->value, max) || !CheckMinMax(node->left.get(), min, &node->value)){
                return false;
            }
            return CheckTreeProperty<T>(node->right.get(), &node->value, max) && CheckTreeProperty<T>(node->left.get(), min, &node->value);;
        }

        else {
            if (!CheckMinMax(node->left.get(), min, &node->value)) {
                return false;
            }
            return CheckTreeProperty<T>(node->left.get(), min, &node->value);
        }
    }

}

template <typename T>
bool CheckTreeProperty(const TreeNode<T>* node) {
    return CheckTreeProperty<T>(node, nullptr, nullptr);

}


template <typename T>
TreeNode<T>* begin(TreeNode<T>* node) noexcept {
    TreeNode<T>* check_node = node;
    while (check_node->left.get()) {
        check_node = check_node->left.get();
    }

    return check_node;
}




template <typename T>
TreeNode<T>* next(TreeNode<T>* node) {

    if (node->right.get() != nullptr) {
        return begin(node->right.get());
    }

    TreeNode<T>* node_ = node;

    if (node_->parent == nullptr) {
        return nullptr;
    }

    while (node_->parent->left.get() != node_) {
        node_ = node_->parent;

        if (node_->parent == nullptr) {
            return nullptr;
        }

    }

    return node_->parent;

}


// функция создаёт новый узел с заданным значением и потомками
TreeNodePtr<int> N(int val, TreeNodePtr<int>&& left = {}, TreeNodePtr<int>&& right = {}) {

    TreeNodePtr<int> res = TreeNodePtr<int>(new TreeNode<int>{val, std::move(left), std::move(right)});
    if (res->left) {
        res->left->parent = res.get();
    }
    if (res->right) {
        res->right->parent = res.get();
    }

    return res;
}

int main() {
    using namespace std;
    using T = TreeNode<int>;
    auto root1 = N(6, N(4, N(3), N(5)), N(7));
    assert(CheckTreeProperty(root1.get()));
    T* iter = begin(root1.get());
    while (iter) {
        cout << iter->value << " "s;
        iter = next(iter);
    }
    cout << endl;

    auto root2 = N(6, N(4, N(3), N(5)), N(7, N(8)));
    assert(!CheckTreeProperty(root2.get()));

    // Функция DeleteTree не нужна. Узлы дерева будут рекурсивно удалены
    // благодаря деструкторам unique_ptr
}
