#include <functional>
#include <assert.h>
#include "function.h"

struct foo {
    int *ptr;
    int x;

    foo(int x) : x{x} { //NOLINT
        ptr = &this->x;
    }

    foo(const foo &other) : foo(other.x) {}

    int operator()(int arg) {
        return *ptr + arg;
    }

    static void start_std() {
        std::function<int(int)> f = foo(42);
        std::function<int(int)> f2 = foo(0);
        f.swap(f2);
        assert(f(228) == 228);
        assert(f2(228) == 270);
    }

    static void start() {
        function<int(int)> f = foo(42);
        function<int(int)> f2 = foo(0);
        f.swap(f2);
        assert(f(228) == 228);
        assert(f2(228) == 270);
    }
};



