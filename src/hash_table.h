#pragma once

#include "linked_list.h"

template<class T> struct HashCalc { };
template<> struct HashCalc<int> {
    static std::size_t calculate(const int& data){
        return std::hash<int>()(data);
    }
};

template<class KeyT, class ValT>
class HashTableElementContent {
public:
    KeyT key;
    ValT val;
    HashTableElementContent(const KeyT& k, const ValT& v) : key(k), val(v) {}
};

template<class KeyT, class ValT>
using HashTableElement = DoubleLinkedList<HashTableElementContent<KeyT, ValT>>;

template<class KeyT, class ValT, std::size_t TABLE_SIZE, class HASH_CALCULATOR = HashCalc<KeyT>>
class HashTable {
private:
    std::array<HashTableElement<KeyT, ValT>*, TABLE_SIZE> table;
    std::size_t sizedHash(const KeyT& key) { return HASH_CALCULATOR::calculate(key) % TABLE_SIZE; }
    std::size_t counter = 0;
public:
    HashTable() { }
    //HashTable(const HashTable<KeyT, ValT, TABLE_SIZE, HASH_CALCULATOR>& table) { }
    //HashTable(HashTable<KeyT, ValT, TABLE_SIZE, HASH_CALCULATOR>&& table) { }

    ValT get(const KeyT& key) {
        ValT rtn;
        table[sizedHash(key)]->iterate((iterate_function<KeyT>)[rtn, key](auto ele){
            if(ele->key == key) { rtn = ele->val; return false; }
            return true;
        });
        return rtn;
    }
    void set(const KeyT& key, const ValT& val){
        counter++;
        bool isUpdateValue = false;
        table[sizedHash(key)]->iterate((iterate_function<KeyT>)[isUpdateValue, key, val](auto ele){
            if(ele->key == key) { isUpdateValue = true; ele->val = val; return false; }
            return true;
        });
        if(isUpdateValue) return;
        HashTableElementContent<KeyT, ValT> ele(key, val);
        table[sizedHash(key)]->insert(ele);
    }
    bool containsKey(const KeyT& key){
        bool rtn = false;
        table[sizedHash(key)]->iterate((iterate_function<KeyT>)[rtn, key](auto ele){
            if(ele->key == key) { rtn = true; return false; }
            return true;
        });
        return rtn;
    }
    std::size_t size() { return this->counter; }

    ~HashTable() { }
};

static int ht_test = push_test("Hash Table", (test_function)[](){
    HashTable<int, int, 10>* t = new HashTable<int, int, 10>();
    return TEST_SUCCESS;
});
