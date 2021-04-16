#pragma once

#include "node.h"

enum BTREE_TRAVERSAL_ORDER_TYPE { PREORDER, INORDER, POSTORDER };
enum BTREE_CHILD_IDENTIFIER { LEFT_CHILD = 0, RIGHT_CHILD = 1 };
enum BTREE_PARENT_IDENTIFIER { PARENT = 2 };
const std::size_t BTREE_LINK_SIZE = 3;

template<class T>
class BinaryTreeNode : public LinkableNode<T, BTREE_LINK_SIZE> {
public:
    BinaryTreeNode() {}
    BinaryTreeNode(T val) {this->value = val; }
    //Copy constructor
    //Since it's linked with its parent and child nodes, and the parent and childs are linked with their ... and so on.
    //So I think we should not copy construct a btree node. But we can copy construct an entire btree though.
    //BinaryTreeNode(const BinaryTreeNode<T>& node) { }
    //Move constructor
    BinaryTreeNode(BinaryTreeNode<T>&& node){
        this->value = node.value;
        for(std::size_t i = 0; i < BTREE_LINK_SIZE; i++) {
            this->link[i] = node.link[i];
            node.link[i] = nullptr; //Prevent deletion of child nodes.
        }
    }

    virtual BinaryTreeNode<T>* getChild(BTREE_CHILD_IDENTIFIER i) const {
        return dynamic_cast<BinaryTreeNode<T>*>(this->link[i]);
    }
    virtual BinaryTreeNode<T>* getParent() const {
        return dynamic_cast<BinaryTreeNode<T>*>(this->link[PARENT]);
    }
    void overwriteChild(BinaryTreeNode<T>* node, BTREE_CHILD_IDENTIFIER i) {
        this->link[i] = node;
    }
    void overwriteParent(BinaryTreeNode<T>* node) {
        this->link[PARENT] = node;
    }
    BTREE_CHILD_IDENTIFIER whichChildOfParent() {
        auto parent = this->getParent();
        if(parent == nullptr) throw "Doesn't have a parent!\n";
        if(parent->getChild(LEFT_CHILD) == this) return LEFT_CHILD;
        else if(parent->getChild(RIGHT_CHILD) == this) return RIGHT_CHILD;
        else throw "Child-parent relationship corrupt!\n";
    }
    void overwriteSelfInParent(BinaryTreeNode<T>* node){
        auto parent = this->getParent();
        if(parent == nullptr) return;
        if(parent->getChild(LEFT_CHILD) == this) parent->overwriteChild(node, LEFT_CHILD);
        else parent->overwriteChild(node, RIGHT_CHILD);
    }
    template<BTREE_TRAVERSAL_ORDER_TYPE ORDER = INORDER>
    void traversal(std::function<bool(BinaryTreeNode<T>*)> func) {
        if constexpr(ORDER == PREORDER) { if(!func(this)) return; }
        auto lchild = this->getChild(LEFT_CHILD);
        if(lchild != nullptr) lchild->template traversal<ORDER>(func);
        if constexpr(ORDER == INORDER) { if(!func(this)) return; }
        auto rchild = this->getChild(RIGHT_CHILD);
        if(rchild != nullptr) rchild->template traversal<ORDER>(func);
        if constexpr(ORDER == POSTORDER) { if(!func(this)) return; }
    }
    bool isValidBinarySearchTree(){
        bool rtn = true;
        bool isFirstValue = true;
        T lastValue;
        traversal((std::function<bool(BinaryTreeNode<T>*)>)[&isFirstValue, &lastValue, &rtn](auto node){
            if(isFirstValue) isFirstValue = false;
            else if(lastValue > node->value) { rtn = false; return false; }
            lastValue = node->value;
            return true;
        });
        return rtn;
    }
    bool isFullTree(){
        auto lc = this->getChild(LEFT_CHILD);
        auto rc = this->getChild(RIGHT_CHILD);
        if((lc == nullptr) && (rc == nullptr)) return true;
        if((lc != nullptr) && (rc != nullptr)) return ((lc->isFullTree()) && (rc->isFullTree()));
        return false;
    }
    virtual BinaryTreeNode<T>* deepCopy(){
        BinaryTreeNode<T>* lchild = nullptr;
        BinaryTreeNode<T>* rchild = nullptr;
        if(this->link[LEFT_CHILD] != nullptr) lchild = this->getChild(LEFT_CHILD)->deepCopy();
        if(this->link[RIGHT_CHILD] != nullptr) rchild = this->getChild(RIGHT_CHILD)->deepCopy();
        BinaryTreeNode<T>* selfCopy = new BinaryTreeNode<T>();
        selfCopy->value = this->value;
        selfCopy->link[LEFT_CHILD] = lchild;
        selfCopy->link[RIGHT_CHILD] = rchild;
        return selfCopy;
    }

    virtual ~BinaryTreeNode() {
        auto child = this->link[LEFT_CHILD];
        if(child != nullptr) delete child;
        child = this->link[RIGHT_CHILD];
        if(child != nullptr) delete child;
    }
};

