#include <array>
#include <cstddef>   // for std::size_t
#include <utility>   // for std::integer_sequence
#include <cstdio>    // for putchar

// Helper function to get the length of a constexpr string (null-terminated)
constexpr std::size_t constexpr_strlen(const char* str) {
    std::size_t len = 0;
    while (str[len] != '\0') {
        ++len;
    }
    return len;
}

// Helper to convert a number (e.g., line number) to a constexpr string
template<std::size_t N>
constexpr auto to_string(std::size_t num) {
    std::array<char, N> arr = {};
    for (std::size_t i = N - 1; i < N; --i) {
        arr[i] = '0' + (num % 10);
        num /= 10;
    }
    return arr;
}

// Helper to calculate the number of digits in a number
constexpr std::size_t num_digits(std::size_t num) {
    std::size_t digits = 1;
    while (num >= 10) {
        num /= 10;
        ++digits;
    }
    return digits;
}

// Overload for concatenating two arrays of different sizes
template <std::size_t N1, std::size_t N2>
constexpr std::array<char, N1 + N2 - 1> concat(const std::array<char, N1>& a, const std::array<char, N2>& b) {
    std::array<char, N1 + N2 - 1> result = {};
    for (std::size_t i = 0; i < N1 - 1; ++i) {
        result[i] = a[i];
    }
    for (std::size_t i = 0; i < N2; ++i) {
        result[N1 - 1 + i] = b[i];
    }
    return result;
}

// Create the constexpr FILE:LINE string
constexpr auto make_file_line_string() {
    constexpr auto file = std::array<char, constexpr_strlen(__FILE__) + 1>{__FILE__};
    constexpr auto line = to_string<num_digits(__LINE__)>(__LINE__);
    return concat(file, concat(std::array{':', '\0'}, line));
}

int main() {
    constexpr auto file_line = make_file_line_string();
    // Now we can use `file_line` as a constexpr string
    for (char c : file_line) {
        if (c == '\0') break;
        putchar(c);  // This prints the compiled-time constant: __FILE__:__LINE__
    }
    putchar('\n');
    return 0;
}
