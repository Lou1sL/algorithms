
template<class T, std::size_t LEN>
void InsertionSort(std::array<T, LEN>& arr){
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
    std::size_t lsize = m - l + 1;
    std::size_t rsize = r - m;

    T larr[lsize], rarr[rsize];
    
    //Copy l to m to larr and m to r to rarr
    for(std::size_t i = 0; i < lsize; i++) larr[i] = arr[l + i];
    for(std::size_t j = 0; j < rsize; j++) rarr[j] = arr[m + j + 1];
 
    std::size_t i = 0, j = 0, k = l;
 
    while (i < lsize && j < rsize) {
        if (larr[i] <= rarr[j]) { arr[k] = larr[i]; i++; }
        else { arr[k] = rarr[j]; j++; }
        k++;
    }
    while (i < lsize) { arr[k] = larr[i]; i++; k++; }
    while (j < rsize) { arr[k] = rarr[j]; j++; k++; }
}
template<class T, std::size_t LEN>
void MergeSortRecursion(std::array<T, LEN>& arr, std::size_t l, std::size_t r){
    if(l >= r) return;
    std::size_t m =l+ (r-l)/2;
    MergeSortRecursion(arr, l, m);
    MergeSortRecursion(arr, m+1, r);
    Merge(arr, l, m, r);
}
template<class T, std::size_t LEN>
void MergeSort(std::array<T, LEN>& arr){
    MergeSortRecursion(arr, 0, LEN-1);
}

static int s_test = push_test("Sort", (test_function)[](){ 
    
    auto test_input = std::array<int, 6> { 5, 4, 3, 2, 1, 0 };
    auto expected_output = std::array<int, 6> { 0, 1, 2, 3, 4, 5 };

    auto test_i = test_input;
    InsertionSort<int, 6>(test_i);
    if(test_i != expected_output) return TEST_FAILED;

    auto test_m = test_input;
    MergeSort<int, 6>(test_m);
    if(test_m != expected_output) return TEST_FAILED;

    return TEST_SUCCESS;
});