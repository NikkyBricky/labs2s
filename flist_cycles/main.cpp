#include <iostream>
#include <utility>
#include "flist.h"


int main(){
    ForwardList lst = ForwardList();
    lst.push_back(1);
    lst.push_back(2);
    lst.push_back(3);
    lst.push_back(4);

    lst.make_cycle(2);
    std::pair<ForwardList::Node*, size_t> cycle_pair = lst.find_cycle();
    std::cout << cycle_pair.second << std::endl;
}
