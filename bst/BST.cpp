#include "BST.h"
#include <limits>
#include <iostream>

BinarySearchTree::Node::Node(Key key, Value value, Node *parent, Node *left, Node *right)
    : keyValuePair(key, value), parent(parent), left(left), right(right) {}

BinarySearchTree::Node::Node(const Node &other)
    : keyValuePair(other.keyValuePair), parent(nullptr), left(nullptr), right(nullptr) {
    if (other.left) {
        left = new Node(*other.left);
        left->parent = this;
    }

    if (other.right) {
        right = new Node(*other.right);
        right->parent = this;
    }
}

bool BinarySearchTree::Node::operator==(const Node &other) const {
    return keyValuePair == other.keyValuePair;
}

void BinarySearchTree::Node::output_node_tree() const {
    if (left != nullptr) {
        left->output_node_tree();
    }
    if (this->keyValuePair.first == std::numeric_limits<Key>::max()) return;
    std::cout << "Ключ: " << keyValuePair.first << " значение: " << keyValuePair.second << std::endl;
    if (right != nullptr) {
        right->output_node_tree();
    }
}

void BinarySearchTree::Node::insert(const Key &key, const Value &value) {
    if (key < keyValuePair.first) {
        if (left != nullptr) left->insert(key, value);
        else left = new Node(key, value, this);
    } else {
        if (right != nullptr) right->insert(key, value);
        else right = new Node(key, value, this);
    }
}

void BinarySearchTree::Node::erase(const Key &key) {
    if (key != keyValuePair.first) {
        if (left != nullptr && key < keyValuePair.first) left->erase(key);
        else if (right != nullptr) right->erase(key);
        return;
    }

    if (left != nullptr && right != nullptr) {
        Node* currentNode = right;
        while (currentNode->left) currentNode = currentNode->left;
        this->keyValuePair = currentNode->keyValuePair;
        currentNode->erase(currentNode->keyValuePair.first);
        return;
    }

    Node* replacement = nullptr;
    if (left != nullptr) replacement = left;
    else replacement = right;

    if (parent != nullptr) {
        if (parent->left == this) parent->left = replacement;
        else parent->right = replacement;
    }

    if (replacement != nullptr) replacement->parent = parent;
    delete this;
}

void BinarySearchTree::freeSubtree(Node* node) {
    if (!node) return;
    freeSubtree(node->left);
    freeSubtree(node->right);
    delete node;
}

BinarySearchTree::BinarySearchTree(const BinarySearchTree &other) : _size(other._size) {
    if (other._root != nullptr) _root = new Node(*other._root);
}

BinarySearchTree& BinarySearchTree::operator=(const BinarySearchTree &other) {
    if (this == &other) return *this;
    freeSubtree(_root);
    _root = nullptr;
    _size = 0;
    if (other._root != nullptr) _root = new Node(*other._root);
    _size = other._size;
    return *this;
}

BinarySearchTree::BinarySearchTree(BinarySearchTree &&other) noexcept {
    _root = other._root;
    _size = other._size;
    other._root = nullptr;
    other._size = 0;
}

BinarySearchTree& BinarySearchTree::operator=(BinarySearchTree &&other) noexcept {
    if (this == &other) return *this;
    _root = other._root;
    _size = other._size;
    other._root = nullptr;
    other._size = 0;
    return *this;
}

BinarySearchTree::~BinarySearchTree() {
    freeSubtree(_root);
}

BinarySearchTree::Iterator::Iterator(Node *node) : _node(node) {}

std::pair<Key, Value>& BinarySearchTree::Iterator::operator*() {
    if (_node == nullptr) throw std::runtime_error("Null pointer");
    return _node->keyValuePair;
}

const std::pair<Key, Value>& BinarySearchTree::Iterator::operator*() const {
    if (_node == nullptr) throw std::runtime_error("Null pointer");
    return _node->keyValuePair;
}

std::pair<Key, Value>* BinarySearchTree::Iterator::operator->() {
    if (_node == nullptr) throw std::runtime_error("Null pointer");
    return &_node->keyValuePair;
}

