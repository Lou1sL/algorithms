#pragma once

template<class T>
inline T Max(const T& a, const T& b){ return a > b ? a : b;  }

template<std::size_t PRECISION>
class Price {
private:
    int value;
public:
    Price() : value(0) { }
    Price(const int& val) : value(val * std::pow(10, PRECISION)) { }
    Price(const float& val) : value(val * std::pow(10, PRECISION)) { }
    Price(const Price& p) : value(p.value) { }
    Price(Price&& p) : value(p.value) { }

    Price<PRECISION>& operator=(const Price<PRECISION>& p) { this->value = p.value; return *this; }

    Price<PRECISION> operator+(const Price<PRECISION>& p) const { Price pp; pp.value = this->value + p.value; return pp; }
    Price<PRECISION> operator-(const Price<PRECISION>& p) const { Price pp; pp.value = this->value - p.value; return pp; }
    
    bool operator==(const Price<PRECISION> &p) const { return (this->value == p.value); };
    bool operator!=(const Price<PRECISION> &p) const { return (this->value != p.value); };
    bool operator< (const Price<PRECISION> &p) const { return (this->value <  p.value); };
    bool operator<=(const Price<PRECISION> &p) const { return (this->value <= p.value); };
    bool operator> (const Price<PRECISION> &p) const { return (this->value >  p.value); };
    bool operator>=(const Price<PRECISION> &p) const { return (this->value >= p.value); };

    template<std::size_t P>
    friend std::ostream& operator<<(std::ostream& os, const Price<P>& p);

    static Price min() { Price pp; pp.value = std::numeric_limits<int>::min() + 1; return pp; }
    static Price max() { Price pp; pp.value = std::numeric_limits<int>::max() - 1; return pp; }

    ~Price() { }
};

template<std::size_t P>
std::ostream& operator<<(std::ostream& os, const Price<P>& p) {
    os << (p.value / static_cast<int>(std::pow(10, P))) << '.' << (p.value % static_cast<int>(std::pow(10, P)));
    return os;
}

//Top-down method, bad practice, costs T(2^n) and n is rod length.
template<std::size_t LEN>
Price<2> CutRod(const std::array<Price<2>, LEN>& arr, std::size_t rod_len){
    assert(("Rod length should shorter than price array len.", (rod_len < LEN)));
    if((LEN == 0) || (rod_len == 0)) return 0;
    Price<2> q = arr[0];
    for(std::size_t i=0; i<rod_len; i++){
        q = Max<Price<2>>(q, arr[i] + CutRod(arr, rod_len-i-1));
    }
    return q;
}

//Memoized top-down method, better practice, O(n^2).
template<std::size_t LEN>
Price<2> MemoizedCutRodAux(const std::array<Price<2>, LEN>& arr, std::size_t rod_len, std::array<Price<2>, LEN>& memo){
    if(rod_len == 0) return 0;
    //If the max price of rod_len is already in the memo then there's no need to recalculate it.
    if(memo[rod_len] >= 0) return memo[rod_len];
    Price<2> q = Price<2>::min();
    for(std::size_t i=0; i<rod_len; i++){
        q = Max<Price<2>>(q, arr[i] + MemoizedCutRodAux(arr, rod_len-i-1, memo));
    }
    //Save the current result into memo.
    memo[rod_len] = q;
    return q;
}
template<std::size_t LEN>
Price<2> MemoizedCutRod(const std::array<Price<2>, LEN>& arr, std::size_t rod_len){
    assert(("Rod length should shorter than price array len.", (rod_len < LEN)));
    if((LEN == 0) || (rod_len == 0)) return 0;
    //Only used rod_len size + 1 of memo array, but since rod_len is not a compile time value and it's smaller than LEN so...
    std::array<Price<2>, LEN> memo { 0 };
    for(std::size_t i=0; i<=rod_len; i++){ memo[i] = Price<2>::min(); }
    return MemoizedCutRodAux(arr, rod_len, memo);
}

//Bottom-up method, best practice, O(n^2).
//Because each step of result relies on the previous step, so we simply calculate from the start till the answer we need.
//Kinda like Sieve of Eratosthenes algorithm way of calculating primes.
template<std::size_t LEN>
Price<2> BottomUpCutRod(const std::array<Price<2>, LEN>& arr, std::size_t rod_len){
    assert(("Rod length should shorter than price array len.", (rod_len < LEN)));
    if((LEN == 0) || (rod_len == 0)) return 0;
    //Only used rod_len size + 1 of memo array, but since rod_len is not a compile time value and it's smaller than LEN so...
    std::array<Price<2>, LEN> memo { 0 };
    for(std::size_t j=1; j<=rod_len; j++){
        Price<2> q = Price<2>::min();
        for(std::size_t i=1; i<=j; i++){
            q = Max<Price<2>>(q, arr[i-1] + memo[j-i]);
        }
        memo[j] = q;
    }
    return memo[rod_len];
}

static int dpcr_test = push_test("DP Cut Rod", (test_function)[](){
    std::array<Price<2>, 10> price_array = { 1, 5, 8, 9, 10, 17, 17, 20, 24, 30 };
    std::cout << CutRod(price_array, 5) << std::endl;
    std::cout << MemoizedCutRod(price_array, 5) << std::endl;
    std::cout << BottomUpCutRod(price_array, 5) << std::endl;
    return TEST_SUCCESS;
});