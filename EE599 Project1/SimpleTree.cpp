//
//  SimpleTree.cpp
//  EE599 Project1
//
//  Created by Chang Yin on 4/8/17.
//  Copyright © 2017 Chang Yin. All rights reserved.
//

#include<iostream>
#include<stdio.h>
#include<vector>
#include<deque>
#include<cmath>
#include<thread>
#include<mutex>
#include<string>


#include "SimpleTree.hpp"
using namespace std;

// ============================================================================================================
// Bin class
// as a lock-free stack implementation
//template <class T>
//class Node {
//public:
//    T data;
//    Node* next;
//public:
//    Node(const T& d): data(d), next(0) {} // Node Constructor
//} ;
//
//template <class T>
//class Bin {
//    Node<T> *top;
//public:
//    Bin(): top(0) {}; // Stack Constructor
//    void put(const T &data);//push
//    T get();//pop
//    void printBin();
//};

template <class T>
void Bin<T>::put(const T &data) {
    // create a new Node, put item on the top
    Node<T> *n = new Node<T>(data);
    while (1) {
        n->next = top;
        // If top == n->next, then top = n
        if (__sync_bool_compare_and_swap(&top, n->next, n)) {
            break;
        }
    }
}

template <class T>
T Bin<T>::get() {
    //if top is null, return null, else return the top element
    while (1) {
        Node<T> *result = top;
        if (result == 0) {
            return 0;
        }
        // If top == top, then top = top->next
        if (top && __sync_bool_compare_and_swap(&top, result, result->next)) {
            return result->data;
        }
    }
}
template <class T>
void Bin<T>::printBin() {
    Node<T> *cur = top;
    if (cur == 0) {
        cout << "0";
    }
    while (cur != 0) {
        cout << cur->data << "->";
        cur = cur->next;
    }
    cout<<endl;
}
// ===========================================================================================================
// class Counter lock-free implementation
// the number of items in the leaves of each node's left subtree
// left subtree contains all nodes that have higher priority

//class Counter {
//    int value;
//public :
//    Counter(int key) {
//        value = key;
//    }
//    int getAndIncrement() {
//        //spinning lock
//        while (1) {
//            int prior = value;
//            if (value == prior) {
//                value = value + 1;
//                return prior;
//            }
//        }
//    }
//    
//    int getAndDecrement() {
//        // can't make it negative, bounded
//        if (value == 0) {
//            return 0;
//        }
//        // spinning lock
//        while (1) {
//            int prior = value;
//            if (prior == value) {
//                value = value - 1;
//                return prior;
//            }
//        }
//    }
//    // ONLY for debug use
//    int getValue(){
//        while (1) {
//            int prior = value;
//            if (prior == value) {
//                return prior;
//            }
//        }
//    }
//};

// ===========================================================================================================
// template class TreeNode
// the inner TreeNode of SimpleTree

//template<class T>
//class TreeNode {
//public:
//    Counter *counter = 0;
//    TreeNode<T> *parent = 0;
//    TreeNode<T> *right = 0;
//    TreeNode<T> *left = 0;
//    Bin<T> *bin = 0;
//public:
//    TreeNode(int num);
//    bool isLeaf() {
//        return right == 0;
//    }
//};

//TreeNode Constructor
template<class T>
TreeNode<T>::TreeNode(int num) {
    counter = new Counter(num);
    bin = new Bin<T>();
    parent = 0;
    right = 0;
    left = 0;
}

// ===========================================================================================================
// template class SimpleTree

//template <class T>
//class SimpleTree {
//public:
//    int range;
//    vector<TreeNode<T>*> leaves;
//    TreeNode<T> *root = 0;
//public:
//    SimpleTree(int logRange);
//    void add(const T &item, int score);
//    T removeMin();
//    void printTree();
//    void printLeaves();
//private:
//    TreeNode<T>* buildTree(int logRange, int num);
//};

// ===========================================================================================================
// SimpleTree class implement

template <class T>
SimpleTree<T>::SimpleTree(int logRange){
    range = logRange;
    // buildTree() return root node reference
    root = buildTree(logRange, 0);
}

template <class T>
TreeNode<T>* SimpleTree<T>::buildTree(int logRange, int num){
    int depth = 0;
    root = new TreeNode<T>(0);
    deque<TreeNode<T>*> queue;
    queue.push_back(root);
    while(depth < logRange) {
       
        for(int i = 0; i < pow(2, depth); i++) {
            // expand a node from queue
            TreeNode<T> *cur = queue.front();
            queue.pop_front();
            // generate left node, link to parent node
            cur->left = new TreeNode<T>(0);
            cur->left->parent = cur;
            queue.push_back(cur->left);
            // generate right node, link to parent node
            cur->right = new TreeNode<T>(0);
            cur->right->parent = cur;
            queue.push_back(cur->right);
            // add leaf nodes to vector<TreeNode<T>*> leaves
            if (depth == logRange - 1) {
                leaves.push_back(cur->left);
                leaves.push_back(cur->right);
            }
        }
        depth++;
    }
  
    return root;
}

template <class T>
void SimpleTree<T>::add(const T &item, int score){
    // 1 << range == 2 ^ range
    if (score >= (1 << range)) {
        cout << "You've exceeded score range!!! The maximum score is " << (1 << range) - 1 << "." << endl;
        return;
    }
    // get leave node reference of corresponding score, put items in
    TreeNode<T> *node = leaves.at(score);
    node->bin->put(item);
    // traverse from leaf-to-root, increment counter value if ascending from left side
    while (node != root) {
        TreeNode<T> *parent = node->parent;
        if (node == parent->left) {
            parent->counter->getAndIncrement();
        }
        node = parent;
    }
}

template <class T>
T SimpleTree<T>::removeMin(){
    //traverse down from root-to-leave
    TreeNode<T> *cur = root;
    while(!cur->isLeaf()) {
        Counter *count = cur->counter;
        //go to left if counter value > 0, else go to right
        if (count->getAndDecrement() > 0) {
            cur = cur->left;
        } else {
            cur = cur->right;
        }
    }
    // cout << "Removed Item:  ";
    // cur->bin->printBin();
    // get item from leave node's bin
    return cur->bin->get();
}

// Debug: Print out the all tree structure
template <class T>
void SimpleTree<T>::printTree(){
    deque<TreeNode<T>*> queue;
    int depth = 0;
    queue.push_back(root);
    while (depth <= range) {
        int size = (int) queue.size();
        cout<<"Depth " << depth <<":  ";
        for(int i = 0; i < size; i++) {
            TreeNode<T> *cur = queue.front();
            queue.pop_front();
            queue.push_back(cur->left);
            queue.push_back(cur->right);
            cout << cur->counter->getValue() << "  ";
        }
        depth++;
        cout << endl;
    }
}

// Debug: Print all leave nodes
template <class T>
void SimpleTree<T>::printLeaves(){
    cout << "Leaves :  " << endl;
    for (int i = 0; i < leaves.size(); i++) {
        leaves.at(i)->bin->printBin();
    }
}
