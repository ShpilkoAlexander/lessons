#include <stdexcept>
#include <utility>
#include <iostream>


// Исключение этого типа должно генерироватся при обращении к пустому optional
class BadOptionalAccess : public std::exception {
public:
    using exception::exception;

    virtual const char* what() const noexcept override {
        return "Bad optional access";
    }
};

template <typename T>
class Optional {
public:
    Optional() = default;
    Optional(const T& value) {
        value_ = new (&data_[0]) T(value);
        is_initialized_ = true;
    }
    Optional(T&& value) {
        value_ = new (&data_[0]) T(std::move(value));
        is_initialized_ = true;
    }
    Optional(const Optional& other) {
        if (other.is_initialized_) {
            value_ = new (&data_[0]) T(*other.value_);
            is_initialized_= true;
        }
    }
    Optional(Optional&& other) {
        if (other.is_initialized_) {
            value_ = new (&data_[0]) T(std::move(*other.value_));
            is_initialized_= true;
        }
    }

    Optional& operator=(const T& value) {
        if (is_initialized_) {
            *value_ = value;
        } else {
            value_ = new (&data_[0]) T(value);
            is_initialized_ = true;
        }
        return *this;
    }

    Optional& operator=(T&& value) {
        if (is_initialized_) {
            *value_ = std::move(value);
        } else {
            value_ = new (&data_[0]) T(std::move(value));
            is_initialized_ = true;
        }
        return *this;
    }

    Optional& operator=(const Optional& rhs) {
        if (this != &rhs) {
            if (is_initialized_ && rhs.is_initialized_) {
                *value_ = *rhs.value_;
            }
            else if (is_initialized_ && !rhs.is_initialized_) {
                value_->~T();
                is_initialized_ = false;
            }
            else if (!is_initialized_ && rhs.is_initialized_) {
                value_ = new (&data_[0]) T(*rhs.value_);
                is_initialized_= true;
            }
        }

        return *this;
    }
    Optional& operator=(Optional&& rhs) {
        if (this != &rhs) {
            if (is_initialized_ && rhs.is_initialized_) {
                *value_ = std::move(*rhs.value_);
            }
            else if (is_initialized_ && !rhs.is_initialized_) {
                value_->~T();
                is_initialized_ = false;
            }
            else if (!is_initialized_ && rhs.is_initialized_) {
               value_ = new (&data_[0]) T(std::move(*rhs.value_));
            }
        }

        return *this;
    }

    ~Optional() {
        if (is_initialized_) value_->~T();
    }

    bool HasValue() const {
        return is_initialized_;
    }

    // Операторы * и -> не должны делать никаких проверок на пустоту Optional.
    // Эти проверки остаются на совести программиста
    T& operator*() {
        return *value_;
    }
    const T& operator*() const {
        return *value_;
    }
    T* operator->() {
        return value_;
    }
    const T* operator->() const {
        return value_;
    }

    // Метод Value() генерирует исключение BadOptionalAccess, если Optional пуст
    T& Value() {
        if (!is_initialized_) {
            throw BadOptionalAccess();
        }
        return *value_;
    }
    const T& Value() const {
        if (!is_initialized_) {
            throw BadOptionalAccess();
        }
        return *value_;
    }

    void Reset() {
        value_->~T();
        is_initialized_ = false;
    }

private:
    // alignas нужен для правильного выравнивания блока памяти
    alignas(T) char data_[sizeof(T)];
    bool is_initialized_ = false;
    T* value_;
};
