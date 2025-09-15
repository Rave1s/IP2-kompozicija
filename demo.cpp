#include "RadixTree.h"
#include <iostream>
#include <exception>

using namespace RadixTreeProject;

int main() {
    try {
        RadixTree r;

        r.insert("toast");
        r.insert("toaster");
        r.insert("cat");
        r.insert("car");

        std::cout << "Radix tree after inserts:\n" << r.toString() << std::endl;

        if (!r.search("toasting")) {
            std::cout << "Word currently isn't in the radix tree.\n" << std::endl;
        }

        r.insert("toasting");
        if (r["toasting"]) {
            std::cout << "The word is in the radix tree.\n" << std::endl;
        }

        RadixTree t;
        t.insert("bar");
        t.insert("bark");
        t.insert("bank");

        if (r > t) {
            std::cout << "Tree \"r\" has more words than tree \"t\"\n" << std::endl;
        }

        r += t;
        std::cout << "Radix tree \"r\" after merging:\n" << r.toString() << std::endl;

        t = r;
        if (t == r) {
            std::cout << "Now the trees are equal\n" << std::endl;
        }

        r.remove("bar");
        std::cout << "Radix tree \"r\" after the removal of word \"bar\":\n" << r.toString() << std::endl;

        !t;
        !r;
        std::cout << "Radix tree \"r\" after clearing:\n" << r.toString() << std::endl;



    }
    catch (const MyException& ex) {
        std::cerr << "My exception caught: " << ex.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Unexpected exception! caught." << std::endl;
    }

    std::cout << "Program completed successfully." << std::endl;

    return 0;
}