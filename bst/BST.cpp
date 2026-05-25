#include "BST.h"
#include <iostream>
#include <limits>
#include <stdexcept>

BinarySearchTree::Node::Node(Key k, Value v, Node* p, Node* l, Node* r)
    : keyValuePair{k, v}, parent(p), left(l), right(r) {}

BinarySearchTree::Node::Node(const Node& other)
    : keyValuePair(other.keyValuePair), parent(nullptr), left(nullptr), right(nullptr)
{
    if (other.left) {
        left = new Node(*other.left);
        left->parent = this;
    }
    if (other.right) {
        right = new Node(*other.right);
        right->parent = this;
    }
}

bool BinarySearchTree::Node::operator==(const Node& other) const {
    return keyValuePair == other.keyValuePair;
}

void BinarySearchTree::Node::output_node_tree() const {
    if (left) left->output_node_tree();

    if (keyValuePair.first != std::numeric_limits<Key>::max()) {
        std::cout << keyValuePair.first << " " << keyValuePair.second << "\n";
    }

    if (right) right->output_node_tree();
}

void BinarySearchTree::Node::insert(const Key& k, const Value& v) {
    if (k < keyValuePair.first) {
        if (left) left->insert(k, v);
        else left = new Node(k, v, this);
    } else {
        if (right) right->insert(k, v);
        else right = new Node(k, v, this);
    }
}

void BinarySearchTree::Node::erase(const Key& k) {
    if (k < keyValuePair.first) {
        if (left) left->erase(k);
        return;
    }
    if (k > keyValuePair.first) {
        if (right) right->erase(k);
        return;
    }

    if (left && right) {
        Node* succ = right;
        while (succ->left) succ = succ->left;

        keyValuePair = succ->keyValuePair;
        succ->erase(succ->keyValuePair.first);
        return;
    }

    Node* childNode = left ? left : right;

    if (parent) {
        if (parent->left == this) parent->left = childNode;
        else parent->right = childNode;
    }

    if (childNode) childNode->parent = parent;

    delete this;
}

void BinarySearchTree::freeSubtree(Node* rootNode) {
    if (!rootNode) return;
    freeSubtree(rootNode->left);
    freeSubtree(rootNode->right);
    delete rootNode;
}

BinarySearchTree::BinarySearchTree(const BinarySearchTree& otherTree)
    : _size(otherTree._size)
{
    if (otherTree._root)
        _root = new Node(*otherTree._root);
}

BinarySearchTree& BinarySearchTree::operator=(const BinarySearchTree& otherTree) {
    if (this == &otherTree) return *this;

    freeSubtree(_root);
    _root = nullptr;
    _size = otherTree._size;

    if (otherTree._root)
        _root = new Node(*otherTree._root);

    return *this;
}

BinarySearchTree::BinarySearchTree(BinarySearchTree&& otherTree) noexcept {
    _root = otherTree._root;
    _size = otherTree._size;

    otherTree._root = nullptr;
    otherTree._size = 0;
}

BinarySearchTree& BinarySearchTree::operator=(BinarySearchTree&& otherTree) noexcept {
    if (this == &otherTree) return *this;

    freeSubtree(_root);

    _root = otherTree._root;
    _size = otherTree._size;

    otherTree._root = nullptr;
    otherTree._size = 0;

    return *this;
}

BinarySearchTree::~BinarySearchTree() {
    freeSubtree(_root);
}

BinarySearchTree::Iterator::Iterator(Node* n) : _node(n) {}

std::pair<Key, Value>& BinarySearchTree::Iterator::operator*() {
    if (!_node) throw std::runtime_error("null");
    return _node->keyValuePair;
}

const std::pair<Key, Value>& BinarySearchTree::Iterator::operator*() const {
    if (!_node) throw std::runtime_error("null");
    return _node->keyValuePair;
}

std::pair<Key, Value>* BinarySearchTree::Iterator::operator->() {
    return &_node->keyValuePair;
}

