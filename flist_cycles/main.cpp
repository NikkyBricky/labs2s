#include <iostream>
#include <utility>
#include "flist.h"

void print_pair(std::pair<ForwardList::Node*, size_t> p){
    if (p.first){
        std::cout << "Cycle found: starts at " << p.second << " index" << std::endl;
    }

    else{
        std::cout << "Cycle not found" << std::endl;
    }
}


int main(){
    ForwardList lst1 = ForwardList();
    lst1.push_back(1);
    lst1.push_back(2);
    lst1.push_back(3);
    lst1.push_back(4);

    lst1.make_cycle(2);
    
    ForwardList lst2 = ForwardList();
    lst2.push_back(1);
    lst2.push_back(2);
    lst2.push_back(3);
    lst2.push_back(4);

    std::pair<ForwardList::Node*, size_t> cycle_pair = lst1.find_cycle();
    std::pair<ForwardList::Node*, size_t> non_cycle_pair = lst2.find_cycle();

    print_pair(cycle_pair);
    print_pair(non_cycle_pair);
}
