#include <iostream>
#include <tuple>
#include <array>
#include <utility>  // 包含 std::index_sequence 和 std::make_index_sequence

struct AnyType {
    template <typename T>
    operator T();
};

template <typename T>
consteval size_t CountMember(auto&&... Args) {
    if constexpr (! requires { T{ Args... }; }) { // (1)
        return sizeof...(Args) - 1;
    } else {
        return CountMember<T>(Args..., AnyType{}); // (2)
    }
}

// 获取假的对象（未初始化），用于推导成员
template <class T>
struct my_wrapper {
    inline static std::decay_t<T> value;
};

template <class T>
inline constexpr std::decay_t<T>& Get_fake_object() noexcept {
    return my_wrapper<std::decay_t<T>>::value;
}

template <typename T>
constexpr auto test_to_tuple_impl() {
  auto& [member1, member2, member3] =
      Get_fake_object<T>();  // 假设结构体有三个成员
  auto ref_tup =  std::tie(member1, member2, member3);
  auto get_ptrs = [](auto&... _refs) {
    return std::make_tuple(&_refs...);
  };
  return std::apply(get_ptrs, ref_tup);
}

struct person {
  int age;
  const char* name;
  bool sex;
};

// 获取成员名称的函数
template <auto ptr>
inline constexpr std::string_view get_member_name() {
#if defined(__GNUC__)
    constexpr std::string_view func_name = __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)
    constexpr std::string_view func_name = __FUNCSIG__;
#endif
    return func_name;
}

template <typename... Ts>
void print(const std::tuple<Ts...>& tu) {
  [&]<size_t... Is>(std::index_sequence<Is...>) mutable {
    (..., (std::cout << std::get<Is>(tu)));
  }(std::make_index_sequence<sizeof...(Ts)>{});
}

template <typename T>
inline constexpr std::array<std::string_view, CountMember<T>()>
get_member_names() {
  constexpr size_t Count = CountMember<T>();
  std::array<std::string_view, Count> arr;
  constexpr auto tp = test_to_tuple_impl<T>();
  [&]<size_t... Is>(std::index_sequence<Is...>) mutable {
    ((arr[Is] = get_member_name<std::get<Is>(tp)>()), ...);
  }(std::make_index_sequence<Count>{});
  return arr;
}

int main() {
  person p{1, "lmy", true};
  auto arr = get_member_names<person>();
  for (auto name : arr) {
    std::cout << name << ", ";
  }
  std::cout << "\n";
  return 0;
}