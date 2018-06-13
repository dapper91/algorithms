#include <iostream>
#include <cassert>


template <typename T>
class AVLTree {
    struct Node {
        Node* left;
        Node* right;
        size_t depth;

        T value;

        Node(const T& value):
            left(nullptr), right(nullptr), value(value), depth(1)
        {}

        ~Node()
        {
            delete left;
            delete right;
        }
    };

public:
    AVLTree():
        root(nullptr)
    {}

    ~AVLTree()
    {
        delete root;
    }

    void insert(const T& value)
    {
        root = insert(root, value);
    }

    void remove(const T& value)
    {
        root = remove(root, value);
    }

    bool contains(const T& value)
    {
        return find(root, value) != nullptr;
    }

    void print_in_order()
    {
        print_in_order(root, 1);
    }

private:
    Node* root;

    Node* remove(Node* node, const T& value)
    {
        if (node) {
            if (value == node->value) {
                if (!node->left && !node->right) {
                	delete node;
                    return nullptr;
                }
                else {
                    if (depth(node->left) > depth(node->right)) {
                        Node* max = find_max(node->left);
                        node->value = max->value;
                        node->left = remove(node->left, max->value);
                    }
                    else {
                        Node* min = find_min(node->right);
                        node->value = min->value;
                        node->right = remove(node->right, min->value);
                    }
                }
            }
            else if (value <= node->value) {
                node->left = remove(node->left, value);                
            }
            else {
                node->right = remove(node->right, value);
            }

            node = balance(node);
        }

        return node;
    }

    int left_depth(Node* node)
    {
        assert(node);

        return node->left ? node->left->depth : 0;
    }

    int right_depth(Node* node)
    {
        assert(node);

        return node->right ? node->right->depth : 0;
    }

    int depth(Node* node)
    {
        return std::max(left_depth(node), right_depth(node)) + 1;
    }

    Node* small_left_rotation(Node* a)
    {
        assert(a);

        Node* b = a->right;

        a->right = b->left;
        b->left = a;

        a->depth = depth(a);
        b->depth = depth(b);

        return b;
    }

    Node* small_right_rotation(Node* a)
    {
        assert(a);

        Node* b = a->left;

        a->left = b->right;
        b->right = a;

        a->depth = depth(a);
        b->depth = depth(b);        
        return b;
    }

    Node* big_left_rotation(Node* a)
    {
        assert(a);

        Node* b = a->right;
        Node* c = a->right->left;

        b->left = c->right;
        a->right = c->left;
        c->left = a;
        c->right = b;

        b->depth = depth(b);
        a->depth = depth(a);
        c->depth = depth(c);

        return c;
    }

    Node* big_right_rotation(Node* a)
    {
        assert(a);

        Node* b = a->left;
        Node* c = a->left->right;

        b->right = c->left;
        a->left = c->right;
        c->left = b;
        c->right = a;

        b->depth = depth(b);
        a->depth = depth(a);
        c->depth = depth(c);

        return c;
    }

    Node* balance(Node* node)
    {
        assert(node);

        if (left_depth(node) - right_depth(node) > 1) {
            if (right_depth(node->left) <= left_depth(node->left)) {
                node = small_right_rotation(node);
            }
            else {
                node = big_right_rotation(node);
            }
        }
        else if (right_depth(node) - left_depth(node) > 1) {
            if (left_depth(node->right) <= right_depth(node->right)) {
                node = small_left_rotation(node);
            }
            else {
                node = big_left_rotation(node);
            }
        }

        node->depth = depth(node);

        return node;
    }

    Node* insert(Node* node, const T& value)
    {
        if (node == nullptr) {
            return new Node(value);
        }
        else {
            if (value <= node->value) {
                node->left = insert(node->left, value);             
            }
            else {
                node->right = insert(node->right, value);
            }

            node = balance(node);

            return node;
        }
    }

    Node* find(Node* node, const T& value)
    {
        if (node) {
            if (value == node->value) {
                return node;
            }
            else if (value < node->value) {
                return find(node->left, value);
            }
            else {
                return find(node->right, value);
            }
        }
        else {
            return nullptr;
        }
    }

    Node* find_max(Node* node) 
    {
        assert(node);

        if (node->right) {
            return find_max(node->right);
        }
        else {
            return node;
        }
    }

    Node* find_min(Node* node) 
    {
        assert(node);

        if (node->left) {
            return find_max(node->left);
        }
        else {
            return node;
        }
    }
    
    void print_in_order(Node* node, int level)
    {
        if (node) {
            print_in_order(node->left, level + 1);
            std::cout << node->value << ":" << level << std::endl;
            print_in_order(node->right, level + 1);
        }
    }
};



int main()
{
    AVLTree<int> tree;

    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);
    tree.insert(5);
    tree.insert(6);
    tree.insert(7);
    tree.insert(8);
    tree.insert(9);

    tree.print_in_order();

    tree.remove(4);

    std::cout << std::endl;

    tree.print_in_order();

    return 0;
}