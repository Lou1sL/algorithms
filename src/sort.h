#pragma once

//Arguments are lvalue references so Copy-assignment is triggered.
template<class T>
inline void Exchange(T& a, T& b){ T temp = a; a = b; b = temp; }

// ┌──────────────────────────────────────┐
// │ Selection Sort                       │
// ├────────────┬────────────┬────────────┤
// │ Best       │ Average    │ Worst      │
// ├────────────┼────────────┼────────────┤
// │ θ(n^2)     | θ(n^2)     | O(n^2)     |
// └────────────┴────────────┴────────────┘
// Comparison Based
template<class T, std::size_t LEN>
void SelectionSort(std::array<T, LEN>& arr){
    if constexpr(LEN <= 1) return;
    for(std::size_t i=0; i<LEN-1; i++)
        for(std::size_t j=i; j<LEN; j++)
            if(arr[i] > arr[j])
                Exchange(arr[i], arr[j]);
}

// ┌──────────────────────────────────────┐
// │ Bubble Sort                          │
// ├────────────┬────────────┬────────────┤
// │ Best       │ Average    │ Worst      │
// ├────────────┼────────────┼────────────┤
// │ Ω(n)       | θ(n^2)     | O(n^2)     |
// └────────────┴────────────┴────────────┘
// Comparison Based
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

// ┌──────────────────────────────────────┐
// │ Insertion Sort                       │
// ├────────────┬────────────┬────────────┤
// │ Best       │ Average    │ Worst      │
// ├────────────┼────────────┼────────────┤
// │ Ω(n)       | θ(n^2)     | O(n^2)     |
// └────────────┴────────────┴────────────┘
// Comparison Based
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

// ┌──────────────────────────────────────┐
// │ Merge Sort                           │
// ├────────────┬────────────┬────────────┤
// │ Best       │ Average    │ Worst      │
// ├────────────┼────────────┼────────────┤
// │ Ω(nlog(n)) | θ(nlog(n)) | O(nlog(n)) |
// └────────────┴────────────┴────────────┘
// Comparison Based
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

// ┌──────────────────────────────────────┐
// │ Quick Sort                           │
// ├────────────┬────────────┬────────────┤
// │ Best       │ Average    │ Worst      │
// ├────────────┼────────────┼────────────┤
// │ Ω(nlog(n)) | θ(nlog(n)) | O(n^2)     |
// └────────────┴────────────┴────────────┘
// Comparison Based
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

// ┌──────────────────────────────────────┐
// │ Heap Sort                            │
// ├────────────┬────────────┬────────────┤
// │ Best       │ Average    │ Worst      │
// ├────────────┼────────────┼────────────┤
// │ Ω(nlog(n)) | θ(nlog(n)) | O(nlog(n)) |
// └────────────┴────────────┴────────────┘
// Comparison Based
inline std::size_t HeapParent     (std::size_t i) { return i >> 1;   }
inline std::size_t HeapLeftChild  (std::size_t i) { return i << 1;   }
inline std::size_t HeapRightChild (std::size_t i) { return ((i << 1) | 1); }
template<class T, std::size_t LEN>
void MaxHeapify(std::array<T, LEN>& arr, std::size_t i, std::size_t heap_size){
    std::size_t l = HeapLeftChild(i);
    std::size_t r = HeapRightChild(i);

    std::size_t largest;
    if((l < heap_size) && (arr[l] > arr[i])) largest = l;
    else largest = i;
    if((r < heap_size) && (arr[r] > arr[largest])) largest = r;
    if(largest != i){
        Exchange(arr[i], arr[largest]);
        MaxHeapify<T, LEN>(arr, largest, heap_size);
    }
}
template<class T, std::size_t LEN>
void BuildMaxHeap(std::array<T, LEN>& arr){
    for(std::size_t i = LEN/2; i>=0; i--) {
        MaxHeapify<T, LEN>(arr, i, LEN);
        if(i==0) break;
    }
}
template<class T, std::size_t LEN>
void HeapSort(std::array<T, LEN>& arr){
    if constexpr(LEN <= 1) return;
    BuildMaxHeap<T, LEN>(arr);
    std::size_t heapSize = LEN;
    for(std::size_t i = LEN - 1; i>=1; i--) {
        heapSize--;
        Exchange(arr[0], arr[i]);
        MaxHeapify<T, LEN>(arr, 0, heapSize);
    }
}

