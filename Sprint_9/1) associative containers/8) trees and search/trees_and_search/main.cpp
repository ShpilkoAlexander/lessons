#include <cassert>
#include <iostream>

using namespace std;

template <typename T>
struct TreeNode {
    T value;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
};

template <typename T>
void DeleteTree(TreeNode<T>* node) {
    if (!node) {
        return;
    }
    DeleteTree(node->left);
    DeleteTree(node->right);
    delete node;
}

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
            if (!CheckMinMax<T>(node->right, &node->value, max)) {
                return false;
            }
            return CheckTreeProperty<T>(node->right, &node->value, max);

        }
    }

    else {
        if (node->right != nullptr) {
            if (!CheckMinMax(node->right, &node->value, max) || !CheckMinMax(node->left, min, &node->value)){
                return false;
            }
            return CheckTreeProperty<T>(node->right, &node->value, max) && CheckTreeProperty<T>(node->left, min, &node->value);;
        }

        else {
            if (!CheckMinMax(node->left, min, &node->value)) {
                return false;
            }
            return CheckTreeProperty<T>(node->left, min, &node->value);
        }
    }
}

template <typename T>
bool CheckTreeProperty(const TreeNode<T>* node) {
    return CheckTreeProperty<T>(node, nullptr, nullptr);;

}


int main() {
    using T = TreeNode<int>;
    T* root1 = new T{6,
        new T{4, new T{3}, new T{5}}, new T{7}};
    assert(CheckTreeProperty(root1));

    T* root2 = new T{6,
        new T{4, new T{3}, new T{5}}, new T{7, new T{8}}};
    assert(!CheckTreeProperty(root2));

    DeleteTree(root1);
    DeleteTree(root2);
    cout << "Success" << endl;

}