const std::pair<Key, Value>* BinarySearchTree::Iterator::operator->() const {
    return &_node->keyValuePair;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++() {
    if (!_node) throw std::runtime_error("null");

    if (_node->right) {
        _node = _node->right;
        while (_node->left) _node = _node->left;
        return *this;
    }

    while (_node->parent) {
        Node* p = _node->parent;
        if (_node == p->left) {
            _node = p;
            return *this;
        }
        _node = p;
    }

    throw std::runtime_error("end");
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++(int) {
    Iterator tmp = *this;
    ++(*this);
    return tmp;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--() {
    if (!_node) throw std::runtime_error("null");

    if (_node->left) {
        _node = _node->left;
        while (_node->right) _node = _node->right;
        return *this;
    }

    while (_node->parent) {
        Node* p = _node->parent;
        if (_node == p->right) {
            _node = p;
            return *this;
        }
        _node = p;
    }

    throw std::runtime_error("begin");
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--(int) {
    Iterator tmp = *this;
    --(*this);
    return tmp;
}

bool BinarySearchTree::Iterator::operator==(const Iterator& otherIt) const {
    return _node == otherIt._node;
}

bool BinarySearchTree::Iterator::operator!=(const Iterator& otherIt) const {
    return _node != otherIt._node;
}

BinarySearchTree::ConstIterator::ConstIterator(const Node* n) : _node(n) {}

const std::pair<Key, Value>& BinarySearchTree::ConstIterator::operator*() const {
    if (!_node) throw std::runtime_error("null");
    return _node->keyValuePair;
}

const std::pair<Key, Value>* BinarySearchTree::ConstIterator::operator->() const {
    return &_node->keyValuePair;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++() {
    return ConstIterator(
        const_cast<Node*>(
            (++Iterator(const_cast<Node*>(_node)))._node
        )
    );
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++(int) {
    ConstIterator tmp = *this;
    ++(*this);
    return tmp;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--() {
    return ConstIterator(
        const_cast<Node*>(
            (--Iterator(const_cast<Node*>(_node)))._node
        )
    );
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--(int) {
    ConstIterator tmp = *this;
    --(*this);
    return tmp;
}

bool BinarySearchTree::ConstIterator::operator==(const ConstIterator& otherIt) const {
    return _node == otherIt._node;
}

bool BinarySearchTree::ConstIterator::operator!=(const ConstIterator& otherIt) const {
    return _node != otherIt._node;
}

void BinarySearchTree::insert(const Key& k, const Value& v) {
    if (!_root) {
        _root = new Node(k, v);
        _root->right = new Node(std::numeric_limits<Key>::max(), v, _root);
    } else {
        _root->insert(k, v);
    }
    ++_size;
}

void BinarySearchTree::erase(const Key& k) {
    if (!_root) return;

    auto it = find(k);
    while (it != end()) {
        it._node->erase(k);
        --_size;
        it = find(k);
    }
}

BinarySearchTree::ConstIterator BinarySearchTree::find(const Key& k) const {
    Node* cur = _root;

    while (cur) {
        if (k < cur->keyValuePair.first) cur = cur->left;
        else if (k > cur->keyValuePair.first) cur = cur->right;
        else return ConstIterator(cur);
    }

    return cend();
}

BinarySearchTree::Iterator BinarySearchTree::find(const Key& k) {
    Node* cur = _root;

    while (cur) {
        if (k < cur->keyValuePair.first) cur = cur->left;
        else if (k > cur->keyValuePair.first) cur = cur->right;
        else return Iterator(cur);
    }

    return end();
}

std::pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator>
BinarySearchTree::equalRange(const Key& k) {
    Iterator firstIt = find(k);
    if (firstIt == end()) return {end(), end()};

    Iterator lastIt = firstIt;
    while (lastIt != end() && (*lastIt).first == k) ++lastIt;

    return {firstIt, lastIt};
}

std::pair<BinarySearchTree::ConstIterator, BinarySearchTree::ConstIterator>
BinarySearchTree::equalRange(const Key& k) const {
    ConstIterator firstIt = find(k);
    if (firstIt == cend()) return {cend(), cend()};

    ConstIterator lastIt = firstIt;
    while (lastIt != cend() && (*lastIt).first == k) ++lastIt;

    return {firstIt, lastIt};
}

BinarySearchTree::ConstIterator BinarySearchTree::min() const {
    if (!_root) return cend();

    Node* cur = _root;
    while (cur->left) cur = cur->left;
    return ConstIterator(cur);
}

BinarySearchTree::ConstIterator BinarySearchTree::max() const {
    if (!_root) return cend();

    Node* cur = _root;
    while (cur->right) cur = cur->right;
    return ConstIterator(cur);
}

BinarySearchTree::ConstIterator BinarySearchTree::min(const Key& k) const {
    auto range = equalRange(k);
    if (range.first == cend()) return cend();

    ConstIterator best = range.first;
    for (auto it = range.first; it != range.second; ++it)
        if ((*it).second < (*best).second)
            best = it;

    return best;
}

BinarySearchTree::ConstIterator BinarySearchTree::max(const Key& k) const {
    auto range = equalRange(k);
    if (range.first == cend()) return cend();

    ConstIterator best = range.first;
    for (auto it = range.first; it != range.second; ++it)
        if ((*it).second > (*best).second)
            best = it;

    return best;
}

BinarySearchTree::Iterator BinarySearchTree::begin() {
    Node* cur = _root;
    while (cur && cur->left) cur = cur->left;
    return Iterator(cur);
}

BinarySearchTree::Iterator BinarySearchTree::end() {
    Node* cur = _root;
    while (cur && cur->right) cur = cur->right;
    return Iterator(cur);
}

BinarySearchTree::ConstIterator BinarySearchTree::cbegin() const {
    Node* cur = _root;
    while (cur && cur->left) cur = cur->left;
    return ConstIterator(cur);
}

BinarySearchTree::ConstIterator BinarySearchTree::cend() const {
    Node* cur = _root;
    while (cur && cur->right) cur = cur->right;
    return ConstIterator(cur);
}

size_t BinarySearchTree::size() const {
    return _size;
}

void BinarySearchTree::output_tree() {
    if (_root) _root->output_node_tree();
}
