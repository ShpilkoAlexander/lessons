#pragma once

#include <cassert>
#include <initializer_list>
#include <algorithm>
#include <stdexcept>
#include <iostream>

#include "array_ptr.h"

using namespace std;

struct ReserveProxyObj {
    size_t capacity_ = 0;

public:
    ReserveProxyObj(size_t capacity) : capacity_(capacity) {
    }
};

ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
}

template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size) : items_(size), size_(size), capacity_(size) {
        Type t{};
        fill(this->begin(), this->end(), t);
    }

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value) : items_(size), size_(size), capacity_(size) {
        fill(this->begin(), this->end(), value);
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(initializer_list<Type> init) : items_(init.size()), size_(init.size()), capacity_(init.size()) {
        copy(init.begin(), init.end(), items_.Get());
    }

    SimpleVector(const SimpleVector& other) :  items_(other.size_), size_(other.size_), capacity_(other.size_) {
        copy(other.begin(), other.end(), items_.Get());
    }

    SimpleVector(ReserveProxyObj reserve) :  items_(reserve.capacity_), capacity_(reserve.capacity_) {
    }

    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept {
        return size_;
    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept {
        return capacity_;
    }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept {
        return items_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        return items_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        if (index >= size_) {
            throw out_of_range("Exceeding index"s);
        }
        return items_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        if (index >= size_) {
            throw out_of_range("Exceeding index"s);
        }
        return items_[index];
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept {
        size_ = 0;
    }

    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size) {
        // Напишите тело самостоятельно
        Type t{};
        if (new_size <= size_) {
            size_ = new_size;
        }
        else if (new_size > size_ && new_size <= capacity_) {
            fill(items_.Get() + size_, items_.Get() + new_size, t);
            size_ = new_size;
        }
        else {
            const size_t new_capacity = std::max(capacity_ * 2, new_size);
            ArrayPtr<Type> extended_vector(new_capacity);
            copy(items_.Get(), items_.Get() + size_, extended_vector.Get());
            fill(extended_vector.Get() + size_, extended_vector.Get() + new_size, t);
            items_.swap(extended_vector);
            size_ = new_size;
            capacity_ = new_capacity;
        }
    }

    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept {
        return &items_[0];
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept {
        return &items_[size_];
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept {
        return ConstIterator{&items_[0]};
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept {
        return  ConstIterator{&items_[size_]};
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept {
        return begin();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept {
        return end();
    }

    SimpleVector& operator=(const SimpleVector& rhs) {
        if (items_.Get() == rhs.items_.Get() && size_ == rhs.size_) {
            return *this;
        }
        SimpleVector tmp(rhs);
        swap(tmp);
        return *this;
    }

    // Добавляет элемент в конец вектора
    // При нехватке места увеличивает вдвое вместимость вектора
    void PushBack(const Type& item) {
        Resize(size_ + 1);
        *next(end(), -1) = item;
    }

    // Вставляет значение value в позицию pos.
    // Возвращает итератор на вставленное значение
    // Если перед вставкой значения вектор был заполнен полностью,
    // вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
    Iterator Insert(ConstIterator pos, const Type& value) {
        Iterator new_pos = begin() + (pos - cbegin());

        if (GetSize() < GetCapacity()) {
            std::copy_backward(new_pos, end(), next(end()));
            items_[new_pos-begin()] = value;
            ++size_;
            return new_pos;
        }
        else {
            const size_t new_capacity = std::max(capacity_ * 2, size_ + 1);
            ArrayPtr<Type> extended_vector(new_capacity);

            copy(begin(), new_pos, extended_vector.Get());
            Iterator tmp_pos = extended_vector.Get() + (pos - begin());
            extended_vector[tmp_pos - extended_vector.Get()] = value;
            copy(new_pos, end(), next(tmp_pos));
            items_.swap(extended_vector);
            ++size_;
            capacity_ = new_capacity;
            return tmp_pos;
        }


    }

    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept {
        if (!IsEmpty()) {
            --size_;
        }
    }

    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos) {
        Iterator new_pos = begin() + (pos - cbegin());
        std::copy_backward(next(new_pos), end(), next(end(), -1));
        --size_;
        return new_pos;
    }

    // Обменивает значение с другим вектором
    void swap(SimpleVector& other) noexcept {
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        items_.swap(other.items_);
    }

    void Reserve(size_t new_capacity) {
        if (new_capacity > GetCapacity()) {
            ArrayPtr<Type> new_items(new_capacity);
            copy(begin(), end(), new_items.Get());
            items_.swap(new_items);
            capacity_ = new_capacity;
        }
    }



private:
    ArrayPtr<Type> items_;

    size_t size_ = 0;
    size_t capacity_ = 0;
};


template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    if (lhs.GetSize() != rhs.GetSize()) {
        return false;
    }
    return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin() );
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return std::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs > rhs);
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs < rhs);
}
