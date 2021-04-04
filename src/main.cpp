#include <iostream>
#include <array>
#include <vector>
#include <tuple>
#include <string>
#include <functional>

enum TEST_RESULT { TEST_SUCCESS, TEST_FAILED };
using test_case = std::tuple<std::string, std::function<TEST_RESULT(void)>>;
static std::vector<test_case> Tests;

#include "binary_search_tree.h"
#include "sort_insertion.h"
#include "sort_merge.h"

int main(){
	//test_insertion_sort();
	test_bst();
	return 0;
}