#include "RadixTree.h"
#include <iostream>
#include <fstream>
#include <cassert>

using namespace RadixTreeProject;

void log(std::ostream& os, const std::string& message) {
    std::cout << message << std::endl;
    os << message << std::endl;
}

int main() {
    std::ofstream logFile("testlog.txt");

    if (!logFile) {
        std::cerr << "Failed to open test log file\n" << std::endl;
        return 1;
    }

    try {
        RadixTree rTree;

        log(logFile, "Initial tree:\n");
        log(logFile, rTree.toString() + "\n");

        log(logFile, "Inserting words: toast, toaster, toasting\n");
        rTree.insert("toast");
        rTree.insert("toaster");
        rTree.insert("toasting");
        assert(rTree.search("toaster"));
        assert(rTree["toasting"]);
        log(logFile, "Words inserted correctly. Both search methods work.\n");
        log(logFile, "Words after insertion:\n");
        log(logFile, rTree.toString() + "\n");

        RadixTree tTree;
        RadixTree pTree;

        tTree = rTree;
        pTree.insert("car");
        pTree.insert("cat");
        assert(rTree == tTree);
        log(logFile, "Cloning operator test passed.\n");
        assert(rTree != pTree);
        assert(rTree > pTree);
        assert(pTree < tTree);
        assert(rTree >= tTree);
        assert(rTree <= tTree);
        log(logFile, "Logical operator test passed.\n");

        rTree += pTree;
        assert(rTree.search("cat"));
        log(logFile, "Tree merging successful.\n");
        log(logFile, "Words after merging:\n");
        log(logFile, rTree.toString() + "\n");

        rTree -= pTree;
        assert(!rTree.search("car"));
        rTree.remove("toasting");
        assert(!rTree.search("toasting"));
        log(logFile, "Tree unmerging and word removal test passed.\n");
        log(logFile, "Words after removal:\n");
        log(logFile, rTree.toString() + "\n");

        RadixTree emptyTree;
        !rTree;
        assert(rTree == emptyTree);
        log(logFile, "Tree clearing test passed.\n");
        log(logFile, "Tree after clearing:\n");
        log(logFile, rTree.toString() + "\n");

        log(logFile, "All tests passed successfully!\n");
    }
    catch (const MyException& ex) {
        std::cerr << "My exception caught: " << ex.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Unexpected exception! caught." << std::endl;
    }

    return 0;
}