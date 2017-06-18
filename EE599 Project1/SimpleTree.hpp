//
//  SimpleTree.hpp
//  EE599 Project1
//
//  Created by Chang Yin on 4/8/17.
//  Copyright © 2017 Chang Yin. All rights reserved.
//

#ifndef SimpleTree_hpp
#define SimpleTree_hpp

#include<iostream>
#include<stdio.h>
#include<vector>
#include<deque>
#include<cmath>

using namespace std;

// ==================================================================================
// Bin class
// lock free stack implementation

template <class T>
class Node {
public:
    T data;
    Node* next;
public:
    Node(const T& d): data(d), next(0) {} // Node Constructor
} ;

template <class T>
class Bin {
    Node<T> *top;
public:
    Bin(): top(0) {}; // Stack Constructor
    void put(const T &data);//push
    T get();//pop
    void printBin();
};


// ====================================================================================
// Counter class

class Counter {
    int value;
    public :
    Counter(int key) {
        value = key;
    }
    int getAndIncrement() {
        //spin lock, compareAndSet()
        while (1) {
            int prior = value;
            if (value == prior) {
                value = value + 1;
                return prior;
            }
        }
    }
    
    int getAndDecrement() {
        // value is bounded to be non-negative
        if (value == 0) {
            return 0;
        }
        // spin lock, compareAndSet()
        while (1) {
            int prior = value;
            if (prior == value) {
                value = value - 1;
                return prior;
            }
        }
    }
    // ONLY for debug use
    int getValue(){
        while (1) {
            int prior = value;
            if (prior == value) {
                return prior;
            }
        }
    }
};

// ===================================================================================
// TreeNode class
template<class T>
class TreeNode {
public:
    Counter *counter = 0;
    TreeNode<T> *parent = 0;
    TreeNode<T> *right = 0;
    TreeNode<T> *left = 0;
    Bin<T> *bin = 0;
public:
    TreeNode(int num);
    bool isLeaf() {
        return right == 0;
    }
};

// ==================================================================================
// SimpleTree class

template <class T>
class SimpleTree {
public:
    int range;
    vector<TreeNode<T>*> leaves;
    TreeNode<T> *root = 0;
public:
    SimpleTree(int logRange);
    void add(const T &item, int score);
    T removeMin();
    void printTree();
    void printLeaves();
private:
    TreeNode<T>* buildTree(int logRange, int num);
};


#endif /* SimpleTree_hpp */


// =======================================================================================
// PQueue interface
// As PQueue is a template class, we can't make SimepleTree inherit a template class PQueue

//template <class T>
//class PQueue {
//public:
//    void add(T item, int score);
//    T removeMin();
//};

// ===========================================================================================
// compareAndSet() version of Counter
//class Counter {
//    int value;
//public :
//    Counter(int key) { value = key; }
//    int getAndIncrement() {
//        //spin lock, compareAndSet()
//        while (1) {
//            int prior = value;
//            if (__sync_bool_compare_and_swap(&value, prior, value + 1)) {
//                return prior;
//            }
//        }
//    }
//                
//    int getAndDecrement() {
//        // value is bounded to be non-negative
//        if (value == 0) {
//            return 0;
//        }
//        // spin lock, compareAndSet()
//        while (1) {
//            int prior = value;
//            if (__sync_bool_compare_and_swap(&value, prior, value - 1)) {
//                return prior;
//            }
//        }
//    }
//};



