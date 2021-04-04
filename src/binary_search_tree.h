template<class T>
class Node {
private:
    T value;
public:
    Node() {}
    Node(T val) : value(val) {}
    virtual ~Node() {}

    T getValue() const { return value; }
    void setValue(T val) { value = val; }
};

template<class T, std::size_t LINK_SIZE>
class LinkableNode : public Node<T> {
private:
    std::array<LinkableNode*, LINK_SIZE> link = { nullptr };
public:
    LinkableNode() {}
    LinkableNode(T val) { setValue(val); }

    //For linkable node, there is no reason to delete before replacement since there's no way to tell whether the linked node is useless or not.
    void setLink(LinkableNode<T, LINK_SIZE>* node, std::size_t index) { link[index] = node; }
    LinkableNode<T, LINK_SIZE>* getLink(std::size_t index) const { return link[index]; }

    virtual ~LinkableNode() {}
};

enum BTREE_TRAVERSAL_ORDER_TYPE { PREORDER, INORDER, POSTORDER };
enum BTREE_CHILD_IDENTIFIER { LEFT_CHILD = 0, RIGHT_CHILD = 1 };
enum BTREE_PARENT_IDENTIFIER { PARENT = 2 };
const std::size_t BTREE_LINK_SIZE = 3;
template<class T>
class BinaryTreeNode : public LinkableNode<T, BTREE_LINK_SIZE> {
public:
    BinaryTreeNode() {}
    BinaryTreeNode(T val) { this->setValue(val); }

    BinaryTreeNode<T>* getChild(BTREE_CHILD_IDENTIFIER i) const {
        return dynamic_cast<BinaryTreeNode<T>*>(this->getLink(i));
    }
    BinaryTreeNode<T>* getParent() const {
        return dynamic_cast<BinaryTreeNode<T>*>(this->getLink(PARENT));
    }

    void overwriteChild(BinaryTreeNode<T>* node, BTREE_CHILD_IDENTIFIER i) {
        this->setLink(node, i);
    }
    void overwriteParent(BinaryTreeNode<T>* node) {
        this->setLink(node, PARENT);
    }
    
    template<BTREE_TRAVERSAL_ORDER_TYPE ORDER = INORDER>
    void traversal(std::function<void(BinaryTreeNode<T>*)> func) {
        if constexpr(ORDER == PREORDER) func(this);
        auto lchild = this->getChild(LEFT_CHILD);
        if(lchild != nullptr) lchild->template traversal<ORDER>(func);
        if constexpr(ORDER == INORDER) func(this);
        auto rchild = this->getChild(RIGHT_CHILD);
        if(rchild != nullptr) rchild->template traversal<ORDER>(func);
        if constexpr(ORDER == POSTORDER) func(this);
    }

    BTREE_CHILD_IDENTIFIER whichChildOfParent() {
        auto parent = this->getParent();
        if(parent == nullptr) throw "Doesn't have a parent!\n";
        if(parent->getChild(LEFT_CHILD) == this) return LEFT_CHILD;
        else if(parent->getChild(RIGHT_CHILD) == this) return RIGHT_CHILD;
        else throw "Child-parent relationship currupt!\n";
    }

    void overwriteSelfInParent(BinaryTreeNode<T>* node){
        auto parent = this->getParent();
        if(parent == nullptr) return;
        if(parent->getChild(LEFT_CHILD) == this) parent->overwriteChild(node, LEFT_CHILD);
        else parent->overwriteChild(node, RIGHT_CHILD);
    }
    
    virtual ~BinaryTreeNode() {
        auto child = this->getLink(LEFT_CHILD);
        if(child != nullptr) delete child;
        child = this->getLink(RIGHT_CHILD);
        if(child != nullptr) delete child;
    }
};

enum BSTREE_SEARCH_MIN_MAX { BST_SEARCH_MIN = 0, BST_SEARCH_MAX = 1 };
template<class T>
class BinarySearchTreeNode : public BinaryTreeNode<T> {
public:
    BinarySearchTreeNode() {}
    BinarySearchTreeNode(T val) { this->setValue(val); }