enum BSTREE_SEARCH_MIN_MAX { BST_SEARCH_MIN = 0, BST_SEARCH_MAX = 1 };
template<class T>
class BinarySearchTreeNode : public BinaryTreeNode<T> {
public:
    BinarySearchTreeNode() {}
    BinarySearchTreeNode(T val) { this->value = val; }
    //Copy constructor
    //BinarySearchTreeNode(const BinarySearchTreeNode<T>& node) { }
    //Move constructor
    BinarySearchTreeNode(BinarySearchTreeNode<T>&& node){
        this->value = node.value;
        for(std::size_t i = 0; i < BTREE_LINK_SIZE; i++) {
            this->link[i] = node.link[i];
            node.link[i] = nullptr; //Prevent deletion of child nodes.
        }
    }

    virtual BinarySearchTreeNode<T>* getChild(BTREE_CHILD_IDENTIFIER i) const {
        return dynamic_cast<BinarySearchTreeNode<T>*>(this->link[i]);
    }
    virtual BinarySearchTreeNode<T>* getParent() const {
        return dynamic_cast<BinarySearchTreeNode<T>*>(this->link[PARENT]);
    }
    BinarySearchTreeNode<T>* searchChildIncludingSelf(T val) const {
        auto node = const_cast<BinarySearchTreeNode<T>*>(this);
        while((node != nullptr) && (node->value != val)){
            if(val < node->value) node = node->getChild(LEFT_CHILD);
            else node = node->getChild(RIGHT_CHILD);
        }
        return dynamic_cast<BinarySearchTreeNode<T>*>(node);
    }
    template<BSTREE_SEARCH_MIN_MAX MINMAX>
    BinarySearchTreeNode<T>* searchChildIncludingSelf() const {
        auto node = const_cast<BinarySearchTreeNode<T>*>(this);
        while(node != nullptr){
            BinarySearchTreeNode<T>* child = nullptr;
            if constexpr (MINMAX == BST_SEARCH_MIN) child = node->getChild(LEFT_CHILD);
            else if constexpr (MINMAX == BST_SEARCH_MAX) child = node->getChild(RIGHT_CHILD);
            if(child == nullptr) break;
            node = child;
        }
        return node;
    }
private:
    template<BTREE_CHILD_IDENTIFIER I, BSTREE_SEARCH_MIN_MAX MINMAX>
    BinarySearchTreeNode<T>* successorOrPredecessor() const {
        //Successor   : It's either the left most child node of the right child tree of current node.
        //Predecessor : It's either the right most child node of the left child tree of current node.
        BinarySearchTreeNode<T>* child = this->getChild(I);
        if(child != nullptr) return child->template searchChildIncludingSelf<MINMAX>();
        
        //Successor   : Or it's the first parent node which the current node is a child of it's left child tree.
        //Predecessor : Or it's the first parent node which the current node is a child of it's right child tree.
        BinarySearchTreeNode<T>* x = const_cast<BinarySearchTreeNode<T>*>(this);
        BinarySearchTreeNode<T>* y = this->getParent();
        while((y != nullptr) && (x == y->getChild(I))){
            x = y;
            y = y->getParent();
        }
        return y;
    }
public:
    BinarySearchTreeNode<T>* successor() const {
        return this->successorOrPredecessor<RIGHT_CHILD, BST_SEARCH_MIN>();
    }
    BinarySearchTreeNode<T>* predecessor() const {
        return this->successorOrPredecessor<LEFT_CHILD, BST_SEARCH_MAX>();
    }
    virtual BinarySearchTreeNode<T>* deepCopy(){
        return dynamic_cast<BinarySearchTreeNode<T>*>(BinaryTreeNode<T>::deepCopy());
    }

    virtual ~BinarySearchTreeNode() { }
};

template<class T>
class AVLTreeNode : public BinarySearchTreeNode<T> {
private:
    std::size_t height;
public:
    AVLTreeNode() : height(1) {}
    AVLTreeNode(T val) : height(1) { this->value = val; }
    //Copy constructor
    //AVLTreeNode(const AVLTreeNode<T>& node) { }
    //Move constructor
    AVLTreeNode(AVLTreeNode<T>&& node) : height(1) {
        this->value = node.value;
        this->height = node.height;
        for(std::size_t i = 0; i < BTREE_LINK_SIZE; i++) {
            this->link[i] = node.link[i];
            node.link[i] = nullptr; //Prevent deletion of child nodes.
        }
    }

    virtual AVLTreeNode<T>* getChild(BTREE_CHILD_IDENTIFIER i) const {
        return dynamic_cast<AVLTreeNode<T>*>(this->link[i]);
    }
    virtual AVLTreeNode<T>* getParent() const {
        return dynamic_cast<AVLTreeNode<T>*>(this->link[PARENT]);
    }
    std::size_t getHeight() const {
        return this->height;
    }
    void setHeight(std::size_t h) const {
        this->height = h;
    }
    int getBalance() const {
        return (
            static_cast<int>(this->getChild(LEFT_CHILD)->getHeight()) - 
            static_cast<int>(this->getChild(RIGHT_CHILD)->getHeight())
            );
    }
};