#include <iostream>
#include "vector.h"

void show(Vector& v){
    for (auto it = v.begin(); it != v.end(); ++it){
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}


int main(){
    Vector v1;
    v1.pushBack(1);
    v1.pushFront(2);
    show(v1);

    Vector v2(v1);
    show(v2);

    double arr[3] = {1, 2, 3};
    Vector v3(arr, 3);
    show(v3);

    v1 = v3;
    v2 = std::move(v3);
    show(v1);
    show(v2);

    v2.pushBack(5);
    v2.insert(4, 3);
    show(v2);

    Vector v4;
    v4.pushFront(25);
    v4.pushFront(20);
    v4.pushFront(15);
    v4.pushFront(10);
    v2.insert(v4, 3);
    show(v2);

    v2.popFront();
    v2.popBack();
    show(v2);

    std::cout << v2.size() << " " << v2.capacity() << " " << v2.loadFactor() << std::endl;
    
    v2.erase(2, 2);
    show(v2);
    v2.eraseBetween(1, 4);
    show(v2);

    std::cout << v2.find(4) << std::endl;
    std::cout << v2.find(5) << std::endl;
    
    v2.reserve(20);
    std::cout << v2.size() << " " << v2.capacity() << " " << v2.loadFactor() << std::endl;
    v2.shrinkToFit();
    std::cout << v2.size() << " " << v2.capacity() << " " << v2.loadFactor() << std::endl;
    
    return 0;
}