    BinarySearchTreeNode<T>* searchChildIncludingSelf(T val) const {
        BinaryTreeNode<T>* node = const_cast<BinarySearchTreeNode<T>*>(this);
        while((node != nullptr) && (node->getValue() != val)){
            if(val < node->getValue()) node = node->getChild(LEFT_CHILD);
            else node = node->getChild(RIGHT_CHILD);
        }
        return dynamic_cast<BinarySearchTreeNode<T>*>(node);
    }

    template<BSTREE_SEARCH_MIN_MAX MINMAX>
    BinarySearchTreeNode<T>* searchChildIncludingSelf() const {
        BinaryTreeNode<T>* node = const_cast<BinarySearchTreeNode<T>*>(this);
        while(node != nullptr){
            BinaryTreeNode<T>* child = nullptr;
            if constexpr (MINMAX == BST_SEARCH_MIN) child = node->getChild(LEFT_CHILD);
            else if constexpr (MINMAX == BST_SEARCH_MAX) child = node->getChild(RIGHT_CHILD);
            if(child == nullptr) break;
            node = child;
        }
        return dynamic_cast<BinarySearchTreeNode<T>*>(node);
    }

    BinarySearchTreeNode<T>* successor() const {
        //It's either the left most child node of the right child tree of current node:
        BinarySearchTreeNode<T>* rchild = dynamic_cast<BinarySearchTreeNode<T>*>(this->getChild(RIGHT_CHILD));
        if(rchild != nullptr) return rchild->template search<BST_SEARCH_MIN>();
        
        //Or it's the first parent node which the current node is a child of it's left child tree:
        BinarySearchTreeNode<T>* x = const_cast<BinarySearchTreeNode<T>*>(this);
        BinarySearchTreeNode<T>* y = dynamic_cast<BinarySearchTreeNode<T>*>(this->getParent());
        while((y != nullptr) && (x == dynamic_cast<BinarySearchTreeNode<T>*>(y->getChild(RIGHT_CHILD)))){
            x = y;
            y = dynamic_cast<BinarySearchTreeNode<T>*>(y->getParent());
        }
        return y;
    }

    BinarySearchTreeNode<T>* predecessor() const {
        //It's either the right most child node of the left child tree of current node:
        BinarySearchTreeNode<T>* lchild = dynamic_cast<BinarySearchTreeNode<T>*>(this->getChild(LEFT_CHILD));
        if(lchild != nullptr) return lchild->template search<BST_SEARCH_MAX>();
        
        //Or it's the first parent node which the current node is a child of it's right child tree:
        BinarySearchTreeNode<T>* x = const_cast<BinarySearchTreeNode<T>*>(this);
        BinarySearchTreeNode<T>* y = dynamic_cast<BinarySearchTreeNode<T>*>(this->getParent());
        while((y != nullptr) && (x == dynamic_cast<BinarySearchTreeNode<T>*>(y->getChild(LEFT_CHILD)))){
            x = y;
            y = dynamic_cast<BinarySearchTreeNode<T>*>(y->getParent());
        }
        return y;
    }

    virtual ~BinarySearchTreeNode() { }
};

template<class T>
class BinaryTree {
private:
    BinaryTreeNode<T>* rootNode;
public:
    BinaryTree() { rootNode = nullptr; }
    BinaryTree(BinaryTreeNode<T>* val) : rootNode(val){ }
    BinaryTree(T val) { rootNode = new BinaryTreeNode<T>(val); }

    BinaryTreeNode<T>* getRootNode() const { return rootNode; }
    void setRootNode(BinaryTreeNode<T>* node) { if(rootNode != nullptr) delete rootNode; rootNode = node; }

