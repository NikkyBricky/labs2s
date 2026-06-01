#pragma once

#include <cstddef>

template<typename Key, typename Value>
class SplayTree {
private:
    struct Node {
        Key key;
        Value value;
        Node* parent;
        Node* left;
        Node* right;

        Node(const Key& k, const Value& v, Node* p = nullptr)
            : key(k),
              value(v),
              parent(p),
              left(nullptr),
              right(nullptr)
        {}
    };

    Node* root_;
    std::size_t size_;

    void clear(Node* node)
    {
        if (!node) {
            return;
        }

        clear(node->left);
        clear(node->right);
        delete node;
    }

    static Node* subtreeMin(Node* node)
    {
        if (!node) {
            return nullptr;
        }

        while (node->left) {
            node = node->left;
        }

        return node;
    }

    static Node* subtreeMax(Node* node)
    {
        if (!node) {
            return nullptr;
        }

        while (node->right) {
            node = node->right;
        }

        return node;
    }

    void rotateLeft(Node* x)
    {
        if (!x || !x->right) {
            return;
        }

        Node* y = x->right;

        x->right = y->left;
        if (y->left) {
            y->left->parent = x;
        }

        y->parent = x->parent;

        if (!x->parent) {
            root_ = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }

        y->left = x;
        x->parent = y;
    }

    void rotateRight(Node* x)
    {
        if (!x || !x->left) {
            return;
        }

        Node* y = x->left;

        x->left = y->right;
        if (y->right) {
            y->right->parent = x;
        }

        y->parent = x->parent;

        if (!x->parent) {
            root_ = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }

        y->right = x;
        x->parent = y;
    }

    void splay(Node* x)
    {
        if (!x) {
            return;
        }

        while (x->parent) {
            Node* p = x->parent;
            Node* g = p->parent;

            if (!g) {
                if (x == p->left) {
                    rotateRight(p);
                } else {
                    rotateLeft(p);
                }
            } else if (x == p->left && p == g->left) {
                rotateRight(g);
                rotateRight(p);
            } else if (x == p->right && p == g->right) {
                rotateLeft(g);
                rotateLeft(p);
            } else if (x == p->right && p == g->left) {
                rotateLeft(p);
                rotateRight(g);
            } else {
                rotateRight(p);
                rotateLeft(g);
            }
        }

        root_ = x;
    }

    Node* findNodeSplay(const Key& key)
    {
        Node* curr = root_;
        Node* last = nullptr;

        while (curr) {
            last = curr;

            if (key < curr->key) {
                curr = curr->left;
            } else if (curr->key < key) {
                curr = curr->right;
            } else {
                splay(curr);
                return curr;
            }
        }

        if (last) {
            splay(last);
        }

        return nullptr;
    }

    const Node* findNodeConst(const Key& key) const
    {
        Node* curr = root_;

        while (curr) {
            if (key < curr->key) {
                curr = curr->left;
            } else if (curr->key < key) {
                curr = curr->right;
            } else {
                return curr;
            }
        }

        return nullptr;
    }

    bool isValidBST(Node* node, const Key* minKey, const Key* maxKey) const
    {
        if (!node) {
            return true;
        }

        if (minKey && !(*minKey < node->key)) {
            return false;
        }

        if (maxKey && !(node->key < *maxKey)) {
            return false;
        }

        return isValidBST(node->left, minKey, &node->key) &&
               isValidBST(node->right, &node->key, maxKey);
    }

public:
    SplayTree()
        : root_(nullptr),
          size_(0)
    {
    }

    ~SplayTree()
    {
        clear(root_);
        root_ = nullptr;
        size_ = 0;
    }

    SplayTree(const SplayTree&) = delete;
    SplayTree& operator=(const SplayTree&) = delete;
    SplayTree(SplayTree&&) = delete;
    SplayTree& operator=(SplayTree&&) = delete;

    void insert(const Key& key, const Value& value)
    {
        if (!root_) {
            root_ = new Node(key, value);
            size_ = 1;
            return;
        }

        Node* curr = root_;
        Node* parent = nullptr;

        while (curr) {
            parent = curr;

            if (key < curr->key) {
                curr = curr->left;
            } else if (curr->key < key) {
                curr = curr->right;
            } else {
                curr->value = value;
                splay(curr);
                return;
            }
        }

        Node* node = new Node(key, value, parent);

        if (key < parent->key) {
            parent->left = node;
        } else {
            parent->right = node;
        }

        splay(node);
        ++size_;
    }

    bool remove(const Key& key)
    {
        if (!root_) {
            return false;
        }

        Node* found = findNodeSplay(key);

        if (!found || !(found->key == key)) {
            return false;
        }

        Node* leftSubtree = root_->left;
        Node* rightSubtree = root_->right;

        if (leftSubtree) {
            leftSubtree->parent = nullptr;
        }

        if (rightSubtree) {
            rightSubtree->parent = nullptr;
        }

        delete root_;
        root_ = nullptr;

        if (!leftSubtree) {
            root_ = rightSubtree;
        } else {
            root_ = leftSubtree;
            Node* maxLeft = subtreeMax(root_);
            splay(maxLeft);
            root_->right = rightSubtree;
            if (rightSubtree) {
                rightSubtree->parent = root_;
            }
        }

        --size_;
        return true;
    }

    Value* search(const Key& key)
    {
        Node* found = findNodeSplay(key);

        if (found && found->key == key) {
            return &found->value;
        }

        return nullptr;
    }

    const Value* search(const Key& key) const
    {
        const Node* found = findNodeConst(key);

        if (found) {
            return &found->value;
        }

        return nullptr;
    }

    bool isValidBST() const
    {
        return isValidBST(root_, nullptr, nullptr);
    }

    std::size_t size() const
    {
        return size_;
    }

    bool empty() const
    {
        return size_ == 0;
    }
};


