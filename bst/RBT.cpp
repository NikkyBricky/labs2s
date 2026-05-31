#include "RBT.h"

#include <algorithm>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <utility>

BinarySearchTree::Node::Node(Key key, Value value, bool color,
                             Node* parent, Node* left, Node* right)
    : keyValuePair(key, value),
      parent(parent),
      left(left),
      right(right),
      color(color)
{
}

BinarySearchTree::Node::Node(const Node& other)
    : keyValuePair(other.keyValuePair)
{
}

bool BinarySearchTree::Node::operator==(const Node& other) const
{
    return keyValuePair == other.keyValuePair;
}

void BinarySearchTree::Node::output_node_tree(int level) const
{
    (void)level;

    if (left) left->output_node_tree(level + 1);

    if (keyValuePair.first == std::numeric_limits<Key>::max())
        return;

    std::cout << "(" << keyValuePair.first << "," << keyValuePair.second << ")\n";

    if (right) right->output_node_tree(level + 1);
}

void BinarySearchTree::Node::insert(const Key& key, const Value& value, Node** root)
{
    if (!root) return;

    if (key <= keyValuePair.first) {
        if (left) {
            left->insert(key, value, root);
            return;
        }

        left = new Node(key, value, true, this);
        left->insertRebalance(root);
        return;
    }

    if (right) {
        right->insert(key, value, root);
        return;
    }

    right = new Node(key, value, true, this);
    right->insertRebalance(root);
}

void BinarySearchTree::Node::erase(const Key& key, Node** root)
{
    if (!root) return;

    if (key != keyValuePair.first) {
        if (left && key <= keyValuePair.first) {
            left->erase(key, root);
        } else if (right) {
            right->erase(key, root);
        }
        return;
    }

    if (left && right) {
        Node* current = right;
        while (current->left) current = current->left;

        keyValuePair = current->keyValuePair;
        current->erase(current->keyValuePair.first, root);
        return;
    }

    if (color) {
        if (parent) {
            if (parent->left == this) parent->left = nullptr;
            else parent->right = nullptr;
        }
        delete this;
        return;
    }

    Node* child = left ? left : right;

    if (!child) {
        color = 0;
        eraseRebalance(root);

        if (parent) {
            if (parent->left == this) parent->left = child;
            else parent->right = child;
        }

        if (this == *root) *root = child;

        delete this;
        return;
    }

    if (parent) {
        if (parent->left == this) parent->left = child;
        else parent->right = child;
    }
    child->parent = parent;

    if (child->color) {
        child->color = false;
    } else {
        child->eraseRebalance(root);
    }

    if (this == *root) *root = child;

    delete this;
}

void BinarySearchTree::Node::rotateLeft()
{
    Node* pivot = right;

    pivot->parent = parent;
    if (parent) {
        if (parent->left == this) parent->left = pivot;
        else parent->right = pivot;
    }

    right = pivot->left;
    if (pivot->left) pivot->left->parent = this;

    parent = pivot;
    pivot->left = this;
}

void BinarySearchTree::Node::rotateRight()
{
    Node* pivot = left;

    pivot->parent = parent;
    if (parent) {
        if (parent->left == this) parent->left = pivot;
        else parent->right = pivot;
    }

    left = pivot->right;
    if (pivot->right) pivot->right->parent = this;

    parent = pivot;
    pivot->right = this;
}

void BinarySearchTree::Node::insertRebalance(Node** root)
{
    if (!parent) {
        color = 0;
        *root = this;
        return;
    }

    if (parent->color == false)
        return;

    Node* grand = parent->parent;
    Node* uncle = (grand->left == parent) ? grand->right : grand->left;

    if (uncle && uncle->color) {
        parent->color = false;
        uncle->color = false;
        grand->color = true;
        grand->insertRebalance(root);
        return;
    }

    Node* n = this;

    if (n == parent->right && parent == grand->left) {
        parent->rotateLeft();
        n = left;
    } else if (n == parent->left && parent == grand->right) {
        parent->rotateRight();
        n = right;
    }

    n->parent->color = false;
    n->parent->parent->color = true;

    if (n == n->parent->left && n->parent == n->parent->parent->left) {
        if (n->parent->parent == *root) *root = n->parent->parent->left;
        n->parent->parent->rotateRight();
    } else {
        if (n->parent->parent == *root) *root = n->parent->parent->right;
        n->parent->parent->rotateLeft();
    }
}

