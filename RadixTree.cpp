#include "RadixTree.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>

namespace RadixTreeProject {
    class RadixTree::RadixImpl{
        private:
            /**
             * @brief Represents a radix tree's node.
             */
            struct RadixNode {
                ValueType word; // The prefix being stored in the node.
                std::unordered_map<char, std::unique_ptr<RadixNode>> children; // Map of all children of the current node.
                bool isEndOfWord; // Marker to mark wheter this node represents an ending of a word.

                /**
                 * @brief Constructs an empty radix tree node.
                 * @param wrd The word (prefix) to be saved at the current node.
                 * By default isEndOfWord marker is assigned to false,
                 * as the program will determine whether this is trule the end of a word later.
                 */
                RadixNode(const ValueType& wrd = "") : word(wrd), isEndOfWord(false) {

                };
            };

        public:
            /**
             * @brief Constructor that automatically craetes a root node.
             */
            std::unique_ptr<RadixNode> root;
            RadixImpl() : root(std::make_unique<RadixNode>()) {

            }

            /**
             * @brief Destructor that frees any alocated memory.
             */
            ~RadixImpl() {
                deleteTree();
            }

            void insert(const ValueType& word) {
                RadixNode* node = root.get();
                size_t index = 0;

                while (index < word.size()) {
                    char c = word[index];
                    /**
                     * Create a child node if none exists matching the prefix.
                     * Place the prefix into the child node.
                    */
                    if (node->children.count(c) == 0) {
                        node->children[c] = std::make_unique<RadixNode>(word.substr(index));
                        node->children[c]->isEndOfWord = true;
                        return;
                    }

                    RadixNode* child = node->children[c].get();
                    size_t matchingLength = 0;
                    // Check to see how much of the prefix matches.
                    while (matchingLength < child->word.size() && index + matchingLength < word.size() && word[index + matchingLength] == child->word[matchingLength]) {
                        ++matchingLength;
                    }
                    // If the entire child's prefix matches, move to it.
                    if (matchingLength == child->word.size()) {
                        node = child;
                        index += matchingLength;
                    }
                    /**
                     * Else, split the node, look for how long the two match,
                     * once they don't match, save the matching part as the new prefix for the child,
                     * place the rest of child's prefix into a grandchild node,
                     * and create another grandchild, placing the prefix there.
                     */
                    else {
                        auto nodeSplit = std::make_unique<RadixNode>(child->word.substr(0, matchingLength));
                        nodeSplit->children[child->word[matchingLength]] = std::move(node->children[c]);
                        nodeSplit->children[child->word[matchingLength]]->word = child->word.substr(matchingLength);
                        nodeSplit->isEndOfWord = false;
                        node->children[c] = std::move(nodeSplit);

                        if (index + matchingLength < word.size()) {
                            node->children[c]->children[word[index + matchingLength]] = std::make_unique<RadixNode>(word.substr(index + matchingLength));
                            node->children[c]->children[word[index + matchingLength]]->isEndOfWord = true;
                        }
                        else {
                            node->children[c]->isEndOfWord = true;
                        }
                        return;
                    }
                }
                
                // If the word already exists, throw an exception.
                if (node->isEndOfWord == true) {
                    throw MyException("The word already exists in the tree");
                }
                // Else mark the current node as the end of the word.
                else {
                    node->isEndOfWord = true;
                }
            }

            bool search(const ValueType& word) {
                const RadixNode* node = root.get();
                size_t index = 0;

                while (index < word.size()) {
                    char c = word[index];

                    // If the current node doesn't have matching children, return false.
                    if (node->children.count(c) == 0)
                    {
                        return false;
                    }

                    const RadixNode* child = node->children.at(c).get();
                    size_t matchingLength = 0;

                    // Check for how long the prefixes match.
                    while (matchingLength < child->word.size() && index + matchingLength < word.size() && word[index + matchingLength] == child->word[matchingLength]) {
                        ++matchingLength;
                    }
                    // If the length doesn't match (meaning the word is shorter than the child prefix), return false.
                    if (matchingLength != child->word.size()) {
                        return false;
                    }

                    node = child;
                    index += matchingLength;
                }

                return node->isEndOfWord;
            }

