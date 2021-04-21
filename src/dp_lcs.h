#pragma once

template<class T, std::size_t XLEN, std::size_t YLEN>
auto LCSLength(const std::array<T, XLEN>& x, const std::array<T, YLEN>& y) -> std::array<std::array<T, XLEN>, YLEN> {
    std::array<std::array<T, XLEN>, YLEN> c;

    return c;
}




static int dplcs_test = push_test("DP LCS", (test_function)[](){

    auto x = std::array<char, 8> { "ABCBDAB" };
    auto y = std::array<char, 7> { "BDCABA" };

    auto resMat = LCSLength(x, y);

    return TEST_SUCCESS;
});