void BinarySearchTree::Node::eraseRebalance(Node** root)
{
    if (!parent) return;

    Node* sibling = (parent->left == this) ? parent->right : parent->left;

    if (sibling->color) {
        parent->color = 1;
        sibling->color = 0;

        if (parent->left == this) {
            if (parent == *root) *root = parent->right;
            parent->rotateLeft();
        } else {
            if (parent == *root) *root = parent->left;
            parent->rotateRight();
        }
    }

    if (!parent->color && !sibling->color &&
        (!sibling->left || !sibling->left->color) &&
        (!sibling->right || !sibling->right->color)) {
        sibling->color = 1;
        parent->eraseRebalance(root);
        return;
    }

    if (parent->color && !sibling->color &&
        (!sibling->left || !sibling->left->color) &&
        (!sibling->right || !sibling->right->color)) {
        sibling->color = 1;
        parent->color = 0;
        return;
    }

    if (!sibling->color) {
        if (this == parent->left &&
            (!sibling->right || !sibling->right->color) &&
            (sibling->left && sibling->left->color)) {
            sibling->color = 1;
            sibling->left->color = 0;
            sibling->rotateRight();
        } else if (this == parent->right &&
                   (sibling->right && sibling->right->color) &&
                   (!sibling->left || !sibling->left->color)) {
            sibling->color = 1;
            sibling->right->color = 0;
            sibling->rotateLeft();
        }
        return;
    }

    sibling->color = parent->color;
    parent->color = 0;

    if (this == parent->left) {
        if (sibling->right) sibling->right->color = 0;
        if (parent == *root) *root = parent->right;
        parent->rotateLeft();
    } else {
        if (sibling->left) sibling->left->color = 0;
        if (parent == *root) *root = parent->left;
        parent->rotateRight();
    }
}

size_t BinarySearchTree::Node::getMaxHeight() const
{
    if (keyValuePair.first == std::numeric_limits<Key>::max())
        return 0;

    size_t lh = left ? left->getMaxHeight() : 0;
    size_t rh = right ? right->getMaxHeight() : 0;

    return 1 + std::max(lh, rh);
}

BinarySearchTree::Node* BinarySearchTree::copySubtree(const Node* other)
{
    if (!other) return nullptr;

    Node* n = new Node(*other);
    n->left = copySubtree(other->left);
    n->right = copySubtree(other->right);

    return n;
}

void BinarySearchTree::freeSubtree(Node* node)
{
    if (!node) return;
    freeSubtree(node->left);
    freeSubtree(node->right);
    delete node;
}

BinarySearchTree::BinarySearchTree(const BinarySearchTree& other)
    : _size(other._size),
      _root(copySubtree(other._root))
{
}

BinarySearchTree& BinarySearchTree::operator=(const BinarySearchTree& other)
{
    if (this == &other) return *this;

    freeSubtree(_root);
    _root = copySubtree(other._root);
    _size = other._size;
    return *this;
}

BinarySearchTree::BinarySearchTree(BinarySearchTree&& other) noexcept
    : _size(other._size),
      _root(other._root)
{
    other._root = nullptr;
    other._size = 0;
}

BinarySearchTree& BinarySearchTree::operator=(BinarySearchTree&& other) noexcept
{
    if (this == &other) return *this;

    std::swap(_root, other._root);
    std::swap(_size, other._size);
    return *this;
}

BinarySearchTree::~BinarySearchTree()
{
    freeSubtree(_root);
}

BinarySearchTree::Iterator::Iterator(Node* node)
    : _node(node)
{
}

std::pair<Key, Value>& BinarySearchTree::Iterator::operator*()
{
    if (!_node) throw std::runtime_error("Null iterator");
    return _node->keyValuePair;
}

const std::pair<Key, Value>& BinarySearchTree::Iterator::operator*() const
{
    if (!_node) throw std::runtime_error("Null iterator");
    return _node->keyValuePair;
}

std::pair<Key, Value>* BinarySearchTree::Iterator::operator->()
{
    if (!_node) throw std::runtime_error("Null iterator");
    return &_node->keyValuePair;
}

