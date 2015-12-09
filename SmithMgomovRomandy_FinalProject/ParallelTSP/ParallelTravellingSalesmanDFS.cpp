/*
 * CS425 Final Project
 * Matt Smith, Reuben Romandy, Maksim Gomov
 * ParallelTravellingSalesmanDFS.cpp
 * 
 * Implementation for ParallelTravellingSalesman.h header, contains parallelization logic and
 * supporting functions.
 */

#include "ParallelTravellingSalesmanDFS.h"

// number of tasks per thread, directly affects task queue size
// larger number here should enable better workload distribution
#define N_TASKS_PER_THREAD 10

ParallelTravellingSalesmanDFS::Tour ParallelTravellingSalesmanDFS::runParallelDFS(unsigned int N_THREADS){
    ParallelTravellingSalesmanDFS::Tour firstNode;
    ParallelTravellingSalesmanDFS::Tour node;
    ParallelTravellingSalesmanDFS::Tour bfsbest(UINT_MAX);
    std::queue<ParallelTravellingSalesmanDFS::Tour> frontier;
    frontier.push(firstNode);
    
    // size that the task queue should reach
    size_t sizeOfTaskQueue = N_THREADS * N_TASKS_PER_THREAD;

    // run BFS until the frontier is empty, or the task queue is the right size
    while(frontier.size() > 0 && frontier.size() < sizeOfTaskQueue){
        node = (frontier.front());
        frontier.pop();
	if(node.get_num_cities() == num_cities && node.better_than(bfsbest)){
            bfsbest = node;
        }
        
        for(int i = 0; i < num_cities && frontier.size() < sizeOfTaskQueue; i++){
            if(!node.is_present(i)){
                ParallelTravellingSalesmanDFS::Tour neighbor = node;
                neighbor.add_city(i);
                frontier.push(neighbor);
            }
        }
    }

    // BFS already found the solution before it could fill the task queue, so just return that 
    if (bfsbest.getMileage() != UINT_MAX) {
        return bfsbest;
    }
    
    // this is the task queue, containing pairs of indices into the shared memory buffer and 
    // the task to be solved
    Shared_Queue<std::pair<unsigned int, ParallelTravellingSalesmanDFS::Tour>> taskQueue;
    
    // shared memory buffer for the results
    std::vector<ParallelTravellingSalesmanDFS::Tour> sharedResults;

    // pool of threads
    std::vector<std::thread> threadPool;

    // the shared bound for the B&B DFS implementation that each thread will run
    std::atomic<unsigned int> sharedBound(std::numeric_limits<unsigned int>::max());
    
    // shared memory buffer should be able to contain all elements in the task queue
    sharedResults.resize(sizeOfTaskQueue);
    
    // populate the task queue with all of the nodes that were in the frontier, along with an
    // index into the shared memory buffer
    for(unsigned int i = 0; i < sizeOfTaskQueue; i++){
        ParallelTravellingSalesmanDFS::Tour tmp(frontier.front());
        frontier.pop();
        std::pair<unsigned int, ParallelTravellingSalesmanDFS::Tour> pairForTaskQueue = std::make_pair(i, tmp);
        taskQueue.push(pairForTaskQueue);
    }

    // launch the threads    
    for(int i = 0; i < N_THREADS; i++){
        threadPool.push_back(std::thread(&ParallelTravellingSalesmanDFS::RunAgainstTaskQueue, this, &taskQueue, &sharedResults, &sharedBound));
    }
    
    // wait for all the threads to be done with the task queue
    for(int i = 0; i < N_THREADS; i++){
        threadPool[i].join();
    }
    
    // reduce the shared memory buffer into the final result
    ParallelTravellingSalesmanDFS::Tour solution(std::numeric_limits<unsigned int>::max());
    for(int i = 0; i < sharedResults.size(); i++){
        if(sharedResults[i].better_than(solution)){
            solution = sharedResults[i];
        }
    }
    
    return solution;
}

// Thread entry point
void ParallelTravellingSalesmanDFS::RunAgainstTaskQueue(Shared_Queue<std::pair<unsigned int, ParallelTravellingSalesmanDFS::Tour>>* taskQueue, std::vector<ParallelTravellingSalesmanDFS::Tour>* sharedResults, std::atomic<unsigned int>* sharedBound){
    while(!taskQueue->isEmpty()){
        // try-catch because there's a potential race between the isEmpty() and the pop
        try {
            std::pair<unsigned int, ParallelTravellingSalesmanDFS::Tour> currentTaskNode = taskQueue->pop();
            ParallelTravellingSalesmanDFS::Tour localTour = currentTaskNode.second;
            ParallelTravellingSalesmanDFS::Tour localBest(*sharedBound);
            ParallelTravellingSalesmanDFS::DFS(localTour, localBest, sharedBound);
            (*sharedResults)[currentTaskNode.first] = localBest;
            
            // Do an atomic compare and swap
            unsigned int localBound = localBest.getMileage();
            unsigned int prevBound = *sharedBound;
            while ((prevBound > localBound) && (!(*sharedBound).compare_exchange_weak(prevBound, localBound)));
        } catch(std::runtime_error e) {
            // nothing left in the task queue, so this thread can leave
            return;
        }
    }
}

// Recursive depth first search
void ParallelTravellingSalesmanDFS::DFS(ParallelTravellingSalesmanDFS::Tour& t, ParallelTravellingSalesmanDFS::Tour& best, std::atomic<unsigned int>* sharedBound) {
    if (!t.better_than(best, sharedBound)) return; // if we've already seen something better
    if (t.get_num_cities() == num_cities) {
        best = t;
        unsigned int localBound = best.getMileage();
        unsigned int prevBound = *sharedBound;
        while ((prevBound > localBound) && (!(*sharedBound).compare_exchange_weak(prevBound, localBound)));
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
