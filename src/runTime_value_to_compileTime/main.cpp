#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<vector>
#include<tuple>

//如何根据一个运行期的值调用一个编译器模板函数
template<size_t N>
void fun() {
	std::cout << N << std::endl;
}

template<size_t... Is, typename Fun>
void process(int i, std::index_sequence<Is...>, Fun&& f){
    (void)((i == Is ? (f(std::integral_constant<size_t, Is>{}), 0) : 0), ...);
    // (f(std::integral_constant<size_t, Is>{}), ...);
}

void generate(int n) {
    process(n, std::make_index_sequence<10>{},[](auto index){
        fun<index>();
    });
}

int main() {
	srand(time(nullptr));
	int a = rand() % 10;
	std::cout << "a = " << a << std::endl;
	generate(a);
	return 0;
}

