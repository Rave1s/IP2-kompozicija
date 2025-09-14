#include "RadixTree.h"
#include <iostream>
#include <string>
#include <unordered_map>

namespace RadixTreeProject {
    class RadixTree::RadixImpl{
        private:
            struct RadixNode {
                ValueType word;
                std::unordered_map<char, std::unique_ptr<RadixNode>> children;
                bool is_leaf;
                RadixNode(const ValueType& wrd = "") : word(word), is_leaf(false) {

                };
            };
            
            std::unique_ptr<RadixNode> root;
            RadixImpl() : root(std::make_unique<RadixNode>()) {
                
            }

        public:

    };
}