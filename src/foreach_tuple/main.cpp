#include <iostream>
#include <tuple>
#include <utility>

// C++20引入了一种新的Lambda语法，称为“Lambda模板参数”，允许我们在Lambda中使用模板参数。这种特性能让Lambda函数接受不仅仅是类型参数，还可以接受非类型模板参数，例如整数序列
template <typename... Ts>
void print(const std::tuple<Ts...>& tu) {
	[&] <size_t... Is>(std::index_sequence<Is...>) mutable {
		((std::cout << std::get<Is>(tu) << '\t'), ...);
	}(std::make_index_sequence<sizeof...(Ts)>{});
}

template <typename... Ts>
void print2(const std::tuple<Ts...>& tu) {
	auto get_ptrs = [](auto&... _refs) {
		((std::cout << _refs << '\t'), ...);
	};
	std::apply(get_ptrs, tu);
}

int main() {
    print(std::make_tuple(1,2.,"hhh"));
    endl(std::cout << ("~~~~~~~~~~~~~"));
    print2(std::make_tuple(3,4.,"hahaha"));
	return 0;
}
