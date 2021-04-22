#pragma once

template<class T, std::size_t XLENGTH, std::size_t YLENGTH>
using Matrix = std::array<std::array<T, YLENGTH>, XLENGTH>;

//Largest common subsequence
template<class T, std::size_t XLEN, std::size_t YLEN>
auto LCSLengthMatrix(const std::array<T, XLEN>& x, const std::array<T, YLEN>& y) -> Matrix<std::size_t, XLEN+1, YLEN+1> {
    Matrix<std::size_t, XLEN+1, YLEN+1> mat;
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

template<class T, class DescT, std::size_t XLEN, std::size_t YLEN>
void LCSPrintMatrix(const Matrix<T, XLEN, YLEN>& mat, const std::array<DescT, XLEN-1>& xdesc, const std::array<DescT, YLEN-1>& ydesc){
    if((XLEN == 0) || (YLEN == 0)) return;
    for(std::size_t i=0; i<XLEN; i++){
        for(std::size_t j=0; j<YLEN; j++){
            if((i==0) && (j==0)) std::cout << " " << " ";
            else if(i==0) std::cout << ydesc[j-1] << " ";
            else if(j==0) std::cout << xdesc[i-1] << " ";
            else std::cout << mat[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

template<class T, std::size_t XLEN, std::size_t YLEN>
void LCSFindLongest(const Matrix<T, XLEN, YLEN>& mat, const std::array<DescT, XLEN-1>& x, const std::array<DescT, YLEN-1>& y){
    if((XLEN == 0) || (YLEN == 0)) return;
    for(std::size_t i=0; i<XLEN; i++){
        for(std::size_t j=0; j<YLEN; j++){
        }
    }
}

static int dplcs_test = push_test("DP LCS", (test_function)[](){

    auto x = std::array<char, 7> { 'A', 'B', 'C', 'B', 'D', 'A', 'B' };
    auto y = std::array<char, 6> { 'B', 'D', 'C', 'A', 'B', 'A' };

    auto resMat = LCSLengthMatrix(x, y);
    PrintMatrix(resMat, x, y);

    return TEST_SUCCESS;
});