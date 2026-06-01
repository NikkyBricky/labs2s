#include "splay_tree_api.h"
#include <iostream>
#include <string>

int main() {
    SplayTree<uint32_t, double> tree;

    tree.insert(228, 6.7);
    tree.insert(1337, 5.2);
    tree.insert(42, 3.14);

    if (auto v = tree.search(228)) {
        std::cout << "splay[228]: " << *v << std::endl;
    } else {
        std::cout << "not found" << std::endl;
    }

    std::cout << "Successfully removed 1337: " << tree.remove(1337) << std::endl;
    std::cout << "Size after remove: " << tree.size() << " IsValid: " << tree.isValidBST() << std::endl;

    SplayTree<std::string, double> tree2;

    tree2.insert("нике", 4.2);
    tree2.insert("рита", 9.9);
    tree2.insert("abibas", 67.77);

    if (auto v = tree2.search("abibas")) {
        std::cout << "splay[abibas]: " << *v << std::endl;
    } else {
        std::cout << "not found" << std::endl;
    }

    std::cout << "Successfully removed рита: " << tree2.remove("рита") << std::endl;
    std::cout << "Size after remove: " << tree2.size() << " IsValid: " << tree2.isValidBST() << std::endl;
}
