#pragma once

namespace dp_prime {

constexpr std::size_t GEN_MAX = 100;

//Find primes by using Sieve of Eratosthenes algorithm
constexpr auto PRIME_GEN() -> std::array<bool, GEN_MAX>{
    std::array<bool, GEN_MAX> rtn = { };
    rtn[0] = false; rtn[1] = false; rtn[2] = true; rtn[3] = true;
    for(std::size_t i=4; i<GEN_MAX; i++) rtn[i] = true;
    for(std::size_t i=2; i<GEN_MAX; i++){
        std::size_t j = i + i;
        while(j<GEN_MAX){ rtn[j] = false; j += i; }
    }
    return rtn;
}
std::array<bool, GEN_MAX> PRIME_LIST = PRIME_GEN();


void printSmallerPrimes(int from, int to){
    if((to >= GEN_MAX) || (from < 0) || (from > to)) return;
    for(int i=from; i<to; i++) if(PRIME_LIST[i]) std::cout << i << " ";
    std::cout << std::endl;
}

static int soe_test = push_test("Sieve of Eratosthenes", (test_function)[](){
    printSmallerPrimes(50, 80);
    return TEST_SUCCESS;
});

}
