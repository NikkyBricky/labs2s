#pragma once
#include <cstddef>
#include <vector>

using ValueType = double;

class IStackImplementation {
public:
    virtual void push(const ValueType& value) = 0;
    virtual void pop() = 0;
    virtual const ValueType& top() const = 0;
    virtual bool isEmpty() const = 0;
    virtual size_t size() const = 0;
    virtual IStackImplementation* clone() const = 0;
    virtual ~IStackImplementation() = default;
};


class StackVector : public IStackImplementation {
    private:
    	std::vector<ValueType> data;
    public:
        void push(const ValueType& value) override;
        void pop() override;
        const ValueType& top() const override;
        bool isEmpty() const override;
        size_t size() const override;   
        IStackImplementation* clone() const override;
};


class StackList : public IStackImplementation {
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
        StackList() = default;
	StackList(const StackList& other);
        StackList& operator=(const StackList& other);
	StackList(StackList&& other) noexcept;
	StackList& operator=(StackList&& other) noexcept;
	~StackList();
        void clear();

	void push(const ValueType& value) override;
	void pop() override;
	const ValueType& top() const override;
	bool isEmpty() const override;
	size_t size() const override;   
	IStackImplementation* clone() const override;
};
