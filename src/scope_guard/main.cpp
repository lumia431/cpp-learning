#include <cstdio>
#include <cassert>

#include <stdexcept>
#include <iostream>
#include <functional>

struct X {
    X() { puts("X()"); }
    X(const X&) { puts("X(const X&)"); }
    X(X&&) noexcept { puts("X(X&&)"); }
    ~X() { puts("~X()"); }
};

template<typename Fun, typename... Args>
struct scope_guard{
	//m_values(std::forward<A>(args)...)此处没有运算符，所以是包展开而不是折叠表达式
	//包展开会自动加上逗号，故此处展开后为m_values(std::forward<int>(a), std::forward<float>(b), std::forward<char>(c));
	template<typename F, typename... A>
	scope_guard(F&& f, A&&... args): m_fun(std::forward<F>(f)), m_values(std::forward<A>(args)...){}
	~scope_guard() {
        std::apply(m_fun, m_values);
    }
    scope_guard(const scope_guard&) = delete;

	Fun m_fun;
	std::tuple<Args...> m_values;
};

//为什么没有这个推导指引无法通过编译
//因为类模板的形参Fun和Args无法被推断
//光凭构造函数中的类型参数F和A，编译期无法得知Fun和Args是什么
//以下两种情况不需要手动添加推导指引
// 1. 当显式传递类型参数时
// template<typename T>
// class Wrapper {
// public:
//     T value;
//     Wrapper(T val) : value(val) {}
// };

// int main() {
//     Wrapper<int> w1(42);  // 显式指定类型参数 int
//     Wrapper<std::string> w2("Hello");  // 显式指定类型参数 std::string
// }

// 2. 当类型参数与构造函数参数一致时
// template<typename T>
// class Box {
// public:
//     T value;
//     Box(T val) : value(val) {}
// };

// int main() {
//     Box b(10);  // 编译器推导 T = int
//     Box s("Hello");  // 编译器推导 T = const char*
// }

template<typename Fun, typename...Args>
scope_guard(Fun&&, Args&&...) -> scope_guard<std::decay_t<Fun>, std::decay_t<Args>...>;

int main() {
    {
        // scope_guard的作用之一，是让各种C风格指针接口作为局部变量时也能得到RAII支持
        // 这也是本题的基础要求
        FILE * fp = nullptr;
        try{
            fp = fopen("test.txt","a");
            auto guard = scope_guard([&] {
                fclose(fp);
                fp = nullptr;
            });

            throw std::runtime_error{"Test"};
        } catch(std::exception & e){
            puts(e.what());
        }
        assert(fp == nullptr);
    }
    puts("----------");
    {
        // 附加要求1，支持函数对象调用
        struct Test {
            void operator()(X* x) {
                delete x;
            }
        } t;
        auto x = new X{};
        auto guard = scope_guard(t, x);
    }
    puts("----------");
    {
        // 附加要求2，支持成员函数和std::ref
        auto x = new X{};
        {
            struct Test {
                void f(X*& px) {
                    delete px;
                    px = nullptr;
                }
            } t;
            auto guard = scope_guard{&Test::f, &t, std::ref(x)};
        }
        assert(x == nullptr);
    }
}