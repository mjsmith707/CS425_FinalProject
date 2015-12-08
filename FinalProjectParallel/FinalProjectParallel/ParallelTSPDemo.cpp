//
//  ParallelTSPDemo.cpp
//  FinalProject
//
//  Created by Max on 12/6/15.
//  Copyright (c) 2015 Matt. All rights reserved.
//

#include "ParallelTSPDemo.h"

#define N_THREADS 4
#define N_TASKS_PER_THREAD 10

ParallelTSPDemo::Tour ParallelTSPDemo::runParallelDFS(unsigned int initialCity){
    ParallelTSPDemo::Tour firstNode;
    ParallelTSPDemo::Tour node;
    std::queue<ParallelTSPDemo::Tour> frontier;
    frontier.push(firstNode);
    
    size_t sizeOfTaskQueue = N_THREADS * N_TASKS_PER_THREAD;

    while(frontier.size() > 0 && frontier.size() < sizeOfTaskQueue){
        node = (frontier.front());
        frontier.pop();
        
        for(int i = 0; i < num_cities && frontier.size() < sizeOfTaskQueue; i++){
            if(!node.is_present(i)){
                ParallelTSPDemo::Tour neighbor = node;
                neighbor.add_city(i);
                frontier.push(neighbor);
            }
        }
    }
    
    Shared_Queue<std::pair<unsigned int, ParallelTSPDemo::Tour>> taskQueue;
    
    std::vector<ParallelTSPDemo::Tour> sharedResults;
    std::vector<std::thread> threadPool;
    std::atomic<unsigned int> sharedBound(std::numeric_limits<unsigned int>::max());
    
    sharedResults.resize(sizeOfTaskQueue);
    
    for(unsigned int i = 0; i < sizeOfTaskQueue; i++){
        ParallelTSPDemo::Tour tmp(frontier.front());
        frontier.pop();
        std::pair<unsigned int, ParallelTSPDemo::Tour> pairForTaskQueue = std::make_pair(i, tmp);
        taskQueue.push(pairForTaskQueue);
    }
    
    for(int i = 0; i < N_THREADS; i++){
        threadPool.push_back(std::thread(&ParallelTSPDemo::RunAgainstTaskQueue, this, &taskQueue, &sharedResults, &sharedBound));
    }
    
    for(int i = 0; i < N_THREADS; i++){
        threadPool[i].join();
    }
    
    ParallelTSPDemo::Tour solution(std::numeric_limits<unsigned int>::max());
    for(int i = 0; i < sharedResults.size(); i++){
        if(sharedResults[i].better_than(solution)){
            solution = sharedResults[i];
        }
    }
    
    return solution;
}

void ParallelTSPDemo::RunAgainstTaskQueue(Shared_Queue<std::pair<unsigned int, ParallelTSPDemo::Tour>>* taskQueue, std::vector<ParallelTSPDemo::Tour>* sharedResults, std::atomic<unsigned int>* sharedBound){
    while(!taskQueue->isEmpty()){
        
        // try-catch because there's a potential race between the isEmpty() timing and
        try{
            std::pair<unsigned int, ParallelTSPDemo::Tour> currentTaskNode = taskQueue->pop();
            ParallelTSPDemo::Tour localTour = currentTaskNode.second;
            ParallelTSPDemo::Tour localBest(*sharedBound);
            ParallelTSPDemo::DFS(localTour, localBest, sharedBound);
            (*sharedResults)[currentTaskNode.first] = localBest;
            
            // Do an atomic compare and swap
            unsigned int localBound = localBest.getMileage();
            unsigned int prevBound = *sharedBound;
            while ((prevBound > localBound) && (!(*sharedBound).compare_exchange_weak(prevBound, localBound)));
        } catch(std::runtime_error e){
            
            // nothing left in the task queue, so this thread can leave
            return;
        }
    }
}

void ParallelTSPDemo::DFS(ParallelTSPDemo::Tour& t, ParallelTSPDemo::Tour& best, std::atomic<unsigned int>* sharedBound) {
    if (!t.better_than(best, sharedBound)) return; // if we've already seen something better
    /*
    
     potential race here, since there's no lock between the time checking for better_than(best, sharedBound) and updating shared bound
     is this a correctness problem? Don't think so, at worst a (potentially major?) performance issue IMO. Worst case:
    
     t1 finds best solution for entire search space
     t2 finds solution better than sharedBound and its local best
     t1 writes to sharedBound
     t2 writes to sharedBound (overwriting optimal solution)
     t1 ultimately finds the optimal solution in its local best and writes it to the sharedResults
     reduction happens, optimal solution is still found at the node t1 wrote to
     
     compare this to the potential performance penalty of locking on every sharedBound best check
    */
    if (t.get_num_cities() == num_cities) {
        best = t;
        unsigned int localBound = best.getMileage();
        unsigned int prevBound = *sharedBound;
        while ((prevBound > localBound) && (!(*sharedBound).compare_exchange_weak(prevBound, localBound)));
        //*sharedBound = t.getMileage();
    }
    else {
        for (unsigned int i=0; i < num_cities; i++) {
            if (t.add_city(i, best)) {
                DFS(t, best, sharedBound);
                t.remove_last();
            }
        }
    }
}