const std::pair<Key, Value>* BinarySearchTree::Iterator::operator->() const
{
    if (!_node) throw std::runtime_error("Null iterator");
    return &_node->keyValuePair;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++()
{
    if (!_node) throw std::runtime_error("Null iterator");

    if (_node->right) {
        _node = _node->right;
        while (_node->left) _node = _node->left;
        return *this;
    }

    while (_node->parent) {
        if (_node == _node->parent->left) {
            _node = _node->parent;
            return *this;
        }
        _node = _node->parent;
    }

    throw std::runtime_error("++end");
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++(int)
{
    if (!_node) throw std::runtime_error("Null iterator");

    Iterator copy = *this;

    if (_node->right) {
        _node = _node->right;
        while (_node->left) _node = _node->left;
        return copy;
    }

    while (_node->parent) {
        if (_node == _node->parent->left) {
            _node = _node->parent;
            return copy;
        }
        _node = _node->parent;
    }

    throw std::runtime_error("end++");
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--()
{
    if (!_node) throw std::runtime_error("Null iterator");

    if (_node->left) {
        _node = _node->left;
        while (_node->right) _node = _node->right;
        return *this;
    }

    while (_node->parent) {
        if (_node == _node->parent->right) {
            _node = _node->parent;
            return *this;
        }
        _node = _node->parent;
    }

    throw std::runtime_error("--begin");
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--(int)
{
    if (!_node) throw std::runtime_error("Null iterator");

    Iterator copy = *this;

    if (_node->left) {
        _node = _node->left;
        while (_node->right) _node = _node->right;
        return copy;
    }

    while (_node->parent) {
        if (_node == _node->parent->right) {
            _node = _node->parent;
            return copy;
        }
        _node = _node->parent;
    }

    throw std::runtime_error("begin--");
}

bool BinarySearchTree::Iterator::operator==(const Iterator& other) const
{
    return _node == other._node;
}

bool BinarySearchTree::Iterator::operator!=(const Iterator& other) const
{
    return _node != other._node;
}

BinarySearchTree::ConstIterator::ConstIterator(const Node* node)
    : _node(node)
{
}

const std::pair<Key, Value>& BinarySearchTree::ConstIterator::operator*() const
{
    if (!_node) throw std::runtime_error("Null iterator");
    return _node->keyValuePair;
}

const std::pair<Key, Value>* BinarySearchTree::ConstIterator::operator->() const
{
    if (!_node) throw std::runtime_error("Null iterator");
    return &_node->keyValuePair;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++()
{
    if (!_node) throw std::runtime_error("Null iterator");

    if (_node->right) {
        _node = _node->right;
        while (_node->left) _node = _node->left;
        return *this;
    }

    while (_node->parent) {
        if (_node == _node->parent->left) {
            _node = _node->parent;
            return *this;
        }
        _node = _node->parent;
    }

    throw std::runtime_error("++end");
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++(int)
{
    if (!_node) throw std::runtime_error("Null iterator");

    ConstIterator copy = *this;

    if (_node->right) {
        _node = _node->right;
        while (_node->left) _node = _node->left;
        return copy;
    }

    while (_node->parent) {
        if (_node == _node->parent->left) {
            _node = _node->parent;
            return copy;
        }
        _node = _node->parent;
    }

    throw std::runtime_error("end++");
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--()
{
    if (!_node) throw std::runtime_error("Null iterator");

    if (_node->left) {
        _node = _node->left;
        while (_node->right) _node = _node->right;
        return *this;
    }

    while (_node->parent) {
        if (_node == _node->parent->right) {
            _node = _node->parent;
            return *this;
        }
        _node = _node->parent;
    }

    throw std::runtime_error("--begin");
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--(int)
{
    if (!_node) throw std::runtime_error("Null iterator");

    ConstIterator copy = *this;

    if (_node->left) {
        _node = _node->left;
        while (_node->right) _node = _node->right;
        return copy;
    }

    while (_node->parent) {
        if (_node == _node->parent->right) {
            _node = _node->parent;
            return copy;
        }
        _node = _node->parent;
    }

    throw std::runtime_error("begin--");
}

bool BinarySearchTree::ConstIterator::operator==(const ConstIterator& other) const
{
    return _node == other._node;
}

bool BinarySearchTree::ConstIterator::operator!=(const ConstIterator& other) const
{
    return _node != other._node;
}

void BinarySearchTree::insert(const Key& key, const Value& value)
{
    if (!_root) {
        _root = new Node(key, value);
        _root->right = new Node(std::numeric_limits<Key>::max(), value, false, _root);
        ++_size;
        return;
    }

    Node* endNode = _root;
    while (endNode->right) endNode = endNode->right;
    if (endNode->parent) endNode->parent->right = nullptr;

    _root->insert(key, value, &_root);
    ++_size;

    Node* current = _root;
    while (current->right) current = current->right;
    current->right = endNode;
    endNode->parent = current;
}

void BinarySearchTree::erase(const Key& key)
{
    if (!_root) return;

    Iterator it(_root);
    while ((it = find(key)) != end()) {
        Node* endNode = _root;
        while (endNode->right) endNode = endNode->right;
        endNode->parent->right = nullptr;

        it._node->erase(key, &_root);
        --_size;

        if (!_root) {
            delete endNode;
            return;
        }

        Node* current = _root;
        while (current->right) current = current->right;
        current->right = endNode;
        endNode->parent = current;
    }
}

BinarySearchTree::ConstIterator BinarySearchTree::find(const Key& key) const
{
    const Node* current = _root;

    while (current) {
        if (current->keyValuePair.first < key) {
            current = current->right;
        } else if (current->keyValuePair.first > key) {
            current = current->left;
        } else {
            return ConstIterator(current);
        }
    }

    return cend();
}

BinarySearchTree::Iterator BinarySearchTree::find(const Key& key)
{
    Node* current = _root;

    while (current) {
        if (current->keyValuePair.first < key) {
            current = current->right;
        } else if (current->keyValuePair.first > key) {
            current = current->left;
        } else {
            return Iterator(current);
        }
    }

    return end();
}

std::pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator>
BinarySearchTree::equalRange(const Key& key)
{
    Iterator e = end();
    Iterator current = begin();

    while (current != e && current->first != key) ++current;

    Iterator first = current;
    while (current != e && current->first == key) ++current;

    return {first, current};
}

std::pair<BinarySearchTree::ConstIterator, BinarySearchTree::ConstIterator>
BinarySearchTree::equalRange(const Key& key) const
{
    ConstIterator e = cend();
    ConstIterator current = cbegin();

    while (current != e && current->first != key) ++current;

    ConstIterator first = current;
    while (current != e && current->first == key) ++current;

    return {first, current};
}

BinarySearchTree::ConstIterator BinarySearchTree::min() const
{
    return cbegin();
}

BinarySearchTree::ConstIterator BinarySearchTree::max() const
{
    return --cend();
}

BinarySearchTree::ConstIterator BinarySearchTree::min(const Key& key) const
{
    std::pair<ConstIterator, ConstIterator> range = equalRange(key);

    ConstIterator current = range.first;
    ConstIterator best = current;

    for (; current != range.second; ++current) {
        if (current->second < best->second) best = current;
    }

    return best;
}

BinarySearchTree::ConstIterator BinarySearchTree::max(const Key& key) const
{
    std::pair<ConstIterator, ConstIterator> range = equalRange(key);

    ConstIterator current = range.first;
    ConstIterator best = current;

    for (; current != range.second; ++current) {
        if (current->second > best->second) best = current;
    }

    return best;
}

BinarySearchTree::Iterator BinarySearchTree::begin()
{
    Node* current = _root;
    if (!current) return Iterator(nullptr);
    while (current->left) current = current->left;
    return Iterator(current);
}

BinarySearchTree::Iterator BinarySearchTree::end()
{
    Node* current = _root;
    if (!current) return Iterator(nullptr);
    while (current->right) current = current->right;
    return Iterator(current);
}

BinarySearchTree::ConstIterator BinarySearchTree::cbegin() const
{
    const Node* current = _root;
    if (!current) return ConstIterator(nullptr);
    while (current->left) current = current->left;
    return ConstIterator(current);
}

BinarySearchTree::ConstIterator BinarySearchTree::cend() const
{
    const Node* current = _root;
    if (!current) return ConstIterator(nullptr);
    while (current->right) current = current->right;
    return ConstIterator(current);
}

size_t BinarySearchTree::size() const
{
    return _size;
}

void BinarySearchTree::output_tree() const
{
    if (!_root) return;
    _root->output_node_tree();
}

size_t BinarySearchTree::max_height() const
{
    return _root ? _root->getMaxHeight() : 0;
}
