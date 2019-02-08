#include "iostream"
#include "function.h"
#include <functional>

struct A {
    A() = default;

    A(A const &) {
        std::cout << "copy constructor" << std::endl;
    }

    A(A &&) noexcept {
        std::cout << "move constructor" << std::endl;
    }

    int operator()() {
        return 2;
    }

    A &operator=(const A &) {
        std::cout << "copy operator=" << std::endl;
        return *this;
    }

    A &operator=(A &&) noexcept {
        std::cout << "move operator=" << std::endl;
        return *this;
    }

    static void start() {
        function<int()> f = A();
        auto f1 = f; //NOLINT
        std::cout << "_____________________________" << std::endl;
        std::function<int()> _f = A();
        auto _f1 = _f; //NOLINT
    }
};
