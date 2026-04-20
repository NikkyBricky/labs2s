#include <stdexcept>
#include "vector.h"


Vector::Vector(const ValueType* rawArray, const size_t size, float coef) : _size(size), _capacity(size), _multiplicativeCoef(coef) {
    _data = new ValueType[_capacity];
    for (size_t i = 0; i < size; ++i){
        _data[i] = rawArray[i];
    }
}


Vector::Vector(const Vector& other) : _size(other._size), _capacity(other._size), _multiplicativeCoef(other._multiplicativeCoef) {
   _data = new ValueType[_capacity];
   for (size_t i = 0; i < _size; ++i){
       _data[i] = (other._data)[i];
   }
}


Vector& Vector::operator=(const Vector& other){
    if (this == &other){
        return *this;
    }

    delete[] _data;
    _size = other._size;
    _capacity = other._capacity;
    _multiplicativeCoef = other._multiplicativeCoef;
    _data = new ValueType[_capacity];
   
   for (size_t i = 0; i < _size; ++i){
       _data[i] = (other._data)[i];
   }
   return *this;

}


Vector::Vector(Vector&& other) noexcept : _data(other._data), _size(other._size), _capacity(other._capacity), _multiplicativeCoef(other._multiplicativeCoef){
    other._data = nullptr;
    other._size = 0;
    other._capacity = 0;
    other._multiplicativeCoef = 0;
}


Vector& Vector::operator=(Vector&& other) noexcept{
    if (this == &other){
        return *this;
    }

    delete[] _data;

    _data = other._data;
    _size = other._size;
    _capacity = other._capacity;
    _multiplicativeCoef = other._multiplicativeCoef;

    other._data = nullptr;
    other._size = 0;
    other._capacity = 0;
    other._multiplicativeCoef = 0;

    return *this;
}


Vector::~Vector(){
    delete[] _data;
}


void Vector::pushBack(const ValueType& value){
   if (_size == _capacity){
       if(_capacity == 0) _capacity = 1;

       reserve(_capacity * _multiplicativeCoef);
   }
   _data[_size] = value;
   ++_size;
}


void Vector::pushFront(const ValueType& value){
   if (_size == _capacity){
       if(_capacity == 0) _capacity = 1;

       reserve(_capacity * _multiplicativeCoef);
   }

   for (size_t i = _size; i > 0; --i){
       _data[i] = _data[i - 1];
   }
   _data[0] = value;
   ++_size;
}


void Vector::insert(const ValueType& value, size_t pos){
    if (_size == _capacity){
         if(_capacity == 0) _capacity = 1;

         reserve(_capacity * _multiplicativeCoef);
    }
    
    for (size_t i = _size; i > pos; --i){
        _data[i] = _data[i - 1];
    }
    _data[pos] = value;
    ++_size;

}


void Vector::insert(const ValueType* values, size_t size, size_t pos){
    if (_size + size > _capacity){
	size_t new_capacity = _capacity;
	if(new_capacity == 0) new_capacity = 1;

        while (_size + size > new_capacity){
	    new_capacity *= _multiplicativeCoef; 
	}
        reserve(new_capacity);
    }
    
    for (size_t i = _size; i > pos; --i){
        _data[i + size - 1] = _data[i - 1];
    }

    for (size_t i = 0; i < size; ++i){
        _data[pos + i] = values[i];
    }
    _size += size;
}


void Vector::insert(const Vector& vector, size_t pos){
    if (_size + vector._size > _capacity){
	size_t new_capacity = _capacity;
	if(new_capacity == 0) new_capacity = 1;

        while (_size + vector._size > new_capacity){
	    new_capacity *= _multiplicativeCoef; 
	}
        reserve(new_capacity);    
    }
    
    for (size_t i = _size; i > pos; --i){
        _data[i + vector._size - 1] = _data[i - 1];
    }

    for (size_t i = 0; i < vector._size; ++i){
        _data[pos + i] = (vector._data)[i];
    }
    _size += vector._size;
}


void Vector::popBack(){
    if(_size){
    --_size;
    }
    else{
        throw std::runtime_error("Vector is empty");
    }
}


void Vector::popFront(){
    if(_size){
	for(size_t i = 0; i + 1 < _size; ++i){
            _data[i] = _data[i + 1];
        }
        --_size;
    }

    else{
        throw std::runtime_error("Vector is empty");
    }
}


void Vector::erase(size_t pos, size_t count){
   if (pos + count > _size){
       count = _size - pos;
   }

   for (size_t i = 0; i < _size - pos - count; ++i){
       _data[pos + i] = _data[pos + count + i];
   }

   if (_size){
       _size -= count;
   }
}


void Vector::eraseBetween(size_t beginPos, size_t endPos){
   if (endPos > _size){
       endPos = _size;
   }

   for (size_t i = 0; i < _size - endPos; ++i){
       _data[beginPos + i] = _data[endPos + i];
   }

   if (_size){
       _size -= (endPos - beginPos);
   }
}


size_t Vector::size() const {return _size;}


size_t Vector::capacity() const {return _capacity;}


double Vector::loadFactor() const {return static_cast<double>(_size) / _capacity;}


ValueType& Vector::operator[](size_t idx){
    return _data[idx];
}


const ValueType& Vector::operator[](size_t idx) const{
   return _data[idx];
}


long long Vector::find(const ValueType& value) const{
   for (size_t i = 0; i < _size; ++i){
       if (_data[i] == value){
           return i;
       }
   }
   return _size;
}

const double& Vector::back() const{
    if (_size){
        return _data[_size - 1];
    }
    else{
        throw std::runtime_error("Vector is empty");
    }
}


bool Vector::empty() const{
   return _size == 0;
}


void Vector::reserve(size_t capacity){
    if (capacity > _capacity){
        ValueType* new_data = new ValueType[capacity];
        for (size_t i = 0; i < _size; ++i){
            new_data[i] = _data[i];
        }
        delete[] _data;
        _data = new_data;
	_capacity = capacity;
    }
}


void Vector::shrinkToFit(){
    if (_size < _capacity){
        ValueType* new_data = new ValueType[_size];
	for (size_t i = 0; i < _size; ++i){
            new_data[i] = _data[i];
        }
        delete[] _data;
        _data = new_data;
	_capacity = _size;
    }
}


Vector::Iterator::Iterator(ValueType* ptr) : _ptr(ptr) {}


ValueType& Vector::Iterator::operator*(){
    return *_ptr;
}


const ValueType& Vector::Iterator::operator*() const{
    return *_ptr;
}


ValueType* Vector::Iterator::operator->(){
    return _ptr;
}


const ValueType* Vector::Iterator::operator->() const{
    return _ptr;
}


Vector::Iterator& Vector::Iterator::operator++(){
    ++_ptr;
    return *this;
}


Vector::Iterator Vector::Iterator::operator++(int){
    Iterator ptr = *this;
    ++_ptr;
    return ptr;
}


bool Vector::Iterator::operator==(const Iterator& other) const{
    return _ptr == other._ptr;
}


bool Vector::Iterator::operator!=(const Iterator& other) const{
    return _ptr != other._ptr;
}


Vector::Iterator Vector::begin(){
    return Iterator(_data);
}


Vector::Iterator Vector::end(){
   return Iterator(_data + _size);
}