// ┌──────────────────────────────────────┐
// │ Bucket Sort                          │
// ├────────────┬────────────┬────────────┤
// │ Best       │ Average    │ Worst      │
// ├────────────┼────────────┼────────────┤
// │ Ω(n+k)     | θ(n+k)     | O(n^2)     |
// └────────────┴────────────┴────────────┘
// Non-comparison Based
constexpr std::size_t BUCKET_SIZE = 10;
template<class T, std::size_t LEN>
void BucketSort(std::array<T, LEN>& arr){
    if constexpr(LEN <= 1) return;
    static_assert(std::is_integral<T>::value, "Integral array required for BucketSort!");
    std::vector<T> bucket[BUCKET_SIZE];
    T bucketDivider = std::numeric_limits<T>::max() / BUCKET_SIZE;
    for(std::size_t i=0; i<LEN; i++)
        if(arr[i] > 0) bucket[arr[i] / bucketDivider].push_back(arr[i]);
        else bucket[0].push_back(arr[i]);
    for(std::size_t i=0; i<BUCKET_SIZE; i++)
        std::sort(bucket[i].begin(), bucket[i].end());
    std::size_t index = 0;
    for(std::size_t i=0; i<BUCKET_SIZE; i++)
        for(std::size_t j=0; j<bucket[i].size(); j++)
            arr[index++] = bucket[i][j];
}

// ┌──────────────────────────────────────┐
// │ Radix Sort                           │
// ├────────────┬────────────┬────────────┤
// │ Best       │ Average    │ Worst      │
// ├────────────┼────────────┼────────────┤
// │ Ω(nk)      | θ(nk)      | O(nk)      |
// └────────────┴────────────┴────────────┘
// Non-comparison Based
template<class T, std::size_t LEN>
void RadixSort(std::array<T, LEN>& arr){
    
}

template<class T, std::size_t LEN>
std::vector<T> genRandomArr(){
    std::random_device r;
    std::seed_seq seed { r(), r(), r(), r(), r(), r(), r(), r() };
    std::mt19937 eng(seed);
    std::uniform_int_distribution<int> dist;
    std::vector<T> v(LEN);
    std::generate(begin(v), end(v), bind(dist, eng));
    return v;
}

template <class T, std::size_t LEN>
TEST_RESULT SortTest(std::string&& name, void (*func)(std::array<T, LEN>&), const std::array<T, LEN>& input, const std::array<T, LEN>& expected, bool print = false){
    auto test_i = input;
    auto start = std::chrono::steady_clock::now();
    func(test_i);
    auto end = std::chrono::steady_clock::now();
    std::cout << name << ": " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " ns. ";
    if(print) for(auto ele : test_i) std::cout << "->" << ele;
    if(test_i != expected) {
        std::cout << " Failed!" << std::endl;
        return TEST_FAILED;
    }
    std::cout << " Success!" << std::endl;
    return TEST_SUCCESS;
}

using SORT_TEST_TYPE = int;
constexpr std::size_t SORT_TEST_SIZE = 5;

static int s_test = push_test("Sort", (test_function)[](){ 
    
    auto rand_data = genRandomArr<SORT_TEST_TYPE, SORT_TEST_SIZE>();
    auto test_input = std::array<SORT_TEST_TYPE, SORT_TEST_SIZE> { 0 };
    std::copy(rand_data.begin(), rand_data.end(), test_input.begin());

    auto expected_output = test_input;
    std::sort(expected_output.begin(), expected_output.end());

    SortTest<SORT_TEST_TYPE, SORT_TEST_SIZE>("C++ Sort", [](std::array<SORT_TEST_TYPE, SORT_TEST_SIZE>& arr){ std::sort(arr.begin(), arr.end()); }, test_input, expected_output);
    SortTest<SORT_TEST_TYPE, SORT_TEST_SIZE>("Selection Sort", &SelectionSort<SORT_TEST_TYPE, SORT_TEST_SIZE>, test_input, expected_output);
    SortTest<SORT_TEST_TYPE, SORT_TEST_SIZE>("Insertion Sort", &InsertionSort<SORT_TEST_TYPE, SORT_TEST_SIZE>, test_input, expected_output);
    SortTest<SORT_TEST_TYPE, SORT_TEST_SIZE>("Merge Sort", &MergeSort<SORT_TEST_TYPE, SORT_TEST_SIZE>, test_input, expected_output);
    SortTest<SORT_TEST_TYPE, SORT_TEST_SIZE>("Bubble Sort", &BubbleSort<SORT_TEST_TYPE, SORT_TEST_SIZE>, test_input, expected_output);
    SortTest<SORT_TEST_TYPE, SORT_TEST_SIZE>("Quick Sort", &QuickSort<SORT_TEST_TYPE, SORT_TEST_SIZE>, test_input, expected_output);
    SortTest<SORT_TEST_TYPE, SORT_TEST_SIZE>("Heap Sort", &HeapSort<SORT_TEST_TYPE, SORT_TEST_SIZE>, test_input, expected_output);
    SortTest<SORT_TEST_TYPE, SORT_TEST_SIZE>("Bucket Sort", &BucketSort<SORT_TEST_TYPE, SORT_TEST_SIZE>, test_input, expected_output);
    return TEST_SUCCESS;
});