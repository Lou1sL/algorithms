#pragma once

template<class T, std::size_t LEN>
auto FindMaxCrossingSubarray(const std::array<T,LEN>& arr, std::size_t low, std::size_t mid, std::size_t high) -> std::tuple<std::size_t, std::size_t, T> {

    std::size_t maxLeft = 0;
    T leftSum = std::numeric_limits<T>::min() + 1;
    T sum = 0;
    for(std::size_t i = mid; i>=low; i--){
        sum += arr[i];
        if(sum > leftSum){
            leftSum = sum;
            maxLeft = i;
        }
        if(i==0) break;
    }

    std::size_t maxRight = 0;
    T rightSum = std::numeric_limits<T>::min() + 1;
    sum = 0;
    for(std::size_t j = mid+1; j<=high; j++){
        sum += arr[j];
        if(sum > rightSum){
            rightSum = sum;
            maxRight = j;
        }
    }

    return std::tuple<std::size_t, std::size_t, T>(maxLeft, maxRight, leftSum + rightSum);
}

template<class T, std::size_t LEN>
auto FindMaximumSubarray(const std::array<T,LEN>& arr, std::size_t low, std::size_t high) -> std::tuple<std::size_t, std::size_t, T> {
    if(high == low) return std::tuple<std::size_t, std::size_t, T>(low, high, arr[low]);
    std::tuple<std::size_t, std::size_t, T> left, right, cross;
    std::size_t mid = (low + high) / 2;
    left = FindMaximumSubarray(arr, low, mid);
    right = FindMaximumSubarray(arr, mid + 1, high);
    cross = FindMaxCrossingSubarray(arr, low, mid, high);

    if((std::get<2>(left) >= std::get<2>(right)) && ((std::get<2>(left) >= std::get<2>(cross)))) return left;
    else if((std::get<2>(right) >= std::get<2>(left)) && ((std::get<2>(right) >= std::get<2>(cross)))) return right;
    else return cross;

}

template<class T, std::size_t LEN>
auto FindMaximumSubarray(const std::array<T,LEN>& arr) -> std::tuple<std::size_t, std::size_t, T> {
    return FindMaximumSubarray<T, LEN>(arr, 0, LEN - 1);
}

static int mcs_test = push_test("Max Subarray", (test_function)[](){

    auto test_input = std::array<int, 5>{ -1, 51, -50, 999, 3 };
    auto expected_output = std::tuple<std::size_t, std::size_t, int>(1, 4, 1003);

    auto result = FindMaximumSubarray<int, 5>(test_input);
    std::cout << "MIN IDX: " << std::get<0>(result) << " MAX IDX: " << std::get<1>(result) << " SUM: " << std::get<2>(result) << std::endl;
    if(result == expected_output) return TEST_SUCCESS;
    return TEST_FAILED;
});