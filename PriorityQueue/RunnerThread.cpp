//
//  RunnerThread.cpp
//  EE599 Project1
//
//  Created by Chang Yin on 4/18/17.
//  Copyright © 2017 Chang Yin. All rights reserved.
//

#include "RunnerThread.hpp"
using namespace std;


// total_iteration = 32 x 10000
// load_ratio = 45, 55, 75, 90
int num_threads = 0;
int iteration_limit = 0;
float load_ratio = 0;
int _usleep = 0;
// usleep = 0 10
// bool load_sleep = 0;
bool hide_progress = false;
int range = 1 << 16;


RunnerThread::RunnerThread(shared_ptr<SimpleTree<int>> hb,
                           shared_ptr<SimpleTree<int>> h2) {
    
    if (hb != NULL) {
        run_mode = RUN_BASIC;
        this->hb = hb;
    } else if (h2 != NULL) {
        run_mode = RUN_H2;
        this->h2 = h2;
    }

    
    count_take.store(0);
    count_put.store(0);
}

RunnerThread::RunnerThread(const RunnerThread& other) {
    // copy constructor so compiler doesn't complain on deleted mutex/atomic constructors
}

tuple<int, int> RunnerThread::run() {
    vector<thread> threads = vector<thread>();
    
    for (int i = 0; i < num_threads; i++) {
        threads.push_back(thread(&RunnerThread::run_task, this, i, iteration_limit));
    }
    
    for (int i = 0; i < num_threads; i++) {
        threads.at(i).join();
    }
    
    return tuple<int, int>(count_put.load(), count_take.load());
}

tuple<int, int> RunnerThread::run(int iteration_limit) {
    run_task(-1, iteration_limit);
    return tuple<int, int>(count_put.load(), count_take.load());
}

void RunnerThread::run_task(int thread_id, int limit) {
    int local_count_put = 0;
    int local_count_take = 0;
    
    for (int i = 0; i < limit; i++) {
        int operation = RunnerThread::random_operation(load_ratio);
        int priority;
        int out = -1;
        
        switch(operation) {
            case 0:
                priority = RunnerThread::random_priority();
                
                // sequential implementation of insert
                if (run_mode == RUN_BASIC) {
                    lock_h.lock();
                    hb->add(thread_id, priority); // insert data to sequential implementation
                    // adjust the method name accordingly
                    lock_h.unlock();
                } else if (run_mode == RUN_H2) {
                    // concurrent implementation insert
                    h2->add(thread_id, priority);
                }
                
                count_put.fetch_add(1);
                local_count_put++;
                break;
            case 1:
                
                // sequential implementation of remove_min
                if (run_mode == RUN_BASIC) {
                    lock_h.lock();
                    out = hb->removeMin();
                    lock_h.unlock();
                } else if (run_mode == RUN_H2) {
                    out = h2->removeMin();
                }
                
                count_take.fetch_add(1);
                local_count_take++;
                break;
        }
        
        if (!hide_progress && i % (limit / 10) == 0) {
            printf("Thread %3d: %3.0f%% (%6d / %6d)\n", thread_id, (float) i / limit * 100.0f, local_count_put, local_count_take);
        }
        
        if (_usleep > 0) {
            this_thread::sleep_for(chrono::microseconds(_usleep));
        }
    }
}

int RunnerThread::random_priority() {
    return rand() % (range - 0) + 0;
}


int RunnerThread::random_operation(float ratio) {
    // Returns 1 = take();
    //         0 = put();
    int threshold = ratio * RAND_MAX;
    return rand() < threshold ? 0 : 1;
}

void RunnerThread::set_attr(RunnerThread_data_t& attributes) {
    num_threads = attributes.num_threads;
    iteration_limit = attributes.iteration_limit;
    load_ratio = attributes.load_ratio;
    _usleep = attributes._usleep;
    // range = attributes.range;
}
