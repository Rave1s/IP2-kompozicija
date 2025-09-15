#ifndef RADIX_H
#define RADIX_H

#include <iostream>
#include <string>
#include <memory>
#include <exception>

//TODO add explanatory comments
namespace RadixTreeProject {
    class RadixTree {
        private:
            class RadixImpl;
            std::unique_ptr<RadixImpl> pImpl;

        public:
            using ValueType = std::string;

            RadixTree();

            ~RadixTree();

            RadixTree(const RadixTree& other);

            RadixTree& operator=(const RadixTree& other);

            void insert(const ValueType& word);

            bool search(const ValueType& word);

            void remove(const ValueType& word);

            std::string toString() const;

            bool operator==(const RadixTree& other) const;
            bool operator!=(const RadixTree& other) const;
            bool operator<(const RadixTree& other) const;
            bool operator>(const RadixTree& other) const;
            bool operator<=(const RadixTree& other) const;
            bool operator>=(const RadixTree& other) const;

            RadixTree& operator+=(const ValueType& word);
            RadixTree& operator-=(const ValueType& word);
            void operator!();
            bool operator[](const ValueType& word) const;

    };

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