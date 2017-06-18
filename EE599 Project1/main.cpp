//
//  main.cpp
//  EE599 Project1
//
//  Created by Chang Yin on 4/5/17.
//  Copyright © 2017 Chang Yin. All rights reserved.
//


//include a library of input and outputs, cout, cin
#include <iostream>
#include "SimpleTree.hpp"
#include "RunnerThread.hpp"
#include <chrono>



// SimpleTree sequentialTest()
void sequentialTest() {
    cout << "======================= Sequential Test for Tree-Based Priority Queue ===================================" << endl;
    SimpleTree<int> *tree1 = new SimpleTree<int>(3);
    tree1->printTree();
    tree1->printLeaves();
    cout << "add item : 3, score : 0" << endl;
    tree1->add(3, 0);
    cout << "add item : 4, score : 4" << endl;
    tree1->add(4, 4);
    cout << "add item : 5, score : 7" << endl;
    tree1->add(5, 7);
    cout << "add item : 6, score : 2" << endl;
    tree1->add(6, 2);
    cout << "add item : 7, score : 6" << endl;
    tree1->add(7, 6);
    cout << "add item : 8, score : 5" << endl;
    tree1->add(8, 5);
    cout << "add item : 7, score : 2" << endl;
    tree1->add(7, 2);
    tree1->printTree();
    tree1->printLeaves();
    cout << "Removed Item:  " << tree1->removeMin() << endl;
    cout << "Removed Item:  " << tree1->removeMin() << endl;
    cout << "Removed Item:  " << tree1->removeMin() << endl;
    cout << "Removed Item:  " << tree1->removeMin() << endl;
    cout << "Removed Item:  " << tree1->removeMin() << endl;
    cout << "Removed Item:  " << tree1->removeMin() << endl;
    cout << "Removed Item:  " << tree1->removeMin() << endl;
    cout << "Removed Item:  " << tree1->removeMin() << endl;
    cout << "Removed Item:  " << tree1->removeMin() << endl;
    tree1->printTree();
    cout << "add item : 1, score : 0" << endl;
    tree1->add(1, 0);
    cout << "add item : 2, score : 1" << endl;
    tree1->add(2, 1);
    cout << "add item : 3, score : 2" << endl;
    tree1->add(3, 2);
    cout << "add item : 4, score : 3" << endl;
    tree1->add(4, 3);
    cout << "add item : 5, score : 4" << endl;
    tree1->add(5, 4);
    cout << "add item : 6, score : 5" << endl;
    tree1->add(6, 5);
    cout << "add item : 7, score : 6" << endl;
    tree1->add(7, 6);
    cout << "add item : 8, score : 7" << endl;
    tree1->add(8, 7);
    tree1->printTree();
    tree1->printLeaves();
}

using namespace std;//shortcut for format, use std format automatically

int main(int argc, char *argv[]) {

    
// =============================================================================================
   
    // m = 2 ^ 16 - 1 = 65535
    sequentialTest();
    int depth = 16;
    auto h1 = make_shared<SimpleTree<int>>(depth);
    auto h2 = make_shared<SimpleTree<int>>(depth);
    
    
    RunnerThread_data_t attributes = RunnerThread_data_t();
    
    if (argc > 1) {
        attributes.num_threads = atoi(argv[1]);
        attributes.iteration_limit = 320000 / attributes.num_threads;
        attributes.load_ratio = atoi(argv[2]) / 100.0;
        attributes._usleep = atoi(argv[3]);
        // printf("num_threads %d, iteration_limit %d, load_ratio %f, _usleep %d.\n", attributes.num_threads,
        // attributes.iteration_limit, attributes.load_ratio, attributes._usleep);
        // cout << attributes.num_threads << " " << attributes.iteration_limit << " " << attributes.load_ratio << " "
        // << attributes._usleep << endl;
    }
    
    RunnerThread_data_t sequential_attributes = RunnerThread_data_t();
    if (argc > 1) {
        sequential_attributes.num_threads = 1;
        sequential_attributes.iteration_limit = 320000;
        sequential_attributes.load_ratio = atoi(argv[2]) / 100.0;
        sequential_attributes._usleep = atoi(argv[3]);
        // printf("num_threads %d, iteration_limit %d, load_ratio %f, _usleep %d.\n", attributes.num_threads,
        // attributes.iteration_limit, attributes.load_ratio, attributes._usleep);
        // cout << attributes.num_threads << " " << attributes.iteration_limit << " " << attributes.load_ratio << " "
        // << attributes._usleep << endl;
    }
    
     RunnerThread r1 = RunnerThread(h1, NULL);
     if (argc > 1) r1.set_attr(sequential_attributes);
     cout << "=============================== Sequential Performance ===================================" << endl;
     printf("num_threads = %d, iteration_limit = %d, load_ratio = %f, _usleep = %d.\n", sequential_attributes.num_threads,
           sequential_attributes.iteration_limit, sequential_attributes.load_ratio, sequential_attributes._usleep);

     auto start_s1 = chrono::high_resolution_clock::now();
     r1.run();
     auto stop_s1 = chrono::high_resolution_clock::now();
     cout << "================================ Concurrent Performance ==================================" << endl;
     RunnerThread r2 = RunnerThread(NULL, h2);
     if (argc > 1) r2.set_attr(attributes);
     printf("num_threads = %d, iteration_limit = %d, load_ratio = %f, _usleep = %d.\n", attributes.num_threads,
           attributes.iteration_limit, attributes.load_ratio, attributes._usleep);

     auto start_s2 = chrono::high_resolution_clock::now();
     r2.run();
     auto stop_s2 = chrono::high_resolution_clock::now();
    
     cout << endl;
     cout << "r1's run time is: " << chrono::duration_cast<chrono::milliseconds>(stop_s1 - start_s1).count()
         << " milliseconds" << endl;
     cout << "r2's run time is: " << chrono::duration_cast<chrono::milliseconds>(stop_s2 - start_s2).count()
         << " milliseconds" << endl;
     cout << "===================================== End =======================================" << endl;
    return 0;
}


// ===============================================================================
// Counter test
//    Counter *c1 = new Counter(0);
//    cout<< "incre "<< c1->getAndIncrement()<<endl;
//    cout<< "incre "<< c1->getAndIncrement()<<endl;


//=========================================================================================
// Bin test
//    Bin<int> *bin1 = new Bin<int>();
//    bin1->put(4);
//    bin1->printBin();
//    bin1->put(5);
//    bin1->printBin();
//    bin1->get();
//    bin1->printBin();
//    bin1->get();
//    bin1->printBin();

