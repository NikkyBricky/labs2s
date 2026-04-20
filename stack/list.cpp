#include <stdexcept>
#include "list.h"

List::List(const List& other){
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


List& List::operator=(const List& other){
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


List::List(List&& other) noexcept{
    head = other.head;
    tail = other.tail;
    other.head = nullptr;
    other.tail = nullptr;
}


List& List::operator=(List&& other) noexcept{
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


List::~List(){
    clear();
}


void List::clear(){
    Node* ptr = head;
    while (ptr != nullptr){
	Node* next_ptr = ptr->next;
	delete ptr;
	ptr = next_ptr;
    }
    tail = nullptr;
    _size = 0;
}


void List::push_back(const ValueType& value){
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


void List::pop_back() {
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


const ValueType& List::back() const {
    if (tail){
	return tail->value;
    }

    else{
        throw std::runtime_error("List is empty");
    }
}


bool List::empty() const {
    return _size == 0;
}


size_t List::size() const {
    return _size;   
}

