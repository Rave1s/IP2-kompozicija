#include "RadixTree.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>

namespace RadixTreeProject {
    class RadixTree::RadixImpl{
        private:
            struct RadixNode {
                ValueType word;
                std::unordered_map<char, std::unique_ptr<RadixNode>> children;
                bool isLeaf;
                RadixNode(const ValueType& wrd = "") : word(word), isLeaf(false) {

                };
            };

        public:
            std::unique_ptr<RadixNode> root;
            RadixImpl() : root(std::make_unique<RadixNode>()) {

            }

            ~RadixImpl() {
                freeMemory();
            }

            void insert(const ValueType& word) {
                RadixNode* node = root.get();
                size_t index = 0;

                while (index < word.size()) {
                    char c = word[index];
                    if (node->children.count(c) == 0) {
                        node->children[c] = std::make_unique<RadixNode>(word.substr(index));
                        node->children[c]->isLeaf = true;
                        return;
                    }

                    RadixNode* child = node->children[c].get();
                    size_t matchingLength = 0;
                    while (matchingLength < child->word.size() && index + matchingLength < word.size() && word[index + matchingLength] == child->word[matchingLength]) {
                        ++matchingLength;
                    }
                    if (matchingLength == child->word.size()) {
                        node = child;
                        index += matchingLength;
                    }
                    else {
                        auto nodeSplit = std::make_unique<RadixNode>(child->word.substr(0, matchingLength));
                        nodeSplit->children[child->word[matchingLength]] = std::move(node->children[c]);
                        nodeSplit->children[child->word[matchingLength]]->word = child->word.substr(matchingLength);
                        nodeSplit->isLeaf = false;
                        node->children[c] = std::move(nodeSplit);

                        if (index + matchingLength < word.size()) {
                            node->children[c]->children[word[index + matchingLength]] = std::make_unique<RadixNode>(word.substr(index + matchingLength));
                            node->children[c]->children[word[index + matchingLength]]->isLeaf = true;
                        }
                        else {
                            node->children[c]->isLeaf = true;
                        }
                        return;
                    }
                }

                node->isLeaf = true;
            }

            bool search(const ValueType& word) {
                const RadixNode* node = root.get();
                size_t index = 0;

                while (index < word.size()) {
                    char c = word[index];

                    if (node->children.count(c) == 0)
                    {
                        return false;
                    }

                    const RadixNode* child = node->children.at(c).get();
                    size_t matchingLength = 0;

                    while (matchingLength < child->word.size() && index + matchingLength < word.size() && word[index + matchingLength] == child->word[matchingLength]) {
                        ++matchingLength;
                    }
                    if (matchingLength != child->word.size()) {
                        return false;
                    }

                    node = child;
                    index += matchingLength;
                }

                return node->isLeaf;
            }

            void remove(const ValueType& word) {
                RadixNode* node = root.get();
                std::vector<std::pair<RadixNode*, char>> removablePart;
                size_t index = 0;

                while (node && index < word.size()) {
                    char c = word[index];
                    auto existingChild = node->children.find(c);
                    if (existingChild == node->children.end()) {
                        throw MyException("Word not found. Couldn't remove");
                    }

                    RadixNode* child = existingChild->second.get();
                    size_t matchingLength = 0;

                    while (matchingLength < child->word.size() && index + matchingLength < word.size() && word[index + matchingLength] == child->word[matchingLength]) {
                        ++matchingLength;
                    }
                    if (matchingLength != child->word.size()) {
                        throw MyException("Word not found. Couldn't remove");
                    }

                    removablePart.emplace_back(node, c);
                    node = child;
                    index += matchingLength;
                }

                if (!node || !node->isLeaf || index != word.size()) {
                    throw MyException("Word not found. Couldn't remove");
                }

                node->isLeaf = false;

                for (int i = removablePart.size() - 1; i >= 0; --i) {
                    RadixNode* parent = removablePart[i].first;
                    char c = removablePart[i].second;
                    RadixNode* child = parent->children[c].get();

                    if (!child->isLeaf && child->children.empty()) {
                        parent->children.erase(c);
                    }
                    else {
                        break;
                    }
                }
            }

            //TODO add the toString method and the operators
            void collectAllWords(const RadixNode* currentNode, ValueType prefix, std::vector<ValueType>& allWords) const {
                if (!currentNode) {
                    return;
                }

                prefix += currentNode->word;
                if (currentNode->isLeaf) {
                    allWords.push_back(prefix);
                }

                for (const auto& [character, child] : currentNode->children) {
                    collectAllWords(child.get(), prefix, allWords);
                }
            }


            std::string toString() const {
                std::vector<ValueType> allSavedWords;
                collectAllWords(root.get(), "", allSavedWords);
                std::ostringstream os;

                for (const ValueType& currentWord : allSavedWords) {
                    os << currentWord << " ";

                }

                return os.str();
            }

            static std::unique_ptr<RadixNode> copyRadixTree(const RadixNode* node) {
                if (!node) {
                    return nullptr;
                }
                auto copy = std::make_unique<RadixNode>(node->word);
                copy->isLeaf = node->isLeaf;
                for (const auto& ptr : node->children) {
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