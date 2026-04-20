#pragma once
#include <cstddef>

using ValueType = double;

class List{
    private:
        struct Node{
	    double value;
	    Node* prev;
	    Node* next;
	};
	Node* head = nullptr;
	Node* tail = nullptr;
	size_t _size = 0;
    public:
        List() = default;
	List(const List& other);
        List& operator=(const List& other);
	List(List&& other) noexcept;
	List& operator=(List&& other) noexcept;
	~List();

        void clear();
	void push_back(const ValueType& value);
	void pop_back();
	const ValueType& back() const;
	bool empty() const;
	size_t size() const;

};
