#pragma once
#include <cstddef>
#include <iostream>
using ValueType = double;

class List{
    private:
        struct Node{
	    ValueType value;
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
        
	void push_front(double n);
	void pop_front();	
	void push_back(const ValueType& value);
	void pop_back();

	void insert(size_t i, ValueType n);
	const ValueType& front() const;
	double& front();
	const ValueType& back() const;
	double& back();
	size_t size() const;
	bool empty() const;
	void display() const;
	void erase(ValueType n);
	void clear();

	double& operator[](unsigned i);
	friend std::ostream& operator<<(std::ostream& strm, const List& lst);
        friend List operator+(const List& one, const List& other); 
};
