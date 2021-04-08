template<class T>
class Node {
protected:
    T value;
public:
    Node() {}
    Node(T val) : value(val) {}
    Node(const Node<T>& node) { this->value = node.value; }
    Node(Node<T>&& node) { this->value = node.value; }

    T getValue() const { return value; }
    void setValue(T val) { value = val; }

    virtual ~Node() {}
};

template<class T, std::size_t LINK_SIZE>
class LinkableNode : public Node<T> {
protected:
    std::array<LinkableNode<T, LINK_SIZE>*, LINK_SIZE> link = { nullptr };
public:
    LinkableNode() {}
    LinkableNode(T val) { this->value = val; }
    //Copy constructor.
    //Usually we should do a deep copy, but considering deep copy may cause an infinite recursion here, so we do a shallow copy.
    LinkableNode(const LinkableNode<T, LINK_SIZE>& node) {
        this->value = node.value;
        for(std::size_t i = 0; i < LINK_SIZE; i++) link[i] = node.link[i];
    }
    //Move constructor.
    //We can't move the elements in link array since we can't tell whether if the linked node inside a movable object is useless or not.
    //In other words, the node is decaying, but the linked nodes are not.
    //And since std::array is not movable, so we just do a shallow copy the same as the copy constructor.
    LinkableNode(LinkableNode<T, LINK_SIZE>&& node){
        this->value = node.value;
        for(std::size_t i = 0; i < LINK_SIZE; i++) {
            link[i] = node.link[i];
            node.link[i] = nullptr; //There's no reason to do so since the destuctor doesn't delete any linked nodes.
            //It's only for the sake of good practice.
        }
        
    }

    //For linkable node, there is no reason to delete before replacement since there's no way to tell whether if it is useless or not.
    //So be careful using it, always make sure the replaced node inside link array is referenced somewhere and got proper deletion somehow.
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

    BinarySearchTreeNode<T>* getChild(BTREE_CHILD_IDENTIFIER i) const {
        return dynamic_cast<BinarySearchTreeNode<T>*>(this->link[i]);
    }
    BinarySearchTreeNode<T>* getParent() const {
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
    BinarySearchTreeNode<T>* deepCopy(){
        BinarySearchTreeNode<T>* lchild = nullptr;
        BinarySearchTreeNode<T>* rchild = nullptr;
        if(this->link[LEFT_CHILD] != nullptr) lchild = this->getChild(LEFT_CHILD)->deepCopy();
        if(this->link[RIGHT_CHILD] != nullptr) rchild = this->getChild(RIGHT_CHILD)->deepCopy();
        BinarySearchTreeNode<T>* selfCopy = new BinarySearchTreeNode<T>();
        selfCopy->value = this->value;
        selfCopy->link[LEFT_CHILD] = lchild;
        selfCopy->link[RIGHT_CHILD] = rchild;
        return selfCopy;
    }

    virtual ~BinarySearchTreeNode() { }
};

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

    BinarySearchTreeNode<T>* getRootNode() const { return dynamic_cast<BinarySearchTreeNode<T>*>(this->rootNode); }
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
using btree_traversal_funcion = std::function<bool(BinaryTreeNode<T>*)>;

static int bst_test = push_test("Binary Search Tree", (test_function)[](){
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

    return 0;
});
