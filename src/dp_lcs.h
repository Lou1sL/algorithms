#pragma once

namespace dp_lcs {

template<class T, std::size_t XLENGTH, std::size_t YLENGTH>
using Matrix = std::array<std::array<T, YLENGTH>, XLENGTH>;

//Largest common subsequence
template<class T, std::size_t XLEN, std::size_t YLEN>
auto LCSMatrix(const std::array<T, XLEN>& x, const std::array<T, YLEN>& y) -> Matrix<std::size_t, XLEN+1, YLEN+1> {
    Matrix<std::size_t, XLEN+1, YLEN+1> mat;
    if((XLEN == 0) || (YLEN == 0)) return mat;
    for(std::size_t i=0; i<=XLEN; i++) mat[i] = { 0 };

    for(std::size_t i=1; i<=XLEN; i++){
        for(std::size_t j=1; j<=YLEN; j++){
            std::size_t& c = mat[i][j], l = mat[i-1][j], u = mat[i][j-1], lu = mat[i-1][j-1];
            const T& xval = x[i-1], yval = y[j-1];
            if(xval == yval) c = lu + 1;
            else if(l >= u) c = l;
            else c = u;
        }
    }
    return mat;
}

template<class MatT, class ArrT, std::size_t XLEN, std::size_t YLEN>
void LCSMatrixPrint(const Matrix<MatT, XLEN, YLEN>& mat, const std::array<ArrT, XLEN-1>& xdesc, const std::array<ArrT, YLEN-1>& ydesc){
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

template<class MatT, class ArrT, std::size_t XLEN, std::size_t YLEN>
auto LCSFind(
    const Matrix<MatT, XLEN, YLEN>& mat,
    const std::array<ArrT, XLEN-1>& x,
    const std::array<ArrT, YLEN-1>& y) -> std::deque<ArrT> {
    std::deque<ArrT> rtn;
    if((XLEN == 0) || (YLEN == 0)) return rtn;

    std::size_t i = XLEN, j = YLEN;
    while(true){
        if((i==0) || (j==0)) break;
        MatT c = mat[i][j], l = mat[i-1][j], u = mat[i][j-1], lu = mat[i-1][j-1];
        ArrT xval = x[i-1], yval = y[j-1];
        if(xval == yval) { rtn.push_front(xval); i--; j--; }
        else if(l > u) { i--; }
        else if(l < u) { j--; }
        else { i--;/** Or j--, whatever, because when l == u it means there are multiple LCS. */ }
    }
    return rtn;
}

static int dplcs_test = push_test("DP LCS", (test_function)[](){

    auto x = std::array<char, 7> { 'A', 'B', 'C', 'B', 'D', 'A', 'B' };
    auto y = std::array<char, 6> { 'B', 'D', 'C', 'A', 'B', 'A' };

    auto resMat = LCSMatrix(x, y);
    LCSMatrixPrint(resMat, x, y);
    auto lcs = LCSFind(resMat, x, y);
    for (auto i = lcs.begin(); i != lcs.end(); ++i) std::cout << *i << ' ';
    std::cout << std::endl;

    return TEST_SUCCESS;
});

}
