#pragma once

#include "ds_tree_node.h"

namespace ds_tree {

using namespace ds_tree_node;

template<class T>
class BinaryTree {
protected:
    BinaryTreeNode<T>* rootNode;
public:
    BinaryTree() { rootNode = nullptr; }
    BinaryTree(BinaryTreeNode<T>* val) : rootNode(val){ }
    BinaryTree(T val) { rootNode = new BinaryTreeNode<T>(val); }
    //Copy constructor
    //Deep copy, copy entire tree recursively.
    BinaryTree(const BinaryTree<T>& tree) {
        if(tree->rootNode == nullptr) { this->rootNode = nullptr; return; }
        this->rootNode = tree->rootNode->deepCopy();
    }
    //Move constructor
    BinaryTree(BinaryTree<T>&& tree){
        this->rootNode = tree->rootNode;
        tree->rootNode = nullptr; //Prevent deletion of root node and its child nodes.
    }

    virtual BinaryTreeNode<T>* getRootNode() const { return rootNode; }
    void setRootNode(BinaryTreeNode<T>* node) { if(rootNode != nullptr) delete rootNode; rootNode = node; }
    template<BTREE_TRAVERSAL_ORDER_TYPE ORDER = INORDER>
    void traversal(std::function<bool(BinaryTreeNode<T>*)> func) {
        if(rootNode != nullptr) rootNode->template traversal<ORDER>(func);
    }
    BinaryTreeNode<T>* transplant(BinaryTreeNode<T>* original, BinaryTreeNode<T>* replacement) {
        if(original == rootNode) rootNode = replacement;
        else original->overwriteSelfInParent(replacement);
        if(replacement != nullptr) replacement->overwriteParent(original->getParent());
        return original;
    }
    void transplantAndDeleteDangling(BinaryTreeNode<T>* original, BinaryTreeNode<T>* replacement) {
        BinaryTreeNode<T>* danglingNode = this->template transplant<T>(original, replacement);
        if(danglingNode != nullptr) delete danglingNode;
    }
    bool isValidBinarySearchTree(){
        return rootNode->isValidBinarySearchTree();
    }
    bool isFullTree(){
        if(this->rootNode == nullptr) return true;
        return this->rootNode->isFullTree();
    }

    virtual ~BinaryTree() { if(rootNode != nullptr) delete rootNode; }
};


template<class T>
class BinarySearchTree : public BinaryTree<T> {
public:
    BinarySearchTree() { this->setRootNode(nullptr); }
    BinarySearchTree(BinarySearchTreeNode<T>* val) { this->setRootNode(val); }
    BinarySearchTree(T val) { this->setRootNode(new BinarySearchTreeNode<T>(val)); }
    //Copy constructor
    BinarySearchTree(const BinarySearchTree<T>& tree) {
        if(tree->rootNode == nullptr) { this->rootNode = nullptr; return; }
        this->rootNode = tree->rootNode->deepCopy();
    }
    //Move constructor
    BinarySearchTree(BinarySearchTree<T>&& tree){
        this->rootNode = tree->rootNode;
        tree->rootNode = nullptr; //Prevent deletion of root node and its child nodes.
    }

    virtual BinarySearchTreeNode<T>* getRootNode() const { return dynamic_cast<BinarySearchTreeNode<T>*>(this->rootNode); }
    BinarySearchTreeNode<T>* search(T val) const {
        return (this->getRootNode())->searchChildIncludingSelf(val);
    }
    template<BSTREE_SEARCH_MIN_MAX MINMAX>
    BinarySearchTreeNode<T>* search() const {
        return (this->getRootNode())->template searchChildIncludingSelf<MINMAX>();
    }
    void insert(T val){
        BinarySearchTreeNode<T>* z = new BinarySearchTreeNode<T>(val);
        BinarySearchTreeNode<T>* y = nullptr;
        BinarySearchTreeNode<T>* x = this->getRootNode();
        while(x != nullptr){
            y = x;
            if(z->getValue() < x->getValue()) x = x->getChild(LEFT_CHILD);
            else x = x->getChild(RIGHT_CHILD);
        }
        z->overwriteParent(y);
        if(y == nullptr) this->setRootNode(z);
        else if(z->getValue() < y->getValue()) y->overwriteChild(z, LEFT_CHILD);
        else y->overwriteChild(z, RIGHT_CHILD);
    }
    void remove(T val) {
        this->remove(this->search(val));
    }
    void remove(BinarySearchTreeNode<T>* node){
        if(node == nullptr) return;
        
        auto parent = node->getParent();
        auto lchild = node->getChild(LEFT_CHILD);
        auto rchild = node->getChild(RIGHT_CHILD);

        if(lchild == nullptr) this->transplant(node, rchild); 
        else if(rchild == nullptr) this->transplant(node, lchild);
        else {
            auto y = rchild->template searchChildIncludingSelf<BST_SEARCH_MIN>();
            if(y->getParent() != node){
                this->transplant(y, y->getChild(RIGHT_CHILD));
                rchild->overwriteParent(y);
                y->overwriteChild(rchild, RIGHT_CHILD);
            }
            this->transplant(node, y);
            lchild->overwriteParent(y);
            y->overwriteChild(lchild, LEFT_CHILD);
        }
    }

