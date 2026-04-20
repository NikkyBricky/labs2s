#pragma once
#include <cstddef>
#include "vector.h"
#include "list.h"

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
    	Vector data;
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
        List lst;
    public:
	void push(const ValueType& value) override;
	void pop() override;
	const ValueType& top() const override;
	bool isEmpty() const override;
	size_t size() const override;   
	IStackImplementation* clone() const override;
};