const std::pair<Key, Value>* BinarySearchTree::Iterator::operator->() const {
    if (_node == nullptr) throw std::runtime_error("Null pointer");
    return &_node->keyValuePair;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++() {
    if (_node == nullptr) throw std::runtime_error("Null pointer");
    if (_node->right != nullptr) {
        _node = _node->right;
        while (_node->left != nullptr) {
            _node = _node->left;
        }
        return *this;
    }

    while (_node->parent != nullptr) {
        if (_node == _node->parent->left) {
            _node = _node->parent;
            return *this;
        }
        _node = _node->parent;
    }
    throw std::runtime_error("end of the iterator!");
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++(int) {
    Iterator temp(*this);
    ++(*this);
    return temp;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--() {
    if (_node == nullptr) throw std::runtime_error("Null pointer");
    if (_node->left != nullptr) {
        _node = _node->left;
        while (_node->right != nullptr) {
            _node = _node->right;
        }
        return *this;
    }

    while (_node->parent != nullptr) {
        if (_node == _node->parent->right) {
            _node = _node->parent;
            return *this;
        }
        _node = _node->parent;
    }
    throw std::runtime_error("begin of the iterator!");
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--(int) {
    Iterator temp(*this);
    --(*this);
    return temp;
}

bool BinarySearchTree::Iterator::operator==(const Iterator &other) const {
    return _node == other._node;
}

bool BinarySearchTree::Iterator::operator!=(const Iterator &other) const {
    return _node != other._node;
}

BinarySearchTree::ConstIterator::ConstIterator(const Node *node) : _node(node) {}

const std::pair<Key, Value>& BinarySearchTree::ConstIterator::operator*() const {
    if (_node == nullptr) throw std::runtime_error("Null pointer");
    return _node->keyValuePair;
}

const std::pair<Key, Value>* BinarySearchTree::ConstIterator::operator->() const {
    if (_node == nullptr) throw std::runtime_error("Null pointer");
    return &_node->keyValuePair;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++() {
    if (_node == nullptr) throw std::runtime_error("Null pointer");
    if (_node->right != nullptr) {
        _node = _node->right;
        while (_node->left != nullptr) {
            _node = _node->left;
        }
        return *this;
    }

    while (_node->parent != nullptr) {
        if (_node == _node->parent->left) {
            _node = _node->parent;
            return *this;
        }
        _node = _node->parent;
    }
    throw std::runtime_error("end of the iterator!");
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++(int) {
    ConstIterator temp(*this);
    ++(*this);
    return temp;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--() {
    if (_node == nullptr) throw std::runtime_error("Null pointer");
    if (_node->left != nullptr) {
        _node = _node->left;
        while (_node->right != nullptr) {
            _node = _node->right;
        }
        return *this;
    }

    while (_node->parent != nullptr) {
        if (_node == _node->parent->right) {
            _node = _node->parent;
            return *this;
        }
        _node = _node->parent;
    }
    throw std::runtime_error("begin of the iterator!");
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--(int) {
    ConstIterator temp(*this);
    --(*this);
    return temp;
}

bool BinarySearchTree::ConstIterator::operator==(const ConstIterator &other) const {
    return _node == other._node;
}

bool BinarySearchTree::ConstIterator::operator!=(const ConstIterator &other) const {
    return _node != other._node;
}

void BinarySearchTree::insert(const Key &key, const Value &value) {
    if (_root == nullptr) {
        _root = new Node(key, value);
        _root->right = new Node(std::numeric_limits<Key>::max(), value, _root);
    } else {
        _root->insert(key, value);
    }
    _size++;
}

void BinarySearchTree::erase(const Key &key) {
    if (_root == nullptr) return;
    Iterator it(_root);

    while ((it = find(key)) != end()) {
        it._node->erase(key);
        _size--;
    }
}

BinarySearchTree::ConstIterator BinarySearchTree::find(const Key &key) const {
    Node *cur = _root;
    while (cur != nullptr) {
        if (key < cur->keyValuePair.first) cur = cur->left;
        else if (key > cur->keyValuePair.first) cur = cur->right;
        else return ConstIterator(cur);
    }
    return cend();
}

BinarySearchTree::Iterator BinarySearchTree::find(const Key &key) {
    Node *cur = _root;
    while (cur != nullptr) {
        if (key < cur->keyValuePair.first) cur = cur->left;
        else if (key > cur->keyValuePair.first) cur = cur->right;
        else return Iterator(cur);
    }
    return end();
}

std::pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator>
BinarySearchTree::equalRange(const Key &key) {
    Iterator first = find(key);
    if (first == end()) return {end(), end()};
    Iterator second = first;
    while (second != end() && (*second).first == key) ++second;
    return {first, second};
}

std::pair<BinarySearchTree::ConstIterator, BinarySearchTree::ConstIterator>
BinarySearchTree::equalRange(const Key &key) const {
    ConstIterator first = find(key);
    if (first == cend()) return {cend(), cend()};
    ConstIterator second = first;
    while (second != cend() && (*second).first == key) ++second;
    return {first, second};
}

BinarySearchTree::ConstIterator BinarySearchTree::min() const {
    Node *cur = _root;
    if (cur == nullptr) return cend();
    while (cur->left) cur = cur->left;
    return ConstIterator(cur);
}

BinarySearchTree::ConstIterator BinarySearchTree::max() const {
    if (_root == nullptr) return cend();
    return --cend();
}

BinarySearchTree::ConstIterator BinarySearchTree::min(const Key &key) const {
    auto range = equalRange(key);
    if (range.first == cend()) return cend();

    ConstIterator minIt = range.first;
    for (auto i = range.first; i != range.second; i++) {
        if ((*minIt).second > (*i).second) minIt = i;
    }
    return minIt;
}

BinarySearchTree::ConstIterator BinarySearchTree::max(const Key &key) const {
    auto range = equalRange(key);
    if (range.first == cend()) return cend();

    ConstIterator maxIt = range.first;
    for (auto i = range.first; i != range.second; i++) {
        if ((*maxIt).second < (*i).second) maxIt = i;
    }
    return maxIt;
}

BinarySearchTree::Iterator BinarySearchTree::begin() {
    Node *cur = _root;
    while (cur->left != nullptr) cur = cur->left;
    return Iterator(cur);
}

BinarySearchTree::Iterator BinarySearchTree::end() {
    Node *cur = _root;
    while (cur->right != nullptr) cur = cur->right;
    return Iterator(cur);
}

BinarySearchTree::ConstIterator BinarySearchTree::cbegin() const {
    Node *cur = _root;
    while (cur->left) cur = cur->left;
    return ConstIterator(cur);
}

BinarySearchTree::ConstIterator BinarySearchTree::cend() const {
    Node *cur = _root;
    while (cur->right != nullptr) cur = cur->right;
    return ConstIterator(cur);
}

size_t BinarySearchTree::size() const {
    return _size;
}

void BinarySearchTree::output_tree() {
    if (_root) _root->output_node_tree();
}