    virtual ~BinarySearchTree() { }
};

template<class T>
class AVLTree : public BinarySearchTree<T> {
public:
    AVLTree() { this->setRootNode(nullptr); }
    AVLTree(AVLTree<T>* val) { this->setRootNode(val); }
    AVLTree(T val) { this->setRootNode(new AVLTree<T>(val)); }
    //Copy constructor
    AVLTree(const AVLTree<T>& tree) {
        if(tree->rootNode == nullptr) { this->rootNode = nullptr; return; }
        this->rootNode = tree->rootNode->deepCopy();
    }
    //Move constructor
    AVLTree(AVLTree<T>&& tree){
        this->rootNode = tree->rootNode;
        tree->rootNode = nullptr; //Prevent deletion of root node and its child nodes.
    }

    virtual AVLTree<T>* getRootNode() const { return dynamic_cast<AVLTree<T>*>(this->rootNode); }
    
    virtual ~AVLTree() { }
};



template<class T>
using btree_traversal_funcion = std::function<bool(BinaryTreeNode<T>*)>;

static int bst_test = push_test("BTree & BST", (test_function)[](){
    BinarySearchTree<int>* bst = new BinarySearchTree<int>(50);
    bst->insert(48);
    bst->insert(52);
    bst->insert(47);
    bst->insert(49);
    bst->insert(51);
    bst->insert(53);

    std::cout << "Root node      : " << bst->getRootNode()->getValue() << std::endl;
    std::cout << "Root node next : " << bst->getRootNode()->successor()->getValue() << std::endl;
    std::cout << "Root node prev : " << bst->getRootNode()->predecessor()->getValue() << std::endl;

    btree_traversal_funcion<int> print_value = [](auto node){ std::cout << node->getValue() << " "; return true; };

    std::cout << "Preorder  : "; bst->traversal<PREORDER>(print_value); std::cout << std::endl;
    std::cout << "Inorder   : "; bst->traversal<INORDER>(print_value); std::cout << std::endl;
    std::cout << "Postorder : "; bst->traversal<POSTORDER>(print_value); std::cout << std::endl;
    
    std::cout << "I\'m " << (bst->isValidBinarySearchTree() ? "":"not ") << "BST." << std::endl;

    bst->remove(50);

    std::cout << "Preorder  : "; bst->traversal<PREORDER>(print_value); std::cout << std::endl;
    std::cout << "Inorder   : "; bst->traversal<INORDER>(print_value); std::cout << std::endl;
    std::cout << "Postorder : "; bst->traversal<POSTORDER>(print_value); std::cout << std::endl;

    std::cout << "I\'m " << (bst->isValidBinarySearchTree() ? "":"not ") << "BST." << std::endl;

    //std::cout << bst->search<BST_SEARCH_MIN>()->getValue() << std::endl;
    //std::cout << bst->search<BST_SEARCH_MAX>()->getValue() << std::endl;
    //std::cout << ((bst->search(52) != nullptr) ? "EXIST ":"NOT EXIST ");
    //std::cout << ((bst->search(53) != nullptr) ? "EXIST ":"NOT EXIST ");
    //std::cout << ((bst->search(54) != nullptr) ? "EXIST ":"NOT EXIST ") << std::endl;
    
    delete bst;

    return TEST_SUCCESS;
});

}
