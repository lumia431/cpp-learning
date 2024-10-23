#include <type_traits>
#include <iostream>

// 有问题，每次只跟第一个元素比较，需要双重循环来判断，故再套一层
// template<typename... Args>
// struct DuplicateType : std::false_type {};

// template<typename T, typename U, typename... Args>
// struct DuplicateType<T, U, Args...> : std::conditional_t<std::is_same_v<T ,U>, std::true_type, DuplicateType<T, Args...>> {};

// template<typename T>
// struct DuplicateType<T> : std::false_type {};

// template<typename... Args>
// constexpr bool duplicateType_v = DuplicateType<Args...>::value;


//通常用的方法
template<typename... Args>
struct Contains : std::false_type {};

template<typename T, typename U, typename... Args>
struct Contains<T, U, Args...> : std::conditional_t<std::is_same_v<T ,U>, std::true_type, Contains<T, Args...>> {};

template<typename T>
struct Contains<T> : std::false_type {};



template<typename... Args>
struct HasDuplicate : std::false_type {};

template<typename T, typename... Args>
struct HasDuplicate<T, Args...> : std::conditional_t<Contains<T, Args...>::value, std::true_type, HasDuplicate<Args...>> {};//模板的匹配规则是基于“特化版本优先”的原则,此处HasDuplicate<Args...>不会匹配默认版本

template<typename T>
struct HasDuplicate<T> : std::false_type {};

template<typename... Args>
constexpr bool hasDuplicate_v = HasDuplicate<Args...>::value;

// C++20[[no_unique_address]]实现
//http://www.purecpp.cn/detail?id=2451

int main(){
    std::cout << std::boolalpha; // 打印布尔值
    std::cout << hasDuplicate_v<int, int> << std::endl; // true
    std::cout << hasDuplicate_v<float, float, int> << std::endl; // true
    std::cout << hasDuplicate_v<float, int, double> << std::endl; // false
    std::cout << hasDuplicate_v<float, int, double, int> << std::endl; // true
}