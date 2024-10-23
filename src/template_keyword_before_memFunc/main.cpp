#include <iostream>

template <typename T>
struct Wrapper {
    template <typename U>
    void func(U value) {
        std::cout << value << std::endl;
    }

    void func2(int value) {
        std::cout << value << std::endl;
    }
};

template <typename T>
void call(Wrapper<T>& w) {
    w.template func<int>(42);  // 显式指定模板参数 U 为 int
}

// template <typename T>
// void call2(Wrapper<T>& w) {
//     w.func<int>(42);  // error, expected primary-expression before ‘int’
// }

template <typename T>
void call2(Wrapper<T>& w) {
    w.template func2(42);  // 可以通过编译
}

int main() {
    Wrapper<int> w;
    call(w);
    call2(w);
}

