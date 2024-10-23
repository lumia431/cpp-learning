#include <array>
#include <iostream>
#include <tuple>
#include <utility>  // 包含 std::index_sequence 和 std::make_index_sequence

struct AnyType {
  template <typename T>
  operator T();
};

template <typename T>
consteval size_t CountMember(auto&&... Args) {
  if constexpr (!requires { T{Args...}; }) {  // (1)
    return sizeof...(Args) - 1;
  }
  else {
    return CountMember<T>(Args..., AnyType{});  // (2)
  }
}

//这个identity是用来防止隐式转换的
template <typename T>
struct identity {};

template <typename T>
constexpr size_t member_count_v = CountMember<T>();

template <typename T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

template <class T>
struct my_wrapper {
  inline static remove_cvref_t<T> value;
};

template <class T>
inline constexpr remove_cvref_t<T>& Get_fake_object() noexcept {
  return my_wrapper<remove_cvref_t<T>>::value;
}

template <typename T, auto... field>
struct private_visitor {
  friend inline constexpr auto get_private_ptrs(const identity<T>&) {
    constexpr auto tp = std::make_tuple(field...);
    return tp;
  }
};

#define REFL_PRIVATE_(STRUCT, ...)                                    \
  inline constexpr auto get_private_ptrs(const identity<STRUCT> &t);      \
  template struct private_visitor<STRUCT, ##__VA_ARGS__>;                 \
  [[maybe_unused]] inline static decltype(auto) refl_object_to_tuple(     \
      STRUCT &t) {                                                        \
    return refl_object_to_tuple_impl(t);                                  \
  }                                                                       \
  [[maybe_unused]] inline static decltype(auto) refl_object_to_tuple(     \
      const STRUCT &t) {                                                  \
    return refl_object_to_tuple_impl(t);                                  \
  }

template <typename T>
constexpr auto test_to_tuple_impl() {
  auto& [member1, member2, member3] =
      Get_fake_object<remove_cvref_t<T>>();  // 假设结构体有三个成员
  auto ref_tup = std::tie(member1, member2, member3);
  auto get_ptrs = [](auto&... _refs) {
    return std::make_tuple(&_refs...);
  };
  return std::apply(get_ptrs, ref_tup);
}

template <auto ptr>
inline constexpr std::string_view test_member_name() {
#if defined(__GNUC__)
  constexpr std::string_view func_name = __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)
  constexpr std::string_view func_name = __FUNCSIG__;
#endif
  size_t first_colon = func_name.find("::");
  if (first_colon == std::string_view::npos) {
    return {};
  }

  size_t second_colon = func_name.find("::", first_colon + 2);
  if (second_colon == std::string_view::npos) {
    return {};
  }

  size_t end = func_name.find(')', second_colon);
  return func_name.substr(second_colon + 2, end - second_colon);
}

template <typename... Ts>
void print(const std::tuple<Ts...>& tu) {
  [&]<size_t... Is>(std::index_sequence<Is...>) mutable {
    (..., (std::cout << std::get<Is>(tu)));
  }(std::make_index_sequence<sizeof...(Ts)>{});
}

template <typename T>
inline constexpr std::array<std::string_view, member_count_v<T>>
test_member_names() {
  constexpr size_t Count = member_count_v<T>;
  std::array<std::string_view, Count> arr;
  constexpr auto tp = test_to_tuple_impl<T>();
  [&]<size_t... Is>(std::index_sequence<Is...>) mutable {
    ((arr[Is] = test_member_name<std::get<Is>(tp)>()), ...);
  }(std::make_index_sequence<Count>{});
  return arr;
}

template <typename T>
inline static decltype(auto) refl_object_to_tuple_impl(T &&t) {
  auto tp = get_private_ptrs(
      identity<std::remove_const_t<std::remove_reference_t<T>>>{});
  auto to_ref = [&t](auto... fields) {
    return std::tie(t.*fields...);
  };
  return std::apply(to_ref, tp);
}

struct person {
  int age;
  const char* name;
  bool sex;
};

class private_person {
private:
  int age;
  const char* name;
public:
  bool sex;
};
REFL_PRIVATE_(private_person, &private_person::age, &private_person::name, &private_person::sex)

int main() {
  //测试聚合类型的编译期反射
  auto arr = test_member_names<person>();
  for (auto name : arr) {
    std::cout << name << ", ";
  }
  std::cout << "\n";


  //测试非聚合类型的编译期反射
  constexpr auto tp = get_private_ptrs(identity<private_person>{});
  std::array<std::string_view, 3> arr2;
  [&]<size_t... Is>(std::index_sequence<Is...>) mutable {
    ((arr2[Is] = test_member_name<std::get<Is>(tp)>()), ...);
  }(std::make_index_sequence<3>{});
  for (auto name : arr2) {
    std::cout << name << ", ";
  }
  std::cout << "\n";

  return 0;
}