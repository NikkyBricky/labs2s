#include "BST.h"
#include <iostream>
#include <limits>
#include <stdexcept>

/*BinarySearchTree::Node::Node(Key k, Value v, Node* p, Node* l, Node* r)
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
        Node* s = right;
        while (s->left) s = s->left;
        keyValuePair = s->keyValuePair;
        s->erase(s->keyValuePair.first);
        return;
    }

    Node* child = left ? left : right;

    if (parent) {
        if (parent->left == this) parent->left = child;
        else parent->right = child;
    }

    if (child) child->parent = parent;

    delete this;
}

void BinarySearchTree::freeSubtree(Node* node) {
    if (!node) return;
    freeSubtree(node->left);
    freeSubtree(node->right);
    delete node;
}

BinarySearchTree::BinarySearchTree(const BinarySearchTree& other)
    : _size(other._size)
{
    if (other._root) _root = new Node(*other._root);
}

BinarySearchTree& BinarySearchTree::operator=(const BinarySearchTree& other) {
    if (this == &other) return *this;

    freeSubtree(_root);
    _root = nullptr;
    _size = other._size;

    if (other._root) _root = new Node(*other._root);

    return *this;
}

BinarySearchTree::BinarySearchTree(BinarySearchTree&& other) noexcept {
    _root = other._root;
    _size = other._size;
    other._root = nullptr;
    other._size = 0;
}

BinarySearchTree& BinarySearchTree::operator=(BinarySearchTree&& other) noexcept {
    if (this == &other) return *this;

    freeSubtree(_root);
    _root = other._root;
    _size = other._size;
    other._root = nullptr;
    other._size = 0;

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
    Iterator t = *this;
    ++(*this);
    return t;
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
    Iterator t = *this;
    --(*this);
    return t;
}

bool BinarySearchTree::Iterator::operator==(const Iterator& o) const {
    return _node == o._node;
}

bool BinarySearchTree::Iterator::operator!=(const Iterator& o) const {
    return _node != o._node;
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
    ConstIterator t = *this;
    ++(*this);
    return t;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--() {
    return ConstIterator(
        const_cast<Node*>(
            (--Iterator(const_cast<Node*>(_node)))._node
        )
    );
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--(int) {
    ConstIterator t = *this;
    --(*this);
    return t;
}

bool BinarySearchTree::ConstIterator::operator==(const ConstIterator& o) const {
    return _node == o._node;
}

bool BinarySearchTree::ConstIterator::operator!=(const ConstIterator& o) const {
    return _node != o._node;
}

void BinarySearchTree::insert(const Key& key, const Value& value) {
    if (!_root) {
        _root = new Node(key, value);
        _root->right = new Node(std::numeric_limits<Key>::max(), value, _root);
    } else {
        _root->insert(key, value);
    }
    ++_size;
}

void BinarySearchTree::erase(const Key& key) {
    if (!_root) return;

    auto it = find(key);
    while (it != end()) {
        it._node->erase(key);
        --_size;
        it = find(key);
    }
}

BinarySearchTree::ConstIterator BinarySearchTree::find(const Key& key) const {
    Node* cur = _root;

    while (cur) {
        if (key < cur->keyValuePair.first) cur = cur->left;
        else if (key > cur->keyValuePair.first) cur = cur->right;
        else return ConstIterator(cur);
    }

    return cend();
}

BinarySearchTree::Iterator BinarySearchTree::find(const Key& key) {
    Node* cur = _root;

    while (cur) {
        if (key < cur->keyValuePair.first) cur = cur->left;
        else if (key > cur->keyValuePair.first) cur = cur->right;
        else return Iterator(cur);
    }

    return end();
}

std::pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator>
BinarySearchTree::equalRange(const Key& key) {
    Iterator a = find(key);
    if (a == end()) return {end(), end()};

    Iterator b = a;
    while (b != end() && (*b).first == key) ++b;

    return {a, b};
}

std::pair<BinarySearchTree::ConstIterator, BinarySearchTree::ConstIterator>
BinarySearchTree::equalRange(const Key& key) const {
    ConstIterator a = find(key);
    if (a == cend()) return {cend(), cend()};

    ConstIterator b = a;
    while (b != cend() && (*b).first == key) ++b;

    return {a, b};
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

BinarySearchTree::ConstIterator BinarySearchTree::min(const Key& key) const {
    auto r = equalRange(key);
    if (r.first == cend()) return cend();

    ConstIterator best = r.first;
    for (auto it = r.first; it != r.second; ++it)
        if ((*it).second < (*best).second)
            best = it;

    return best;
}

BinarySearchTree::ConstIterator BinarySearchTree::max(const Key& key) const {
    auto r = equalRange(key);
    if (r.first == cend()) return cend();

    ConstIterator best = r.first;
    for (auto it = r.first; it != r.second; ++it)
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
}*/
#include "BST.h"
#include <limits>
#include <iostream>