    template<BTREE_TRAVERSAL_ORDER_TYPE ORDER = INORDER>
    void traversal(std::function<void(BinaryTreeNode<T>*)> func) {
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
    
    virtual ~BinaryTree() { if(rootNode != nullptr) delete rootNode; }
};


template<class T>
class BinarySearchTree : public BinaryTree<T> {
public:
    BinarySearchTree() { this->setRootNode(nullptr); }
    BinarySearchTree(BinarySearchTreeNode<T>* val) { this->setRootNode(val); }
    BinarySearchTree(T val) { this->setRootNode(new BinarySearchTreeNode<T>(val)); }

    BinarySearchTreeNode<T>* search(T val) const {
        return (dynamic_cast<BinarySearchTreeNode<T>*>(this->getRootNode()))->searchChildIncludingSelf(val);
    }

    template<BSTREE_SEARCH_MIN_MAX MINMAX>
    BinarySearchTreeNode<T>* search() const {
        return (dynamic_cast<BinarySearchTreeNode<T>*>(this->getRootNode()))->template searchChildIncludingSelf<MINMAX>();
    }

    void insert(T val){
        BinarySearchTreeNode<T>* z = new BinarySearchTreeNode<T>(val);
        BinarySearchTreeNode<T>* y = nullptr;
        BinarySearchTreeNode<T>* x = dynamic_cast<BinarySearchTreeNode<T>*>(this->getRootNode());
        while(x != nullptr){
            y = x;
            if(z->getValue() < x->getValue()) x = dynamic_cast<BinarySearchTreeNode<T>*>(x->getChild(LEFT_CHILD));
            else x = dynamic_cast<BinarySearchTreeNode<T>*>(x->getChild(RIGHT_CHILD));
        }
        z->overwriteParent(y);
        if(y == nullptr) this->setRootNode(z);
        else if(z->getValue() < y->getValue()) y->overwriteChild(z, LEFT_CHILD);
        else y->overwriteChild(z, RIGHT_CHILD);
    }

    void remove(BinarySearchTreeNode<T>* node){
        if(node == nullptr) return;

        auto parent = node->getParent();
        auto lchild = node->getChild(LEFT_CHILD);
        auto rchild = node->getChild(RIGHT_CHILD);
        //bool isParentLeftChild =

        if((lchild == nullptr) && (rchild == nullptr)){
            if(parent != nullptr) {
                if(parent->getChild(LEFT_CHILD) == node) parent->overwriteChild(nullptr, LEFT_CHILD);
                if(parent->getChild(RIGHT_CHILD) == node) parent->overwriteChild(nullptr, RIGHT_CHILD);
            }
            delete node;
        }else if((lchild != nullptr) && (rchild != nullptr)){
            auto replacement = node->successor();

        }
    }

    virtual ~BinarySearchTree() { }
};

template<class T>
using bst_traversal_funcion = std::function<void(BinaryTreeNode<T>*)>;

static int bst_test = push_test("Binary Search Tree", (test_function)[](){
    BinarySearchTree<int>* bst = new BinarySearchTree<int>(50);
    bst->insert(48);
    bst->insert(52);
    bst->insert(47);
    bst->insert(49);
    bst->insert(51);
    bst->insert(53);

    std::cout << bst->getRootNode()->getValue() << std::endl;

    bst_traversal_funcion<int> print_value = [](auto node){ std::cout << node->getValue() << " "; };
    bst->traversal<PREORDER>(print_value); std::cout << std::endl;
    bst->traversal<INORDER>(print_value); std::cout << std::endl;
    bst->traversal<POSTORDER>(print_value); std::cout << std::endl;
    
    std::cout << bst->search<BST_SEARCH_MIN>()->getValue() << std::endl;
    std::cout << bst->search<BST_SEARCH_MAX>()->getValue() << std::endl;
    std::cout << ((bst->search(52) != nullptr) ? "EXIST ":"NOT EXIST ");
    std::cout << ((bst->search(53) != nullptr) ? "EXIST ":"NOT EXIST ");
    std::cout << ((bst->search(54) != nullptr) ? "EXIST ":"NOT EXIST ") << std::endl;
    
    delete bst;

    return 0;
});
