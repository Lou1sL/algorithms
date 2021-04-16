#pragma once

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