BinarySearchTree::Node::Node(Key key, Value value, Node *parent, Node *left, Node *right) : keyValuePair(key, value), parent(parent), left(left), right(right) {}

BinarySearchTree::Node::Node(const Node &other) : keyValuePair(other.keyValuePair), parent(nullptr), left(nullptr), right(nullptr) {
    if (other.left)
    {
        left = new Node(*other.left);
        left->parent = this;
    }

    if (other.right)
    {
        right = new Node(*other.right);
        right->parent = this;
    }
}

bool BinarySearchTree::Node::operator==(const Node &other) const {
	return keyValuePair == other.keyValuePair;
}

void BinarySearchTree::Node::output_node_tree() const {
	if (left!=nullptr)
	{
		left->output_node_tree();
	}
	if (this->keyValuePair.first == std::numeric_limits<Key>::max()) return;
	std::cout << "Ключ: " << keyValuePair.first << " значение: " << keyValuePair.second << std::endl;
	if (right!=nullptr)
	{
		right->output_node_tree();
	}
}

void BinarySearchTree::Node::insert(const Key &key, const Value &value) {
	if (key < keyValuePair.first)
	{
		if (left!=nullptr) left->insert(key, value);
		else left = new Node(key, value, this);
	}
	else
	{
		if (right!=nullptr) right->insert(key, value);
		else right = new Node(key, value, this);
	}
}

void BinarySearchTree::Node::erase(const Key &key) {
	if (key != keyValuePair.first) {
        	if (left!=nullptr && key < keyValuePair.first) left->erase(key);
        	else if (right!=nullptr) right->erase(key);
        	return;
	}
	if (left!=nullptr && right!=nullptr) {
		Node* current = right;
		while (current->left) current = current->left;
		this->keyValuePair = current->keyValuePair;
		current->erase(current->keyValuePair.first);
		return;
	}
	Node* ptr = nullptr;
	if (left!=nullptr) ptr = left;
	else ptr = right;
	if (parent!=nullptr) {
		if (parent->left == this) parent->left = ptr;
		else parent->right = ptr;
	}
	if (ptr!=nullptr) ptr->parent = parent;
	delete this;
	return;
}

void BinarySearchTree::freeSubtree(Node* node) {
	if (!node) return;
	freeSubtree(node->left);
	freeSubtree(node->right);
	delete node;
}

BinarySearchTree::BinarySearchTree(const BinarySearchTree &other) : _size(other._size) {
	if (other._root!=nullptr) _root = new Node(*other._root);
}

