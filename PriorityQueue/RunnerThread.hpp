//
//  RunnerThread.hpp
//  EE599 Project1
//
//  Created by Chang Yin on 4/18/17.
//  Copyright © 2017 Chang Yin. All rights reserved.
//


#include <stdio.h>

#ifndef RunnerThread_hpp
#define RunnerThread_hpp

// This is a possible implementation of benchmark code for priority queue
// This is how to invoke the runner threads:
//
// ====================
//   // replace with shared pointer to your priority queue implementation
// auto h = make_shared<HeapPriorityBasic<int>>();
// auto rt = RunnerThread(h, NULL, cv);
//   // first argument is pointer to sequential implementation
//   // second argument is pointer to concurrent implementation
//   // third argument is optional verificator objec
//
// auto begin = chrono::high_resolution_clock::now();
// auto tracker = rt.run();
// auto end = chrono::high_resolution_clock::now();
// ====================
//
// begin and end now contains timestamps that you can use to time execution speed
// tracker is 2-element tuple that contains number of insert/remove operations
//
// you need to have the following global variables defined:
// - bool hide_progress // prevents printing per-thread progress to STDOUT
// - load_sleep // thread will sleep about this many microseconds (us, *NOT* ms)
//              // between iteration. Put 0 to disable sleep
//
// and you also need the following definition:
// - LOAD_PRIORITY_MAX // maximum and minimum priority score
// - LOAD_PRIORITY_MIN // recommended value is min = 1, max = whatever you want
//                     // this is simply to randomize priority score to fall
//                     // within a certain region
// - LOAD_PUT_TO_TAKE_RATIO // a floating point of insert to remove ratio
//                          // 0.5f will yield about 50% insert 50% remove
//                          // 0.9f yields 90% insert 10% remove (make your tree so deep)

#define RUN_BASIC 0 // for sequential and coarse-grained implementation
#define RUN_H2 1    // for multithread-optimized implementation


#include <stdio.h>
#include <chrono>
#include <atomic>

#include "SimpleTree.cpp" // replace with multithread imlementation

using namespace std;

typedef struct RunnerThread_data_t {
    int num_threads;
    int iteration_limit;
    float load_ratio;
    int _usleep;
    // _usleep = 0 10
    // bool load_sleep = 0;
    int range;
} RunnerThread_data_t;


class RunnerThread {
public:
    RunnerThread(std::shared_ptr<SimpleTree<int>> hb, std::shared_ptr<SimpleTree<int>> h2);
    RunnerThread(const RunnerThread& other);
    
    // the tuple contains number of insert and remove_min
    // you don't have to record this calls if you don't want to
    std::tuple<int, int> run();
    std::tuple<int, int> run(int limit);
    
    static int random_priority(); // randomly selects a priority score
    // see implementation details
    static int random_operation(); // randomly selects between insert / remove_min
    static int random_operation(float ratio); // same but with specified insert to remove_min ratio
    void set_attr(RunnerThread_data_t& attributes);
    
private:
    void run_task(int thread_id, int limit); // the per-thread runner task
    
    int run_mode;
    std::shared_ptr<SimpleTree<int>> hb; // replace this with shared pointer to sequential impl.
    std::shared_ptr<SimpleTree<int>> h2; // replace with concurrent impl
    
    std::mutex lock_h; // used to serializ coarse-grained implementation
    // optional if you put the coarse lock in your structure
    
    std::atomic_int count_take; // number of remove_min
    std::atomic_int count_put;  // number of insert
    
};


#endif /* RunnerThread_hpp */
