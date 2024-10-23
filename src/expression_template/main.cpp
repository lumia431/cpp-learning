#include <iostream>
#include <vector>

// 定义向量类
class Vector {
    std::vector<double> data;

public:
    explicit Vector(std::size_t size) : data(size) {}
    double& operator[](std::size_t i) { return data[i]; }
    const double& operator[](std::size_t i) const { return data[i]; }
    std::size_t size() const { return data.size(); }

    template<typename Expr>
    Vector& operator=(const Expr& expr) {
        for (std::size_t i = 0; i < size(); ++i) {
            data[i] = expr[i];
        }
        return *this;
    }
};

// 定义表达式模板
template<typename L, typename R>
class Add {
    // const Func f;
    const L& lhs;
    const R& rhs;

public:
    Add(const L& l, const R& r) : lhs(l), rhs(r) {}
    double operator[](std::size_t i) const { return lhs[i] + rhs[i]; }
    // double operator[](std::size_t i) const { return f(lhs[i], rhs[i]); } 可以是加减乘除等操作
};

// 操作符重载
template<typename L, typename R>
Add<L, R> operator+(const L& lhs, const R& rhs) {
    return Add<L, R>(lhs, rhs);
}

// Add<Vector, Vector>对象不是一个临时结果吗，依然有内存开销？

// 是的，Add<Vector, Vector> 确实是一个临时对象，但它和传统方法生成的临时 结果向量对象 是完全不同的概念。
// 这个对象本质上是一个轻量级的 表达式描述符，只包含对 Vector 的引用以及描述计算逻辑的信息，
// 而不会真正存储结果数据。因此，它的内存开销远小于直接生成一个完整的中间结果 Vector。
int main() {
    Vector x(3), y(3), z(3);
    for (std::size_t i = 0; i < 3; ++i) {
        x[i] = i + 1;
        y[i] = i * 2;
    }

    z = x + y + x; // 使用表达式模板延迟计算
    for (std::size_t i = 0; i < 3; ++i) {
        std::cout << z[i] << " ";
    }
    std::cout << std::endl;
    return 0;
}
