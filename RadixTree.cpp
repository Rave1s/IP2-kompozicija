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

        public:
            std::unique_ptr<RadixNode> root;
            RadixImpl() : root(std::make_unique<RadixNode>()) {

            }

            ~RadixImpl() {
                freeMemory();
            }

            

            static std::unique_ptr<RadixNode> copyRadixTree(const RadixNode* node) {
                if (!node) {
                    return nullptr;
                }
                auto copy = std::make_unique<RadixNode>(node->word);
                copy->is_leaf = node->is_leaf;
                for(const auto& ptr : node->children) {
                    copy->children[ptr.first] = copyRadixTree(ptr.second.get());
                }
                return copy;
            }

            void freeMemory() {
                root.reset();
            }
    };

    RadixTree::RadixTree() : pImpl(std::make_unique<RadixImpl>()) {
                
    }

    RadixTree::~RadixTree() = default;

    RadixTree::RadixTree(const RadixTree& other) : pImpl(std::make_unique<RadixImpl>()) {
        pImpl->root = RadixImpl::copyRadixTree(other.pImpl->root.get());
    }

    RadixTree& RadixTree::operator=(const RadixTree& other) {
        if (this != &other) {
            pImpl = std::make_unique<RadixImpl>();
            pImpl->root = RadixImpl::copyRadixTree(other.pImpl->root.get());
        }
        return *this;
    }
}