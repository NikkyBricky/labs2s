#include <iostream>
#include <utility>
#include <stdexcept>
#include "flist.h"


ForwardList::ForwardList() {}


ForwardList::ForwardList(const ForwardList& other){
    Node* ptr1 = other.head;

    if (ptr1) {
    Node* new_node = new Node{ptr1->num, head};
    head = new_node;
    }

    Node* ptr2 = head;

    while (ptr1->next) {
	ptr1 = ptr1->next;
        Node* new_node = new Node{ptr1->num, nullptr};
        ptr2->next = new_node;
	ptr2 = ptr2->next;
    }
    _size = other._size;
}


ForwardList& ForwardList::operator=(const ForwardList& other){
    if (this == &other){
        return *this;
    }

    clear();
    Node* ptr1 = other.head;
    if (ptr1) {
    Node* new_node = new Node{ptr1->num, head};
    head = new_node;
    }
    Node* ptr2 = head;

    while (ptr1->next) {
	ptr1 = ptr1->next;
        Node* new_node = new Node{ptr1->num, nullptr};
        ptr2->next = new_node;
	ptr2 = ptr2->next;
    }
    _size = other._size;
    return *this;
}


ForwardList::ForwardList(ForwardList&& other) noexcept{
    head = other.head;
    _size = other._size;
    other.head = nullptr;
    other._size = 0;
}


ForwardList& ForwardList::operator=(ForwardList&& other) noexcept{
    if (this == &other){
        return *this;
    }
    clear();
    head = other.head;
    _size = other._size;
    other.head = nullptr;
    other._size = 0;
    return *this;
}


ForwardList::~ForwardList(){
    clear();
}


void ForwardList::push_front(double n) {
    Node* new_node = new Node{n, head};
    head = new_node;
    ++_size;
}

void ForwardList::push_back(double n){
    Node* ptr = head;
    while (ptr && ptr->next){
        ptr = ptr->next;
    }
    Node* new_node = new Node{n, nullptr};
    if (ptr){
        ptr->next = new_node; 
    }
    else{
        head = new_node;
    }
    ++_size;
}


void ForwardList::pop_front() {
    if (head != nullptr){
        Node* sec_ptr = head->next;
	delete head;
	head = sec_ptr;
	--_size;
    }
}


void ForwardList::pop_back(){
    if (_size == 1){
        pop_front();
	--_size;
    }
    else if (_size > 1){
        size_t k = 0;
        Node* ptr = head;
        while (k < _size - 2){
            ptr = ptr->next;
	    ++k;
        }
	delete ptr->next;
	ptr->next = nullptr;
	--_size;
    }
}


const double& ForwardList::front() const{
    if (head != nullptr){
	const double& n = head->num;
        return n;
    }
    else{
        throw std::runtime_error("List is empty");
    }
}


double& ForwardList::front(){
    if (head != nullptr){
	double& n = head->num;
        return n;
    }
    else{
        throw std::runtime_error("List is empty");
    }
}


double* ForwardList::find(double n){
    Node* ptr = head;
    while (ptr){
        if (ptr->num == n){
	    return &(ptr->num);
	}
	ptr = ptr->next;
    }
    return nullptr;
}


const double* ForwardList::find(double n) const{
    Node* ptr = head;
    while (ptr){
        if (ptr->num == n){
	    return &(ptr->num);
	}
	ptr = ptr->next;
    }
    return nullptr;
}


size_t ForwardList::size() const {
   return _size;
}


bool ForwardList::empty() const {
    return (_size == 0) ? true : false;
}


void ForwardList::display() const {
    Node* ptr = head;
    for (size_t i = 0; i < _size ; ++i){
	std::cout << ptr->num;
	if (ptr->next != nullptr){
	    std::cout << " -> ";
	}
	ptr = ptr->next;
    }
    std::cout << std::endl;
}



void ForwardList::erase(double n){
    Node* ptr = head;
    Node* prev_ptr = head;

    while (ptr != nullptr){
        if (ptr->num == n){
	    Node* next_ptr = ptr->next;
	    if (ptr == head){
		delete head;
	        head = next_ptr;
		ptr = head;
		prev_ptr = head;
	    }
	    else{
	        prev_ptr->next = next_ptr;
		delete ptr;
		ptr = next_ptr;
	    }
	    --_size;
	}
	else{
	    prev_ptr = ptr;
	    ptr = ptr->next;
	}
    }
}


void ForwardList::clear(){
    Node* ptr = head;
    for(size_t i = 0; i < _size; ++i){
	Node* next_ptr = ptr->next;
	delete ptr;
	ptr = next_ptr;
    }
}


void ForwardList::make_cycle(size_t ind){
    if (!head){
	    throw std::runtime_error("list is empty");
    }
    
    if (ind >= _size || ind < 0){
        throw std::out_of_range("index out of range");
    }

    Node* ptr = head;
    Node* beg_cyc_ptr = nullptr;
    size_t k = 0;
    while (ptr->next){
	if (k == ind){
	    beg_cyc_ptr = ptr;
	}
        ptr = ptr->next;
	++k;
    }
    ptr->next = beg_cyc_ptr;
}


std::pair<ForwardList::Node*, size_t> ForwardList::find_cycle() const{
   if (!head){
	return std::pair<ForwardList::Node*, size_t>(nullptr, 0);
    }

    ForwardList::Node* slow = head;
    ForwardList::Node* fast = head;

    while(fast != nullptr && fast->next != nullptr){
        slow = slow->next;
	fast = fast->next->next;

	if (slow == fast){
	    break;
	}
    }
        
    if (slow != fast){
	return std::pair<ForwardList::Node*, size_t>(nullptr, _size);
    }

    slow = head;
    size_t i = 0;
    while (slow != fast){
	slow = slow->next;
	fast = fast->next;
	++i;
    }

    return std::pair<ForwardList::Node*, size_t>(slow, i);
}


