#pragma once

#include "ds_node.h"

template<class T>
using SingleLinkNode = LinkableNode<T, 1>;
template<class T>
using DoubleLinkNode = LinkableNode<T, 2>;

constexpr std::size_t DOUBLE_LINK_PREV = 0;
constexpr std::size_t DOUBLE_LINK_NEXT = 1;

#define GET_PREV() getLink(DOUBLE_LINK_PREV)
#define GET_NEXT() getLink(DOUBLE_LINK_NEXT)
#define SET_PREV(...) setLink(__VA_ARGS__ __VA_OPT__(,) DOUBLE_LINK_PREV)
#define SET_NEXT(...) setLink(__VA_ARGS__ __VA_OPT__(,) DOUBLE_LINK_NEXT)

template<class T>
using iterate_function = std::function<bool(T*)>;

template<class T>
class DoubleLinkedList {
private:
    DoubleLinkNode<T>* rootNode;
    DoubleLinkNode<T>* lastNode;
    std::size_t counter;
public:
    DoubleLinkedList() {
        this->rootNode = nullptr;
        this->lastNode = nullptr;
        this->counter = 0;
    }
    DoubleLinkedList(T val) {
        this->rootNode = new DoubleLinkNode<T>(val);
        this->lastNode = this->rootNode;
        this->counter++;
    }
    //DoubleLinkedList(const DoubleLinkedList& l) {  }
    DoubleLinkedList(DoubleLinkedList&& l) {
        this->rootNode = l->rootNode;
        this->lastNode = l->lastNode;
        this->counter = l->counter;
        l->rootNode = nullptr;
        l->lastNode = nullptr;
    }
    void insert(T val){
        DoubleLinkNode<T>* newNode = new DoubleLinkNode<T>(val);
        if(this->counter == 0){
            this->rootNode = newNode;
            this->lastNode = newNode;
        }else{
            newNode->SET_PREV(this->lastNode);
            this->lastNode->SET_NEXT(newNode);
            this->lastNode = newNode;
        }
        this->counter++;
    }
    //In case of multiple identical value insertion, we only delete the first one.
    void remove(T val){
        DoubleLinkNode<T>* nextNode = this->rootNode;
        while(nextNode != nullptr){
            if(nextNode->getValue() == val) {
                nextNode->GET_PREV()->SET_NEXT(nextNode->GET_NEXT());
                nextNode->GET_NEXT()->SET_PREV(nextNode->GET_PREV());
                this->counter--;
                break;
            }
            nextNode = nextNode->GET_NEXT();
        }
    }
    void iterate(iterate_function<T> func){
        DoubleLinkNode<T>* nextNode = this->rootNode;
        while(nextNode != nullptr){
            if(!func(nextNode->getValueRef())) break;
            nextNode = nextNode->GET_NEXT();
        }
    }
    DoubleLinkedList<T>* getRootNode() { return this->rootNode; }
    std::size_t size() { return this->counter; }

    ~DoubleLinkedList() {
        DoubleLinkNode<T>* nextNode = this->lastNode;
        while(nextNode != nullptr){
            DoubleLinkNode<T>* n = nextNode->GET_NEXT();
            delete nextNode;
            nextNode = n;
        }
        this->rootNode = nullptr;
        this->lastNode = nullptr;
        this->counter = 0;
    }
};


static int ll_test = push_test("Linked List", (test_function)[](){

    DoubleLinkedList<int>* dl = new DoubleLinkedList<int>();
    dl->insert(1);
    dl->insert(2);
    dl->insert(3);
    dl->insert(4);
    dl->insert(3);
    dl->insert(2);
    dl->insert(1);

    dl->iterate((iterate_function<int>)[](auto val){ std::cout << *val << " "; return true; });
    std::cout << std::endl << dl->size() << std::endl;
    
    dl->remove(3);

    dl->iterate((iterate_function<int>)[](auto val){ std::cout << *val << " "; return true; });
    std::cout << std::endl << dl->size() << std::endl;

    delete dl;

    return TEST_SUCCESS;
});
