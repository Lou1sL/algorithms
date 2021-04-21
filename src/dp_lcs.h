#pragma once

//Largest common subsequence
template<class T, std::size_t XLEN, std::size_t YLEN>
auto LCSLengthMatrix(const std::array<T, XLEN>& x, const std::array<T, YLEN>& y) -> std::array<std::array<std::size_t, YLEN+1>, XLEN+1> {
    std::array<std::array<std::size_t, YLEN+1>, XLEN+1> mat;
    if((XLEN == 0) || (YLEN == 0)) return mat;
    for(std::size_t i=0; i<=XLEN; i++) mat[i] = { 0 };
    for(std::size_t i=1; i<=XLEN; i++){
        for(std::size_t j=1; j<=YLEN; j++){
            if(x[i-1] == y[j-1]) mat[i][j] = mat[i-1][j-1] + 1;
            else if(mat[i-1][j] >= mat[i][j-1]) mat[i][j] = mat[i-1][j];
            else mat[i][j] = mat[i][j-1];
        }
    }
    return mat;
}

template<class T, std::size_t XLEN, std::size_t YLEN>
void PrintMatrix(const std::array<std::array<T, XLEN>, YLEN>& mat){
    if((XLEN == 0) || (YLEN == 0)) return;
    for(std::size_t i=0; i<YLEN; i++){
        for(std::size_t j=0; j<XLEN; j++){
            std::cout << mat[i][j] << " ";
        }
        std::cout << std::endl;
    }
}


static int dplcs_test = push_test("DP LCS", (test_function)[](){

    auto x = std::array<char, 7> { 'A', 'B', 'C', 'B', 'D', 'A', 'B' };
    auto y = std::array<char, 6> { 'B', 'D', 'C', 'A', 'B', 'A' };

    auto resMat = LCSLengthMatrix(x, y);
    PrintMatrix(resMat);

    return TEST_SUCCESS;
});