#pragma once

#include "node.h"

template<class T> std::size_t HashData(const T& dat);

template<> std::size_t HashData(const int& dat){ return dat % 701; }

template<class KeyT, class ValT>
class HashTableElementContent {
public:
    KeyT key;
    ValT val;
};


using HashTableEle = DoubleLinkNode<HashTableElementContent<>>;

template<class EleType, std::size_t SIZE>
class HashTable {
private:
    std::array<HashTableEle, SIZE> table;

public:
    HashTable() { }
    HashTable(const HashTable<EleType, SIZE>& table) { }
    HashTable(HashTable<EleType, SIZE>&& table) { }
};

static int ht_test = push_test("Hash Table", (test_function)[](){

    return TEST_SUCCESS;
});
