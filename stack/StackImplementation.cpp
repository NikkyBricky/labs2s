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


StackList::StackList(const StackList& other){
    Node* ptr = other.head;

    while (ptr) {
        Node* new_node = new Node{ptr->value, tail, nullptr};

        if (tail){
            tail->next = new_node;
            tail = new_node;
        }
        else{
            head = new_node;
	    tail = head;
        }

	ptr = ptr->next;
    }
    _size = other._size;
}


StackList& StackList::operator=(const StackList& other){
    if (this == &other){
        return *this;
    }

    clear(); 

    Node* ptr = other.head;

    while (ptr) {
        Node* new_node = new Node{ptr->value, tail, nullptr};

        if (tail){
            tail->next = new_node;
            tail = new_node;
        }
        else{
            head = new_node;
	    tail = head;
        }

	ptr = ptr->next;
    }
    _size = other._size;

    return *this;
}


StackList::StackList(StackList&& other) noexcept{
    head = other.head;
    tail = other.tail;
    other.head = nullptr;
    other.tail = nullptr;
}


StackList& StackList::operator=(StackList&& other) noexcept{
    if (this == &other){
        return *this;
    }
    clear();
    head = other.head;
    tail = other.tail;
    other.head = nullptr;
    other.tail = nullptr;
    return *this;
}


StackList::~StackList(){
    clear();
}


void StackList::clear(){
    Node* ptr = head;
    while (ptr != nullptr){
	Node* next_ptr = ptr->next;
	delete ptr;
	ptr = next_ptr;
    }
    tail = nullptr;
    _size = 0;
}


void StackList::push(const ValueType& value){
    Node* new_node = new Node{value, tail, nullptr};
    if (tail){
        tail->next = new_node;
	tail = new_node;
    }
    else{
        head = new_node;
	tail = head;
    }
    ++_size;
}


void StackList::pop() {
    if (tail){
        Node* prev = tail->prev;
        delete tail;
        tail = prev;

        if (prev){
            prev->next = nullptr;
        }

        else{
            head = nullptr;
        }

        --_size;
    }   
}


const ValueType& StackList::top() const {
    if (tail){
	return tail->value;
    }

    else{
        throw std::runtime_error("List is empty");
    }
}


bool StackList::isEmpty() const {
    return _size == 0;
}


size_t StackList::size() const {
    return _size;   
}


IStackImplementation* StackList::clone() const {
    return new StackList(*this);
}