BinarySearchTree& BinarySearchTree::operator=(const BinarySearchTree &other) {
	if (this == &other) return *this;
	freeSubtree(_root);
	_root = nullptr;
	_size = 0;
	if (other._root!=nullptr) _root = new Node(*other._root);
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

BinarySearchTree::Iterator::Iterator(Node *node) : _node(node) {};

std::pair<Key, Value>& BinarySearchTree::Iterator::operator*() {
	if (_node==nullptr) throw std::runtime_error("Null pointer");
	return _node->keyValuePair;
}

const std::pair<Key, Value>& BinarySearchTree::Iterator::operator*() const {
	if (_node==nullptr) throw std::runtime_error("Null pointer");
	return _node->keyValuePair;
}

std::pair<Key, Value>* BinarySearchTree::Iterator::operator->() {
	if (_node==nullptr) throw std::runtime_error("Null pointer");
	return &_node->keyValuePair;
}

const std::pair<Key, Value>* BinarySearchTree::Iterator::operator->() const {
	if (_node==nullptr) throw std::runtime_error("Null pointer");
	return &_node->keyValuePair;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++() {
	if (_node==nullptr) throw std::runtime_error("Null pointer");
	if (_node->right!=nullptr)
	{
		_node = _node->right;
		while (_node->left!=nullptr)
		{
			_node = _node->left;
		}
		return *this;
	}
	while (_node->parent!=nullptr)
	{
		if (_node == _node->parent->left)
		{
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
	if (_node==nullptr) throw std::runtime_error("Null pointer");
          if (_node->left!=nullptr)
          {
                  _node = _node->left;
                  while (_node->right!=nullptr)
                  {
                          _node = _node->right;
                  }
                  return *this;
          }
          while (_node->parent!=nullptr)
          {
                  if (_node == _node->parent->right)
                  {
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
	if (_node==nullptr) throw std::runtime_error("Null pointer");
	return _node->keyValuePair;
}

const std::pair<Key, Value>* BinarySearchTree::ConstIterator::operator->() const {
	if (_node==nullptr) throw std::runtime_error("Null pointer");
	return &_node->keyValuePair;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++() {
	if (_node==nullptr) throw std::runtime_error("Null pointer");
          if (_node->right!=nullptr)
          {
                  _node = _node->right;
                  while (_node->left!=nullptr)
                  {
                          _node = _node->left;
                  }
                  return *this;
          }
          while (_node->parent!=nullptr)
          {
                  if (_node == _node->parent->left)
                  {
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
	if (_node==nullptr) throw std::runtime_error("Null pointer");
          if (_node->left!=nullptr)
          {
                  _node = _node->left;
                  while (_node->right!=nullptr)
                  {
                          _node = _node->right;
                  }
                  return *this;
          }
          while (_node->parent!=nullptr)
          {
                  if (_node == _node->parent->right)
                  {
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
	if (_root==nullptr) {
		_root = new Node(key, value);
		_root->right = new Node(std::numeric_limits<Key>::max(),value,_root);
	}
	else _root->insert(key, value);
	_size++;
}

void BinarySearchTree::erase(const Key &key) {
	if (_root==nullptr) return;
	Iterator tmp(_root);
	while ((tmp = find(key)) != end())
	{
		tmp._node->erase(key);
		_size--;
	}
}

BinarySearchTree::ConstIterator BinarySearchTree::find(const Key &key) const {
	Node *tmp = _root;
	while (tmp!=nullptr)
	{
		if (key < tmp->keyValuePair.first) tmp = tmp->left;
		else if (key > tmp->keyValuePair.first) tmp = tmp->right;
		else return ConstIterator(tmp);
	}
	return cend();
}

BinarySearchTree::Iterator BinarySearchTree::find(const Key &key) {
	Node *tmp = _root;
	while (tmp!=nullptr)
	{
		if (key < tmp->keyValuePair.first) tmp = tmp->left;
		else if (key > tmp->keyValuePair.first) tmp = tmp->right;
		else return Iterator(tmp);
	}
	return end();
}

std::pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator> BinarySearchTree::equalRange(const Key &key) {
	Iterator first = find(key);
	if (first == end()) return {end(), end()};
	Iterator second = first;
	while (second != end() && (*second).first == key) ++second;
	return {first, second};
}

std::pair<BinarySearchTree::ConstIterator,BinarySearchTree::ConstIterator> BinarySearchTree::equalRange(const Key &key) const {
	ConstIterator first = find(key);
	if (first == cend()) return {cend(), cend()};
	ConstIterator second = first;
	while (second != cend() && (*second).first == key) ++second;
	return {first, second};
}

BinarySearchTree::ConstIterator BinarySearchTree::min() const {
	Node *tmp = _root;
	if (tmp==nullptr) return cend();
	while (tmp->left) tmp = tmp->left;
	return ConstIterator(tmp);
}

BinarySearchTree::ConstIterator BinarySearchTree::max() const {
	if (_root==nullptr) return cend();
	return --cend();
}

BinarySearchTree::ConstIterator BinarySearchTree::min(const Key &key) const {
	std::pair<ConstIterator,ConstIterator> tmp = equalRange(key);
	if (tmp.first==cend()) return cend();
	ConstIterator mini = tmp.first;
	for (auto i = tmp.first;i!=tmp.second;i++)
	{
		if ((*mini).second > (*i).second) mini=i;
	}
	return mini;
}

BinarySearchTree::ConstIterator BinarySearchTree::max(const Key &key) const {
	std::pair<ConstIterator,ConstIterator> tmp = equalRange(key);
        if (tmp.first==cend()) return cend();
        ConstIterator maxi = tmp.first;
        for (auto i = tmp.first;i!=tmp.second;i++)
        {
                if ((*maxi).second < (*i).second) maxi=i;
        }
        return maxi;
}

BinarySearchTree::Iterator BinarySearchTree::begin() {
	Node *tmp = _root;
	while (tmp->left!=nullptr) tmp = tmp->left;
	return Iterator(tmp);
}

BinarySearchTree::Iterator BinarySearchTree::end() {
	Node *tmp = _root;
        while (tmp->right!=nullptr) tmp = tmp->right;
        return Iterator(tmp);
}

BinarySearchTree::ConstIterator BinarySearchTree::cbegin() const {
	Node *tmp = _root;
	while (tmp->left) tmp = tmp->left;
	return ConstIterator(tmp);
}

BinarySearchTree::ConstIterator BinarySearchTree::cend() const {
	Node *tmp = _root;
        while (tmp->right!=nullptr) tmp = tmp->right;
        return ConstIterator(tmp);
}

size_t BinarySearchTree::size() const {
	return _size;
}

void BinarySearchTree::output_tree() {
	if (_root) _root->output_node_tree();
}
