#include <iostream>

// 错误,函数模板不支持偏特化
// template<int N, char... chars>
// constexpr auto generateStr(){
//     return generateStr<N / 10, '0' + N % 10, chars...>();
// }

// template<char... chars>
// constexpr auto generateStr<0>(){
//     return {chars..., '\0'};
// }

// 错误，你的代码问题在于 返回值类型的推断 和 聚合类型的使用。具体地：

// 返回值类型的推断
// C++ 中，return {chars..., '\0'}; 的返回值推断会失败，因为 {chars..., '\0'} 是一个匿名的初始化列表，编译器无法推断其类型。

// 初始化列表不支持直接返回
// {chars..., '\0'} 本质上是一个初始化列表，不能被直接返回作为返回值类型。


// template<int N, char... chars>
// constexpr auto generateStr(){
//     if constexpr (N == 0){
//         return {chars..., '\0'};
//     }
//     return generateStr<N / 10, '0' + N % 10, chars...>();
// }

// constexpr if 是在 编译时进行条件判断，它只会选择符合条件的分支来编译。
// 因此，在递归展开时，如果条件不满足，相关代码会被编译器忽略掉。
// 然而，递归的本质仍然是对模板进行递归实例化，因此在每次递归时，
// 模板实例化并不受 constexpr if 控制，依然会产生重复的实例化计算，
// 导致编译时间过长和递归展开过多。递归计算会导致栈溢出：

// 即便使用了 constexpr if，递归过程仍然在编译时展开，并不会立即停止。
// 在每次递归展开时，generateStr<N / 10, '0' + N % 10, chars...>仍然会进行模板实例化，
// 因此这种递归展开不会得到即时的优化。尤其是 N 较大的时候，
// 编译器可能会遇到递归深度过大的问题，导致编译失败或栈溢出（stack overflow）问题。
// template<int N, char... chars>
// constexpr auto generateStr() {
//     if constexpr (N == 0) {
//         return std::array<char, sizeof...(chars) + 1>{chars..., '\0'};
//     }
//     return generateStr<N / 10, '0' + N % 10, chars...>();
// }

// 辅助模板：用于存储字符串
template <char... chars>
struct String {
    static constexpr const char value[] = {chars..., '\0'};
};

// 主模板：将整数转为字符串
template <int N, char... chars>
struct IntToString : IntToString<N / 10, '0' + (N % 10), chars...> {};

// 特化：处理整数为0的情况
template <char... chars>
struct IntToString<0, chars...> {
    using type = String<chars...>;
};

int main(){
    constexpr auto str = IntToString<12345>::type::value;
    endl(std::cout << str);
}