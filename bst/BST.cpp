#include "BST.h"

#include <iostream>
#include <algorithm>
#include <stack>
#include <functional>

namespace
{
    constexpr bool RED = true;
    constexpr bool BLACK = false;
}

BinarySearchTree::Node::Node(
    Key key,
    Value value,
    Node *parent,
    Node *left,
    Node *right)
    : keyValuePair(key, value),
      parent(parent),
      left(left),
      right(right),
      color(RED)
{
}

BinarySearchTree::Node::Node(const Node &other)
    : keyValuePair(other.keyValuePair),
      parent(nullptr),
      left(nullptr),
      right(nullptr),
      color(other.color)
{
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

bool BinarySearchTree::Node::operator==(const Node &other) const
{
    if (keyValuePair != other.keyValuePair)
        return false;

    if (color != other.color)
        return false;

    bool l =
        (!left && !other.left) ||
        (left && other.left && *left == *other.left);

    bool r =
        (!right && !other.right) ||
        (right && other.right && *right == *other.right);

    return l && r;
}

void BinarySearchTree::Node::output_node_tree() const
{
    if (left)
        left->output_node_tree();

    std::cout << keyValuePair.first << " " << keyValuePair.second << "\n";

    if (right)
        right->output_node_tree();
}

void BinarySearchTree::Node::insert(const Key &, const Value &)
{
}

void BinarySearchTree::Node::erase(const Key &)
{
}

BinarySearchTree::BinarySearchTree(const BinarySearchTree &other)
{
    _size = other._size;
    if (other._root)
        _root = new Node(*other._root);
}

BinarySearchTree &BinarySearchTree::operator=(const BinarySearchTree &other)
{
    if (this == &other)
        return *this;

    this->~BinarySearchTree();

    _size = other._size;

    if (other._root)
        _root = new Node(*other._root);
    else
        _root = nullptr;

    return *this;
}

BinarySearchTree::BinarySearchTree(BinarySearchTree &&other) noexcept
{
    _root = other._root;
    _size = other._size;
    other._root = nullptr;
    other._size = 0;
}

BinarySearchTree &BinarySearchTree::operator=(BinarySearchTree &&other) noexcept
{
    if (this == &other)
        return *this;

    this->~BinarySearchTree();

    _root = other._root;
    _size = other._size;
    other._root = nullptr;
    other._size = 0;

    return *this;
}

BinarySearchTree::~BinarySearchTree()
{
    if (!_root)
        return;

    std::stack<Node *> st;
    st.push(_root);

    while (!st.empty())
    {
        Node *cur = st.top();
        st.pop();

        if (cur->left)
            st.push(cur->left);
        if (cur->right)
            st.push(cur->right);

        delete cur;
    }

    _root = nullptr;
    _size = 0;
}

BinarySearchTree::Iterator::Iterator(Node *node)
    : _node(node)
{
}

std::pair<Key, Value> &BinarySearchTree::Iterator::operator*()
{
    return _node->keyValuePair;
}

const std::pair<Key, Value> &BinarySearchTree::Iterator::operator*() const
{
    return _node->keyValuePair;
}

std::pair<Key, Value> *BinarySearchTree::Iterator::operator->()
{
    return &_node->keyValuePair;
}

const std::pair<Key, Value> *BinarySearchTree::Iterator::operator->() const
{
    return &_node->keyValuePair;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++()
{
    if (!_node)
        return *this;

    if (_node->right)
    {
        _node = _node->right;
        while (_node->left)
            _node = _node->left;
    }
    else
    {
        Node *p = _node->parent;
        while (p && _node == p->right)
        {
            _node = p;
            p = p->parent;
        }
        _node = p;
    }

    return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++(int)
{
    Iterator tmp(*this);
    ++(*this);
    return tmp;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--()
{
    if (!_node)
        return *this;

    if (_node->left)
    {
        _node = _node->left;
        while (_node->right)
            _node = _node->right;
    }
    else
    {
        Node *p = _node->parent;
        while (p && _node == p->left)
        {
            _node = p;
            p = p->parent;
        }
        _node = p;
    }

    return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--(int)
{
    Iterator tmp(*this);
    --(*this);
    return tmp;
}

bool BinarySearchTree::Iterator::operator==(const Iterator &other) const
{
    return _node == other._node;
}

bool BinarySearchTree::Iterator::operator!=(const Iterator &other) const
{
    return _node != other._node;
}

BinarySearchTree::ConstIterator::ConstIterator(const Node *node)
    : _node(node)
{
}

const std::pair<Key, Value> &BinarySearchTree::ConstIterator::operator*() const
{
    return _node->keyValuePair;
}

const std::pair<Key, Value> *BinarySearchTree::ConstIterator::operator->() const
{
    return &_node->keyValuePair;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++()
{
    if (!_node)
        return *this;

    if (_node->right)
    {
        _node = _node->right;
        while (_node->left)
            _node = _node->left;
    }
    else
    {
        const Node *p = _node->parent;
        while (p && _node == p->right)
        {
            _node = p;
            p = p->parent;
        }
        _node = p;
    }

    return *this;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++(int)
{
    ConstIterator tmp(*this);
    ++(*this);
    return tmp;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--()
{
    if (!_node)
        return *this;

    if (_node->left)
    {
        _node = _node->left;
        while (_node->right)
            _node = _node->right;
    }
    else
    {
        const Node *p = _node->parent;

        while (p && _node == p->left)
        {
            _node = p;
            p = p->parent;
        }

        _node = p;
    }

    return *this;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--(int)
{
    ConstIterator tmp(*this);
    --(*this);
    return tmp;
}

bool BinarySearchTree::ConstIterator::operator==(const ConstIterator &other) const
{
    return _node == other._node;
}

bool BinarySearchTree::ConstIterator::operator!=(const ConstIterator &other) const
{
    return _node != other._node;
}

void BinarySearchTree::insert(const Key &key, const Value &value)
{
    auto colorOf = [](Node *n) { return n ? n->color : BLACK; };

    auto leftRotate = [&](Node *x)
    {
        Node *y = x->right;
        x->right = y->left;
        if (y->left)
            y->left->parent = x;

        y->parent = x->parent;

        if (!x->parent)
            _root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;

        y->left = x;
        x->parent = y;
    };

    auto rightRotate = [&](Node *y)
    {
        Node *x = y->left;
        y->left = x->right;
        if (x->right)
            x->right->parent = y;

        x->parent = y->parent;

        if (!y->parent)
            _root = x;
        else if (y == y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;

        x->right = y;
        y->parent = x;
    };

    Node *z = new Node(key, value);
    Node *y = nullptr;
    Node *x = _root;

    while (x)
    {
        y = x;
        if (key < x->keyValuePair.first)
            x = x->left;
        else
            x = x->right;
    }

    z->parent = y;

    if (!y)
        _root = z;
    else if (key < y->keyValuePair.first)
        y->left = z;
    else
        y->right = z;

    while (z != _root && colorOf(z->parent) == RED)
    {
        if (z->parent == z->parent->parent->left)
        {
            Node *u = z->parent->parent->right;

            if (colorOf(u) == RED)
            {
                z->parent->color = BLACK;
                u->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->right)
                {
                    z = z->parent;
                    leftRotate(z);
                }

                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(z->parent->parent);
            }
        }
        else
        {
            Node *u = z->parent->parent->left;

            if (colorOf(u) == RED)
            {
                z->parent->color = BLACK;
                u->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->left)
                {
                    z = z->parent;
                    rightRotate(z);
                }

                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(z->parent->parent);
            }
        }
    }

    _root->color = BLACK;
    ++_size;
}

void BinarySearchTree::erase(const Key &key)
{
    while (true)
    {
        Node *z = _root;

        while (z)
        {
            if (key < z->keyValuePair.first)
                z = z->left;
            else if (key > z->keyValuePair.first)
                z = z->right;
            else
                break;
        }

        if (!z)
            break;

        auto minimum = [](Node *n)
        {
            while (n->left)
                n = n->left;
            return n;
        };

        auto transplant = [&](Node *u, Node *v)
        {
            if (!u->parent)
                _root = v;
            else if (u == u->parent->left)
                u->parent->left = v;
            else
                u->parent->right = v;

            if (v)
                v->parent = u->parent;
        };

        Node *y = z;

        if (!z->left)
        {
            transplant(z, z->right);
        }
        else if (!z->right)
        {
            transplant(z, z->left);
        }
        else
        {
            y = minimum(z->right);

            if (y->parent != z)
            {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        delete z;
        --_size;
    }

    if (_root)
        _root->color = BLACK;
}

BinarySearchTree::Iterator BinarySearchTree::find(const Key &key)
{
    Node *cur = _root;
    Node *res = nullptr;

    while (cur)
    {
        if (key < cur->keyValuePair.first)
            cur = cur->left;
        else if (key > cur->keyValuePair.first)
            cur = cur->right;
        else
        {
            res = cur;
            cur = cur->left;
        }
    }

    return res ? Iterator(res) : end();
}

BinarySearchTree::ConstIterator BinarySearchTree::find(const Key &key) const
{
    const Node *cur = _root;
    const Node *res = nullptr;

    while (cur)
    {
        if (key < cur->keyValuePair.first)
            cur = cur->left;
        else if (key > cur->keyValuePair.first)
            cur = cur->right;
        else
        {
            res = cur;
            cur = cur->left;
        }
    }

    return res ? ConstIterator(res) : cend();
}

std::pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator>
BinarySearchTree::equalRange(const Key &key)
{
    Iterator first = find(key);
    if (first == end())
        return {end(), end()};

    Iterator second = first;
    while (second != end() && second->first == key)
        ++second;

    return {first, second};
}

std::pair<BinarySearchTree::ConstIterator, BinarySearchTree::ConstIterator>
BinarySearchTree::equalRange(const Key &key) const
{
    ConstIterator first = find(key);
    if (first == cend())
        return {cend(), cend()};

    ConstIterator second = first;
    while (second != cend() && second->first == key)
        ++second;

    return {first, second};
}

BinarySearchTree::ConstIterator BinarySearchTree::min() const
{
    const Node *cur = _root;
    if (!cur)
        return cend();

    while (cur->left)
        cur = cur->left;

    return ConstIterator(cur);
}

BinarySearchTree::ConstIterator BinarySearchTree::max() const
{
    const Node *cur = _root;
    if (!cur)
        return cend();

    while (cur->right)
        cur = cur->right;

    return ConstIterator(cur);
}

BinarySearchTree::ConstIterator BinarySearchTree::min(const Key &key) const
{
    return equalRange(key).first;
}

BinarySearchTree::ConstIterator BinarySearchTree::max(const Key &key) const
{
    auto r = equalRange(key);
    if (r.first == r.second)
        return cend();
    ConstIterator it = r.second;
    --it;
    return it;
}

BinarySearchTree::Iterator BinarySearchTree::begin()
{
    Node *cur = _root;
    if (!cur)
        return end();

    while (cur->left)
        cur = cur->left;

    return Iterator(cur);
}

BinarySearchTree::Iterator BinarySearchTree::end()
{
    return Iterator(nullptr);
}

BinarySearchTree::ConstIterator BinarySearchTree::cbegin() const
{
    const Node *cur = _root;
    if (!cur)
        return cend();

    while (cur->left)
        cur = cur->left;

    return ConstIterator(cur);
}

BinarySearchTree::ConstIterator BinarySearchTree::cend() const
{
    return ConstIterator(nullptr);
}

size_t BinarySearchTree::size() const
{
    return _size;
}

void BinarySearchTree::output_tree()
{
    if (_root)
        _root->output_node_tree();
}

size_t BinarySearchTree::max_height() const
{
    std::function<size_t(Node *)> dfs = [&](Node *n)
    {
        if (!n)
            return 0ULL;
        return 1ULL + std::max(dfs(n->left), dfs(n->right));
    };

    return dfs(_root);
}
