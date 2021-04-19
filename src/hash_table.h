#pragma once

#include "linked_list.h"

template<class T> struct HashCalc { };
template<> struct HashCalc<int> {
    static std::size_t calculate(const int& data){
        return std::hash<int>()(data);
    }
};

template<class KeyT, class ValT>
class KVPair {
public:
    KeyT key;
    ValT val;
    KVPair() { }
    KVPair(const KeyT& k, const ValT& v) : key(k), val(v) { }
};

template<class KeyT, class ValT>
using HashTableElement = DoubleLinkedList<KVPair<KeyT, ValT>>;

template<class KeyT, class ValT, std::size_t TABLE_SIZE, class HASH_CALCULATOR = HashCalc<KeyT>>
class HashTable {
private:
    std::array<HashTableElement<KeyT, ValT>*, TABLE_SIZE> table;
    std::size_t sizedHash(const KeyT& key) { return HASH_CALCULATOR::calculate(key) % TABLE_SIZE; }
    std::size_t counter = 0;
public:
    HashTable() { for(std::size_t i=0; i<TABLE_SIZE; i++) table[i] = new HashTableElement<KeyT, ValT>(); }
    //HashTable(const HashTable<KeyT, ValT, TABLE_SIZE, HASH_CALCULATOR>& table) { }
    //HashTable(HashTable<KeyT, ValT, TABLE_SIZE, HASH_CALCULATOR>&& table) { }

    ValT get(const KeyT& key) {
        ValT rtn = 0;
        table[sizedHash(key)]->iterate((iterate_function<KVPair<KeyT, ValT>>)[&rtn, key](auto ele){
            if(ele->key == key) { rtn = ele->val; return false; }
            return true;
        });
        return rtn;
    }
    void set(const KeyT& key, const ValT& val){
        counter++;
        bool isUpdateValue = false;
        table[sizedHash(key)]->iterate((iterate_function<KVPair<KeyT, ValT>>)[&isUpdateValue, key, val](auto ele){
            if(ele->key == key) { isUpdateValue = true; ele->val = val; return false; }
            return true;
        });
        if(isUpdateValue) return;
        KVPair<KeyT, ValT> ele(key, val);
        table[sizedHash(key)]->insert(ele);
    }
    bool containsKey(const KeyT& key){
        bool rtn = false;
        table[sizedHash(key)]->iterate((iterate_function<KVPair<KeyT, ValT>>)[&rtn, key](auto ele){
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
    t->set(4, 1);
    std::cout << t->get(4) << std::endl;

    delete t;

    return TEST_SUCCESS;
});
