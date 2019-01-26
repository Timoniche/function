#include <iostream>
#include "function.h"
#include <functional>
//int func1(const int * x, int y) {
//    return (*x) + y;
//}
//
//int func2(const int * x, int y) {
//    return (*x) - y;
//}
//
//int func3(const int * x, int y) {
//    return (*x) + 2 * y;
//}

struct A {
    A() = default;
    A(A const&) {
        std::cout << "copy constructor" << std::endl;
    }
    A(A&&) noexcept {
        std::cout << "move constructor" << std::endl;
    }
    int operator()() {
        return 2;
    }
    A& operator=(const A&) {
        std::cout << "copy operator=" << std::endl;
        return *this;
    }
    A& operator=(A&&) noexcept {
        std::cout << "move operator=" << std::endl;
        return *this;
    }

};

int main() {
    //std::function<int()> f = A();
    function<int()> f = A();
    std::cout << f() << std::endl;
//    typedef function<int (const int * , int)> int_function_with_two_args_t;
//    function<void ()> f0;
//    int_function_with_two_args_t f1(func1);
//    int_function_with_two_args_t f2(func2);
//    int_function_with_two_args_t f3(func3);
//    std::cout << "gap byte size is: " << f1.get_gap_size() << std::endl; //NOLINT
//    int x = 10;
//    std::cout << "constructors: " << std::endl;
//    std::cout << "calling empty f0: " << std::endl;
//    if (f0) { std::cout << "f0 is true" << std::endl; }
//    else { std::cout << "f0 is false" << std::endl; }
//    std::cout << "calling function1 with signature int (const int * , int) f1: " <<  f1(&x, 20) << std::endl;
//    std::cout << "calling function2 with signature int (const int * , int) f2: " <<  f2(&x, 20) << std::endl;
//    std::cout << "calling function1 with signature int (const int * , int) f3: " <<  f3(&x, 20) << std::endl;
//    f1.swap(f2);
//    std::cout << "____________________________________________________________" << std::endl;
//    std::cout << "calling swap<f1, f2> f1: " <<  f1(&x, 20) << std::endl;
//    std::cout << "calling swapped f2: " <<  f2(&x, 20) << std::endl;
//    std::cout << "____________________________________________________________" << std::endl;
//    std::cout << "copy constructor of f1: " << std::endl;
//    int_function_with_two_args_t f1_copy(f1);
//    std::cout << "calling f1_copy: " <<  f1(&x, 20) << std::endl;
//    std::cout << "____________________________________________________________" << std::endl;
//    std::cout << "operator=: f2 = f3: " << std::endl;
//    f2 = f3;
//    std::cout << "calling f2 == f3: " <<  f2(&x, 20) << std::endl;
//    std::cout << "____________________________________________________________" << std::endl;
//    int_function_with_two_args_t f_moved(std::move(f1));
//    std::cout << "calling move constructor: " <<  f_moved(&x, 20) << std::endl;
//    std::cout << "____________________________________________________________" << std::endl;
    return 0;
}