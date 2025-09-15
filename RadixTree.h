/**
 * @author: Arturas Timofejevas (@Rave1s), VU SE 2 course 2 group
*/

#ifndef RADIX_H
#define RADIX_H

#include <iostream>
#include <string>
#include <memory>
#include <exception>

namespace RadixTreeProject {

    /** 
     * @class RadixTree
     * @brief Radix tree data structure for storing words.
     * 
     * Implementation of the class is hidden using the PImpl idiom.
    */

    class RadixTree {
        private:
            // Declaration of the class implementation, providing a pointer to the tree.
            class RadixImpl;
            std::unique_ptr<RadixImpl> pImpl;

        public:
            //Creating an alias to be used by the tree's methods.
            using ValueType = std::string;

            //Radix tree constructor - creates an empty tree.
            RadixTree();

            //Destructor - deletes the tree and frees the allocated memory.
            ~RadixTree();

            /**
             * @brief Deep copy constructor.
             * @param other The tree you wish to copy.
             */
            RadixTree(const RadixTree& other);

            /**
             * @brief Deep copy assignment operator.
             * @param other The tree you wish to copy from.
             * @return Reference to the current radix tree.
             */
            RadixTree& operator=(const RadixTree& other);

            /**
             * @brief Inserts a new word into the tree.
             * @param word The word to insert into the tree.
             * @throws an exception if the word already exists.
             */
            void insert(const ValueType& word);

            /**
             * @brief Searches for a word in the tree.
             * @param word The word to search in the tree.
             * @return True if the word exists in the tree, false otherwise.
             */
            bool search(const ValueType& word);

            /**
             * @brief Removes a given word from the tree.
             * @param word The word to remove from the tree.
             * @throws an exception if the word doesn't exist in the tree.
             */
            void remove(const ValueType& word);

            /**
             * @brief Returns a string representing each word in the tree.
             * @return A string containing all words in the tree.
             */
            std::string toString() const;

            /**
             * @brief Compares two radix trees for equality.
             * @param other The radix tree to compare with.
             * @return True if the trees are equal (have the same words), false otherwise.
             */
            bool operator==(const RadixTree& other) const;

            /**
             * @brief Compares to radix trees for inequality.
             * @param other The radix tree to compare with.
             * @return True if trees are not equal, false otherwise.
             */
            bool operator!=(const RadixTree& other) const;

            /**
             * @brief Checks if current tree has less words saved than another tree.
             * @param other The radix tree to compare with.
             * @return True if the current tree has less words than the other tree, false otherwise.
             */
            bool operator<(const RadixTree& other) const;

            /**
             * @brief Checks if current tree has more words saved than another tree.
             * @param other The radix tree to compare with.
             * @return True if the current tree has more words than the other tree, false otherwise.
             */
            bool operator>(const RadixTree& other) const;

            /**
             * @brief Checks if the current radix tree is equal to the other or has less words.
             * @param other The radix tree to compare with.
             * @return True if either condition if met, false otherwise.
             */
            bool operator<=(const RadixTree& other) const;

            /**
             * @brief Checks if the current radix tree is equal to the other or has more words.
             * @param other The radix tree to compare with.
             * @return True if either condition if met, false otherwise.
             */
            bool operator>=(const RadixTree& other) const;

            /**
             * @brief Merges the current tree with another.
             * @param other The tree to merge into the current one.
             * @return Reference to the current radix tree.
             */
            RadixTree& operator+=(const RadixTree& other);

            /**
             * @brief Removes another tree's words from the current one.
             * @param other The tree words of which will be removed from the current one.
             * @return Reference to the current radix tree.
             */
            RadixTree& operator-=(const RadixTree& other);

            /**
             * @brief Clears the current radix tree but doesn't delete it.
             * @return Reference to the current radix tree.
             */
            RadixTree& operator!();

            /**
             * @brief Searches for the given word in the tree.
             * @param word The word to be searched.
             * @return True if the word exists in the tree, false otherwise.
             */
            bool operator[](const ValueType& word) const;

    };

    /**
     * @brief Exception class to handle exceptions appearing with tree's methods.
     */
    class MyException : public std::exception {
        private:
            std::string message;

        public:
            explicit MyException(const std::string msg) : message(msg) {

            }

            const char* what() const noexcept override {
                return message.c_str();
            }
    };
}

#endif