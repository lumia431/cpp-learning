#include <iostream>
#include <string>

// 函数模板
template <typename T>
T add(T a, T b) {
    return a + b;
}

// 显式实例化定义
template std::string add<std::string>(std::string, std::string);   // 只实例化 int 类型的 add 函数

int main() {
    int result = add(3, 4);  // 使用实例化后的 int 版本
    std::cout << result << std::endl;  // 输出：7
}

