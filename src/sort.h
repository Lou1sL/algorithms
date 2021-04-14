#pragma once

//Arguments are lvalue references so Copy-assignment is triggered.
template<class T>
inline void Exchange(T& a, T& b){ T temp = a; a = b; b = temp; }

/**
 * Comparison Based Sorting
 */

template<class T, std::size_t LEN>
void BubbleSort(std::array<T, LEN>& arr){
    if constexpr(LEN <= 1) return;
    bool swapped;
    for(std::size_t i = 0; i < LEN-1; i++) {
        swapped = false;
        //Variable j pushes one biggest element gradually to the right during loop, hence 'bubble'.
        //After completion, the right most element would be the largest.
        //So there's no reason to push beyond that, hence j < LEN - i - 1
        for(std::size_t j = 0; j < LEN-i-1; j++){
            if(arr[j] > arr[j+1]) {
                Exchange<T>(arr[j], arr[j+1]);
                swapped = true;
            }
        }
        //If there was no swapping happend at all, which means the array has been sorted, then no reason to continue.
        if(swapped == false) break;
   }
}

template<class T, std::size_t LEN>
void InsertionSort(std::array<T, LEN>& arr){
    if constexpr(LEN <= 1) return;
    for(std::size_t j = 1; j < LEN; j++){
        // { 5, 4, 3, 2, 1, 0 }
        //      ↑ key = 4, as a temp
        T key = arr[j];
        // { 5, 4, 3, 2, 1, 0 }
        //   ↑ Index i = 0
        int i = j - 1; //Use int because std::size_t can't be negative!
        // { 5, 4, 3, 2, 1, 0 }
        //      ← If keeps finding larger nums than the key then keep shifting right & it will overwirte key & so the left most shifted is duplicated & be like
        // { 5, 5, 3, 2, 1, 0 }
        while((i >= 0) && (arr[i] > key)){
            arr[i + 1] = arr[i];
            i = i - 1; //Use int because std::size_t can't be negative!
        }
        //Write temp on to the duplicated
        // { 3, 5, 4, 2, 1, 0 }
        arr[i + 1] = key;
    }
}

template<class T, std::size_t LEN>
void Merge(std::array<T, LEN>& arr, std::size_t l, std::size_t m, std::size_t r){
    std::size_t lSize = m - l + 1;
    std::size_t rSize = r - m;

    T larr[lSize], rarr[rSize];
    
    //Copy l to m to larr and m to r to rarr
    for(std::size_t i = 0; i < lSize; i++) larr[i] = arr[l + i];
    for(std::size_t j = 0; j < rSize; j++) rarr[j] = arr[m + j + 1];
 
    std::size_t lIndex = 0, rIndex = 0, arrIndex = l;
 
    while (lIndex < lSize && rIndex < rSize) {
        if (larr[lIndex] <= rarr[rIndex])
            { arr[arrIndex] = larr[lIndex]; lIndex++; }
        else
            { arr[arrIndex] = rarr[rIndex]; rIndex++; }
        arrIndex++;
    }
    while (lIndex < lSize) { arr[arrIndex] = larr[lIndex]; lIndex++; arrIndex++; }
    while (rIndex < rSize) { arr[arrIndex] = rarr[rIndex]; rIndex++; arrIndex++; }
}
template<class T, std::size_t LEN>
void MergeSortRecursion(std::array<T, LEN>& arr, std::size_t l, std::size_t r){
    if(l >= r) return;
    std::size_t m =l+ (r-l)/2;
    MergeSortRecursion<T, LEN>(arr, l, m);
    MergeSortRecursion<T, LEN>(arr, m+1, r);
    Merge<T, LEN>(arr, l, m, r);
}
template<class T, std::size_t LEN>
void MergeSort(std::array<T, LEN>& arr){
    if constexpr(LEN <= 1) return;
    MergeSortRecursion<T, LEN>(arr, 0, LEN-1);
}

template<class T, std::size_t LEN>
int Partition(std::array<T, LEN>& arr, int l, int r){
    T pivot = arr[r];
    int i = l;
    for(int j = l; j <= r-1; j++){
        if(arr[j] <= pivot){
            Exchange(arr[i], arr[j]);
            i++;
        }
    }
    Exchange(arr[i], arr[r]);
    return i;
}
template<class T, std::size_t LEN>
void QuickSortRecursion(std::array<T, LEN>& arr, int l, int r){
    if (l >= r) return;
    int m = Partition<T, LEN>(arr, l, r);
    //[l <-> ... <-> m - 1]<-> m <-> m + 1 <-> ... <-> r
    QuickSortRecursion<T, LEN>(arr, l, m-1);
    // l <-> ... <-> m - 1 <-> m <->[m + 1 <-> ... <-> r]
    QuickSortRecursion<T, LEN>(arr, m+1, r);
}
template<class T, std::size_t LEN>
void QuickSort(std::array<T, LEN>& arr){
    if constexpr(LEN <= 1) return;
    QuickSortRecursion<T, LEN>(arr, 0, LEN - 1);
}

/**
 * Non-comparison Based Sorting
 */


using SORT_TEST_TYPE = float;
constexpr std::size_t SORT_TEST_SIZE = 50;

template<class T, std::size_t LEN>
std::vector<T> create_random_data(){
    std::random_device r;
    std::seed_seq seed { r(), r(), r(), r(), r(), r(), r(), r() };
    std::mt19937 eng(seed);
    std::uniform_int_distribution<int> dist;
    std::vector<T> v(LEN);
    std::generate(begin(v), end(v), bind(dist, eng));
    return v;
}

static int s_test = push_test("Sort", (test_function)[](){ 
    
    auto rand_data = create_random_data<SORT_TEST_TYPE, SORT_TEST_SIZE>();
    auto test_input = std::array<SORT_TEST_TYPE, SORT_TEST_SIZE> { 0 };
    std::copy(rand_data.begin(), rand_data.end(), test_input.begin());

    auto expected_output = test_input;
    std::sort(expected_output.begin(), expected_output.end());

    auto test_i = test_input;
    InsertionSort<SORT_TEST_TYPE, SORT_TEST_SIZE>(test_i);
    if(test_i != expected_output) return TEST_FAILED;

    auto test_m = test_input;
    MergeSort<SORT_TEST_TYPE, SORT_TEST_SIZE>(test_m);
    if(test_m != expected_output) return TEST_FAILED;

    auto test_b = test_input;
    BubbleSort<SORT_TEST_TYPE, SORT_TEST_SIZE>(test_b);
    if(test_b != expected_output) return TEST_FAILED;

    auto test_q = test_input;
    QuickSort<SORT_TEST_TYPE, SORT_TEST_SIZE>(test_q);
    if(test_q != expected_output) return TEST_FAILED;

    return TEST_SUCCESS;
});