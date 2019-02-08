#include "test_loskutov.cpp"
#include "my_base_test.cpp"
#include "test_eugene.cpp"

int main() {
    ///my test
    my_base_test::start();

    ///test eugene
    A::start();

    ///assert test_loskutov
    foo::start();
    return 0;
}