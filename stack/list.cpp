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


void List::push_front(ValueType n) {
    Node* new_node = new Node{n, nullptr, head};
    if (!head){
	head = new_node;
        tail = head;
    }
    else{
        head->prev = new_node;
	head = new_node;
    }

}


void List::pop_front() {
    if (head){
        if (tail != head){
            Node* sec_ptr = head->next;
	    delete head;
	    head = sec_ptr;
        }
	else{
	    delete head;
	    tail = nullptr;
	}
    }
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


void List::insert(size_t i, ValueType n){
    if (i > size() - 1 || i < 0){
        throw std::runtime_error("index out of range");
    }

    if (i == 0){
        push_front(n);
    }

    else{
        size_t k = 0;
        Node* ptr = head;
        while (k < i){
            ptr = ptr->next;
  	    ++k;
        }
        Node* prev = ptr->prev;
        Node* next = ptr->next;
        Node* new_node = new Node{n, prev, ptr};
        prev->next = new_node;
        }
}


const double& List::front() const{
    if (head != nullptr){
	return head->value;
	}
    else{
        throw std::runtime_error("List is empty");
    }
}

double& List::front(){
    if (head != nullptr){
	return head->value;
    }
    else{
        throw std::runtime_error("List is empty");
    }
}


ValueType& List::back(){
    if (tail){
	return tail->value;
    }
    else{
        throw std::runtime_error("List is empty");
    }
}


void List::display() const {
    Node* ptr = head;
    while (ptr){
	std::cout << ptr->value;
	if (ptr->next){
	    std::cout << " -> ";
	}
	ptr = ptr->next;
    }
    std::cout << std::endl;
}



void List::erase(ValueType n){
    Node* ptr = head;
    Node* prev_ptr = head;

    while (ptr != nullptr){
        if (ptr->value == n){
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
	}
	else{
	    prev_ptr = ptr;
	    ptr = ptr->next;
	}
    }
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


ValueType& List::operator[](unsigned i){
    if (i > size() - 1 || i < 0){
        throw std::runtime_error("index out of range");
    }
    unsigned k = 0;
    Node* ptr = head;
    while (k < i){
        ptr = ptr->next;
	++k;
    }
    ValueType& val = ptr->value;
    return val;
}


std::ostream& operator<<(std::ostream& strm, const List& lst){
   unsigned k = 0;
   List::Node* ptr = lst.head;
   while (k < lst.size()){
       strm << ptr->value;
       if (k != lst.size() - 1){
           strm << " -> ";
       }
       ptr = ptr->next;
       ++k;
   }
   return strm;
}


List operator+(const List& one, const List& other){
    List lst;
    List::Node* ptr1 = one.head;
    List::Node* ptr2 = lst.head;
    if (ptr1){
        List::Node* new_node = new List::Node{ptr1->value, nullptr, lst.head};
	lst.head = new_node;
	lst.tail = new_node;
	ptr2 = lst.head;
	while (ptr1->next){
	    ptr1 = ptr1->next;
            List::Node* new_node = new List::Node{ptr1->value, ptr2, nullptr};
	    ptr2->next = new_node;
	    ptr2 = ptr2->next;
	    lst.tail = ptr2;
	}
    }
    return lst;
}
