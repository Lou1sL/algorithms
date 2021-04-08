
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
void Merge(std::array<T, LEN>& arr, std::size_t p, std::size_t q, std::size_t r){
    
}
template<class T, std::size_t LEN>
void MergeSort(std::array<T, LEN>& arr){
}

static int s_test = push_test("Sort", (test_function)[](){ 
    
    auto test_in = std::array<int, 6> { 5, 4, 3, 2, 1, 0 };
    auto expected_out = std::array<int, 6> { 0, 1, 2, 3, 4, 5 };

    InsertionSort<int, 6>(test_in);
    bool failed = false;
    for(int i=0; i<6; i++) if(test_in[i] != expected_out[i]) failed = true;

    return failed;
});