#include "StackImplementation.h"


void StackVector::push(const ValueType& value){
    data.push_back(value);
}


void StackVector::pop(){
    data.pop_back();
}


const ValueType& StackVector::top() const {
    return data.back();
}


bool StackVector::isEmpty() const {
    return data.empty();
}


size_t StackVector::size() const {
    return data.size();
}


IStackImplementation* StackVector::clone() const {
    return new StackVector(*this);
}


void StackList::push(const ValueType& value){
    lst.push_back(value);
}


void StackList::pop() {
    lst.pop_back();
}


const ValueType& StackList::top() const {
    lst.back();    
}


bool StackList::isEmpty() const {
    return lst.empty();
}


size_t StackList::size() const {
    return lst.size();   
}


IStackImplementation* StackList::clone() const {
    return new StackList(*this);
}

