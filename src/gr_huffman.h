#pragma once

#include "ds_tree.h"

using Freq = float;

template<class ValT>
struct ValFreqPair {
    ValT value;
    Freq frequency;
    ValFreqPair() { }
    ValFreqPair(const ValFreqPair<ValT>& vfp) : value(vfp.value), frequency(vfp.frequency) { }
    ValFreqPair(const ValT& v, Freq f) : value(v), frequency(f) { }
    ValFreqPair(Freq f) : frequency(f) { }
    ~ValFreqPair() { }
};

template<class ValT>
std::ostream& operator<< (std::ostream &out, const ValFreqPair<ValT>& vfp) {
    out << "{ val: " << vfp.value << ", freq: " << vfp.frequency << " }";
    return out;
}

template<class ValT>
class FreqList {
private:
    std::vector<ValFreqPair<ValT>> vec;
    void insert() { }
public:
    FreqList() { }
    template<class... Types>
    FreqList(ValT t, Freq f, Types... args) {
        insert(t, f, args...);
    }
    FreqList(const FreqList<ValT>& fl) {
        vec = fl.vec;
    }
    
    std::size_t size() const { return this->vec.size(); }
    void insert(const ValFreqPair<ValT>& data) {
        this->vec.push_back(data);
    }
    void insert(ValT t, Freq f) {
        this->insert(ValFreqPair<ValT>(t, f));
    }
    template<class... Types>
    void insert(ValT t, Freq f, Types... args) {
        insert(t, f);
        insert(args...);
    }
    ValFreqPair<ValT> min(bool extract = false){
        if(this->vec.size() == 0) throw("Failed to extract because FraqList size is 0.");
        auto itt = this->vec.begin();
        ValFreqPair<ValT> vfp = *itt;
        for(auto it = this->vec.begin(); it != vec.end(); it++){
            if(vfp.frequency > it->frequency) {
                vfp = *it;
                itt = it;
            }
        }
        //remove(...) value, erase(...) index(or say position). :)
        if(extract) vec.erase(itt);
        return vfp;
    }
    const std::vector<ValFreqPair<ValT>>& vector() const {
        return this->vec;
    }

    ~FreqList(){ }
};

template<class ValT>
BinaryTree<ValFreqPair<ValT>>* HuffmanTree(const FreqList<ValT>& l){
    if (l.size() == 0) return new BinaryTree<ValFreqPair<ValT>>();

    //Create a new freq list using the same data except each element is wrapped with BinaryTreeNode.
    FreqList<BinaryTreeNode<ValFreqPair<ValT>>*> treeList;
    for(auto it = l.vector().begin(); it != l.vector().end(); it++){
        treeList.insert(new BinaryTreeNode<ValFreqPair<ValT>>(*it), it->frequency);
    }
    
    //.......why would I do this.
    while(treeList.size() != 1){
        auto node = new BinaryTreeNode<ValFreqPair<ValT>>();
        BinaryTreeNode<ValFreqPair<ValT>>* x = treeList.min(true).value;
        BinaryTreeNode<ValFreqPair<ValT>>* y = treeList.min(true).value;
        x->overwriteParent(node);
        y->overwriteParent(node);
        node->overwriteChild(x, LEFT_CHILD);
        node->overwriteChild(y, RIGHT_CHILD);
        Freq frequency = x->getValueRef()->frequency + y->getValueRef()->frequency;
        node->setValue(ValFreqPair<ValT>(frequency));
        treeList.insert(node, frequency);
    }

    return new BinaryTree<ValFreqPair<ValT>>(treeList.min().value);
}

static int grh_test = push_test("Huffman", (test_function)[](){
    FreqList freqArr0('f', 5, 'e', 9, 'c', 12, 'b', 13, 'd', 16, 'a', 45);
    FreqList freqArr1( 'A'   , 0.1f, 'B'   , 0.4f,    'C', 0.2f, 'D', 0.3f);
    FreqList freqArr2( 60    , 0.1f,  72   , 0.1f,     88, 0.1f,  -5, 0.1f, -27, 0.1f, -102, 0.1f, -102, 0.4f);
    FreqList freqArr3( "Meow", 0.5f, "Woof", 0.3f, "Bonk", 0.2f);

    auto* tree = HuffmanTree(freqArr0);
    btree_traversal_funcion<ValFreqPair<char>> print_value = [](auto node){ std::cout << node->getValue() << std::endl; return true; };
    std::cout << "Preorder of Huffman tree: " << std::endl;
    tree->traversal<PREORDER>(print_value);
    delete tree;
    return TEST_SUCCESS;
});