            void remove(const ValueType& word) {
                RadixNode* node = root.get();
                std::vector<std::pair<RadixNode*, char>> removablePart;
                size_t index = 0;

                while (node && index < word.size()) {
                    char c = word[index];
                    // If the child with matching prefix doesn't exist, throw exception.
                    auto existingChild = node->children.find(c);
                    if (existingChild == node->children.end()) {
                        throw MyException("Word not found. Couldn't remove");
                    }

                    RadixNode* child = existingChild->second.get();
                    size_t matchingLength = 0;

                    // Check for how long the prefixes match.
                    while (matchingLength < child->word.size() && index + matchingLength < word.size() && word[index + matchingLength] == child->word[matchingLength]) {
                        ++matchingLength;
                    }
                    // If the length doesn't match (meaning the word is shorter than the child prefix), throw exception.
                    if (matchingLength != child->word.size()) {
                        throw MyException("Word not found. Couldn't remove");
                    }

                    // Save how much of the tree will be deleted.
                    removablePart.emplace_back(node, c);
                    node = child;
                    index += matchingLength;
                }

                /**
                 * If:
                 * 1) node is null,
                 * 2) current node's prefix isn't marked as an end of a word OR,
                 * 3) index doesn't match the length of the word,
                 * throw exception.
                 */
                if (!node || !node->isEndOfWord || index != word.size()) {
                    throw MyException("Word not found. Couldn't remove");
                }

                // Remove this prefix as the end of the word.
                node->isEndOfWord = false;

                /**
                 * Iterate backwards through the nodes (leaf to root),
                 * checking to see if the child isn't marked as a word ending,
                 * and doesn't have children.
                 * If not - deletes it, else - breaks the cycle.
                 */
                for (int i = removablePart.size() - 1; i >= 0; --i) {
                    RadixNode* parent = removablePart[i].first;
                    char c = removablePart[i].second;
                    RadixNode* child = parent->children[c].get();

                    if (!child->isEndOfWord && child->children.empty()) {
                        parent->children.erase(c);
                    }
                    else {
                        break;
                    }
                }
            }

            /**
             * @brief Collects all words saved in the radix tree.
             */
            void collectAllWords(const RadixNode* currentNode, ValueType prefix, std::vector<ValueType>& allWords) const {
                if (!currentNode) {
                    return;
                }

                prefix += currentNode->word;
                if (currentNode->isEndOfWord) {
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

            /**
             * @brief Compares if trees are equal (have the same saved words).
             * @param thisNode Current node.
             * @param otherNode Node to compare with.
             * @return True if they are equal, false otherwise.
             */
            bool compareTrees(const RadixNode* thisNode, const RadixNode* otherNode) const {
                if (!thisNode && !otherNode) {
                    return true;
                }
                if (!thisNode || !otherNode) {
                    return false;
                }

                if (thisNode->word != otherNode->word || thisNode->isEndOfWord != otherNode->isEndOfWord || thisNode->children.size() != otherNode->children.size()) {
                    return false;
                }

                for (const auto& [thisNodesChar, thisNodesChild] : thisNode->children) {
                    auto matchingChild = otherNode->children.find(thisNodesChar);
                    if (matchingChild == otherNode->children.end()) {
                        return false;
                    }

                    if (!compareTrees(thisNodesChild.get(), matchingChild->second.get())) {
                        return false;
                    }
                }

                return true;
            }

            static std::unique_ptr<RadixNode> copyRadixTree(const RadixNode* node) {
                if (!node) {
                    return nullptr;
                }
                auto copy = std::make_unique<RadixNode>(node->word);
                copy->isEndOfWord = node->isEndOfWord;
                for (const auto& ptr : node->children) {
                    copy->children[ptr.first] = copyRadixTree(ptr.second.get());
                }
                return copy;
            }

            // Clears the radix tree.
            void emptyTree() {
                root = std::make_unique<RadixNode>();
            }

            // Deletes the radix tree.
            void deleteTree() {
                root.reset();
            }
    };

    // Constructor.
    RadixTree::RadixTree() : pImpl(std::make_unique<RadixImpl>()) {
                
    }

    // Destructor.
    RadixTree::~RadixTree() = default;

    // Deep copy constructor.
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


    bool RadixTree::operator==(const RadixTree& other) const{
        return pImpl->compareTrees(pImpl->root.get(), other.pImpl->root.get());
    }

    bool RadixTree::operator!=(const RadixTree& other) const{
        return !(*this == other);
    }

    bool RadixTree::operator<(const RadixTree& other) const {
        std::vector<ValueType> wordsInThisTree;
        std::vector<ValueType> wordsInOtherTree;

        pImpl->collectAllWords(pImpl->root.get(), "", wordsInThisTree);
        other.pImpl->collectAllWords(other.pImpl->root.get(), "", wordsInOtherTree);

        return wordsInThisTree.size() < wordsInOtherTree.size();
    }

    bool RadixTree::operator>(const RadixTree& other) const {
        return !(*this < other);
    }

    bool RadixTree::operator<=(const RadixTree& other) const {
        return ((*this < other) || (*this == other));
    }

    bool RadixTree::operator>=(const RadixTree& other) const {
        return !(*this <= other);
    }

    RadixTree& RadixTree::operator+=(const RadixTree& other) {
        std::vector<ValueType> wordsInOtherTree;
        other.pImpl->collectAllWords(other.pImpl->root.get(), "", wordsInOtherTree);

        for (const auto& word : wordsInOtherTree) {
            this->insert(word);
        }

        return *this;
    }

    RadixTree& RadixTree::operator-=(const RadixTree& other) {
        std::vector<ValueType> wordsInOtherTree;
        other.pImpl->collectAllWords(other.pImpl->root.get(), "", wordsInOtherTree);

        for (const auto& word : wordsInOtherTree) {
            this->remove(word);
        }

        return *this;
    }

    RadixTree& RadixTree::operator!() {
        pImpl->emptyTree();
        
        return *this;
    }

    bool RadixTree::operator[](const ValueType& word) const {
        return pImpl->search(word);
    }
}