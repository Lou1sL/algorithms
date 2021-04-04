
// How to add a test case:
// static int var = push_test("Test Name Here", (test_function)[](){ /** Test Code Here */ return 0; });

#include <iostream>
#include <array>
#include <vector>
#include <tuple>
#include <string>
#include <functional>

using test_function = std::function<int(void)>;
using test_case = std::tuple<std::string, test_function>;
constexpr int MAX_TEST_CASE = 100;
static int test_count = 0;
static test_case nullcase = std::make_tuple("", nullptr);
static std::array<test_case, MAX_TEST_CASE> test_set = { nullcase };

static int push_test(test_case c) {
	if(test_count == MAX_TEST_CASE) return 1;
	test_set[test_count] = c;
	test_count++;
	return 0;
}
static int push_test(std::string name, test_function func) {
	return push_test(std::make_tuple(name, func));
}
static int push_test(const char* name, test_function func) {
	return push_test(std::make_tuple(std::string(name), func));
}

#include "include_test.h"

int main(){
	std::vector<std::string> failed_case;
	for(test_case ele : test_set) {
		if(std::get<1>(ele) == nullptr) continue;
		std::cout << "[" << std::get<0>(ele) << "]" << std::endl;
		int rtn = std::get<1>(ele)();
		std::cout << "[" << rtn << "]" << std::endl;
		if(rtn != 0) failed_case.push_back(std::get<0>(ele));
	}
	for(std::string ele : failed_case) {
		std::cout << "FAILED : [" << ele << "]" << std::endl;
	}
	return 0;
}
