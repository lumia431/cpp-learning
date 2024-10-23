#include <iostream>
#include <tuple>
#include <utility>

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
