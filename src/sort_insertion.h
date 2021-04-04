
template<int Alength>
void insertion_sort(std::array<int, Alength>& A){
    for(std::size_t j = 1; j < Alength; j++){
        // { 5, 4, 3, 2, 1, 0 }
        //      ↑ key = 4, as a temp
        int key = A[j];
        // { 5, 4, 3, 2, 1, 0 }
        //   ↑ Index i = 0
        int i = j - 1; //Use int because std::size_t can't be negative!
        // { 5, 4, 3, 2, 1, 0 }
        //      ← If keeps finding larger nums than the key then keep shifting right & it will overwirte key & so the left most shifted is duplicated & be like
        // { 5, 5, 3, 2, 1, 0 }
        while((i >= 0) && (A[i] > key)){
            A[i + 1] = A[i];
            i = i - 1; //Use int because std::size_t can't be negative!
        }
        //Write temp on to the duplicated
        // { 3, 5, 4, 2, 1, 0 }
        A[i + 1] = key;
    }
}

void test_insertion_sort(){
    std::array<int, 6> test = std::array<int, 6> { 5, 4, 3, 2, 1, 0 };
    insertion_sort<6>(test);
    for(int ele:test) std::cout << ele << " ";
    std::cout << std::endl;
}