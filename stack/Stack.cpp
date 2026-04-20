#include "stack.h"
#include "StackImplementation.h"


Stack::Stack(StackContainer container){
    _containerType = container;
    switch (container){
	case StackContainer::Vector:
            _pimpl = new StackVector();
	    break;

	case StackContainer::List:
            _pimpl = new StackList();
	    break;
    }
}


Stack::Stack(const ValueType* valueArray, const size_t arraySize, StackContainer container){
    _containerType = container;
    switch (container){
        case StackContainer::Vector:
            _pimpl = new StackVector();
	    break;

	case StackContainer::List:
            _pimpl = new StackList();
	    break;
    }

    for (size_t i = 0; i < arraySize; ++i){
        push(valueArray[i]);
    }
}


Stack::Stack(const Stack& copyStack){
    _pimpl = copyStack._pimpl->clone();
    _containerType = copyStack._containerType;
}


Stack& Stack::operator=(const Stack& copyStack){
    if (this == &copyStack){
        return *this;
    }
    delete _pimpl;

    _pimpl = copyStack._pimpl->clone();
    _containerType = copyStack._containerType;
    
    return *this;
}


Stack::Stack(Stack&& moveStack) noexcept {
    _pimpl = moveStack._pimpl;
    _containerType = moveStack._containerType;
    moveStack._pimpl = nullptr;
}


Stack& Stack::operator=(Stack&& moveStack) noexcept {
    if (this == &moveStack){
        return *this;
    }
    delete _pimpl;

    _pimpl = moveStack._pimpl;
    _containerType = moveStack._containerType;
    moveStack._pimpl = nullptr;

    return *this;
}


Stack::~Stack() {
    delete _pimpl;
}


void Stack::push(const ValueType& value){
    _pimpl->push(value);
}


void Stack::pop(){
    _pimpl->pop();
}


const ValueType& Stack::top() const{
    return _pimpl->top();
}


bool Stack::isEmpty() const{
    return _pimpl->isEmpty();
}


size_t Stack::size() const{
    return _pimpl->size();
}

