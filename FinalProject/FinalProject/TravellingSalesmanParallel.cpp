//
//  TravellingSalesmanParallel.cpp
//  FinalProject
//
//  Created by Max on 12/6/15.
//  Copyright (c) 2015 Matt. All rights reserved.
//

#include <stdio.h>
#include "TravellingSalesmanParallel.h"

bool TravellingSalesmanParallel::TSPNode::equals(TravellingSalesmanParallel::TSPNode &other) const {
    if(this->cumulativeCost != other.cumulativeCost){
        return false;
    }
    
    if(this->containsCities != other.containsCities){
        return false;
    }
    
    if(this->path.size() != other.path.size()){
        return false;
    }

    for(int i = 0; i < this->path.size(); i++){
        if(this->path[i] != other.path[i]){
            return false;
        }
    }
    
    return true;
}

void TravellingSalesmanParallel::TSPNode::generateNeighbors(){
    
    if(neighbors.size() != 0){
        throw std::runtime_error("Generated neighbors twice for a node");
    }

    // this node leads to a goal state (last city), so add that to the path
    if(path.size() == num_cities - 1){
        TravellingSalesmanParallel::TSPNode tmp(*this, goalCity);
        neighbors.push_back(&tmp);
    }
    
    size_t theCurrentBound = *currentBound;
    for(int i = 0; i < num_cities; i++){
        if(!containsCities[i]){
            if(theCurrentBound > cumulativeCost + mileage_table[currentCity][i]){
                TravellingSalesmanParallel::TSPNode tmp(*this, i);
                neighbors.push_back(&tmp);
            }
        }
    }
    
    /*
    for (unsigned int i=0; i<num_cities; i++) {
        if (i != city) {
            auto itr = std::find(previousCities.begin(), previousCities.end(), i);
            if (itr == previousCities.end()) {
                std::vector<unsigned int> prevcity = previousCities;
                prevcity.push_back(i);
                std::shared_ptr<TravellingSalesman> newCity(new TravellingSalesman(i, city, goalCity, prevcity));
                candidates.push_back(newCity);
            }
            else if ((i == goalCity) && (previousCities.size() == num_cities)) {
                std::vector<unsigned int> prevcity = previousCities;
                prevcity.push_back(i);
                std::shared_ptr<TravellingSalesman> newCity(new TravellingSalesman(i, city, goalCity, prevcity));
                candidates.push_back(newCity);
            }
        }
    }
    */
}

/*
TravellingSalesmanParallel::TSPNode TravellingSalesmanParallel::runParallelSearch(int startCity) {
    std::stack<TravellingSalesmanParallel::TSPNode> queue;
    TravellingSalesmanParallel::TSPNode node;
    return node;
}
*/

void TravellingSalesmanParallel::runSerialSearch(TravellingSalesmanParallel::TSPNode* initial) {
    std::stack<TravellingSalesmanParallel::TSPNode> queue;
    
}

/*
std::vector<T> TravellingSalesmanParallel::ParallelUniformCostSearchBnB(T start, T goal){
    // Eventual solution
    std::vector<T> solution;
    std::shared_ptr<T> lastNode = nullptr;
    
    // Copy start node
    std::shared_ptr<T> node(new T(start));
    node->setCost(0);
    
    // Initialize Data Structures
    std::queue<std::shared_ptr<T>> frontier;
    std::unordered_set<std::shared_ptr<T>, THash<T>, TEqual<T>> explored;
    
    // threads that get spawned
    std::vector<std::thread*> threads;
    
    // shared memory for the threads to throw results into
    std::vector<std::shared_ptr<T>>* results = new std::vector<std::shared_ptr<T>>();
    
    // size to take the task queue to
    size_t taskQueueSize = N_THREADS * N_TASKS_PER_THREAD;
    
    exploredCountAtomic = 0;
    exploredCount = 0;
    
    frontier.push(node);
    
    for (;;) {
        if (frontier.empty()) {
            break;
        }
        
        // Dequeue top node from frontier
        node = frontier.front();
        frontier.pop();
        
        // Mark as visited
        explored.emplace(node);
        
        // Find neighbors
        std::vector<std::shared_ptr<T>> candidates = node->neighbors();
        for (auto& n : candidates) {
            // Check if already explored
            if (explored.find(n) == explored.end()) {
                n->setCost(n->getCost() + node->getCost());
                n->setPrevious(node);
                
                // Place in explored
                explored.emplace(n);
                // Place in queue
                frontier.push(n);
            }
        }
        
        // if this is true, fulfilled our predefined criteria for the task queue size, so stop the BFS here
        if(frontier.size() + candidates.size() > taskQueueSize){
            break;
        }
    }
    
    // how much work is in the task queue
    size_t actualTaskQueueSize = frontier.size();
    
    // to my understanding, don't need to have an index for each completed task in the shared results vector,
    // and instead just have one slot per thread; since the result of the task is a final cost, on completion of
    // any additional assigned tasks, the thread compares the just-completed cost with the existing cost
    // and replaces if the just-completed cost is lower than existing cost
    
    // the size_t of the pair will tell each thread where to put the completed task result in the sharedResults vector
    Shared_Queue<std::pair<std::shared_ptr<T>, size_t>>* taskQueue = new Shared_Queue<std::pair<std::shared_ptr<T>, size_t>>();
    
    // this is our shared memory that we will coalesce results into, and then reduce from
    results->resize(actualTaskQueueSize);
    
    // add the whole frontier into the task queue
    size_t resultsIdx = 0;
    while(frontier.size() != 0){
        std::shared_ptr<T> temp(frontier.front());
        std::pair<std::shared_ptr<T>, size_t> taskPair(temp, resultsIdx++);
        taskQueue->push(taskPair);
        frontier.pop();
    }
    
    // spawn worker threads and start them on the task queue
    for(size_t i = 0; i < actualTaskQueueSize; i++){
        //std::thread* UCSThread = new std::thread(&GraphSearch::ForParallelUniformSearchBnB, this, temp, start, i, results);
        std::thread* UCSThread = new std::thread(&GraphSearch::RunTasks, this, taskQueue, goal, results);
        threads.push_back(UCSThread);
    }
    
    // threads should be done with the task queue here
    for(int i = 0; i < threads.size(); i++){
        threads[i]->join();
    }
    
    std::shared_ptr<T> bestNode;
    bestNode = results->front();
    
    // simple reduction, finding the lowest-cost node
    for(size_t i = 1; i < results->size(); i++){
        if((*results)[i]->getCost() < bestNode->getCost()){
            bestNode = (*results)[i];
        }
    }
    
    // find the whole path
    while (bestNode->getPrevious() != nullptr) {
        T temp = (*bestNode);
        solution.insert(solution.begin(), temp);
        bestNode = bestNode->getPrevious();
    }
    solution.insert(solution.begin(), start);
    
    return solution;
}

void TravellingSalesmanParallel::RunTasks(Shared_Queue<std::pair<std::shared_ptr<T>, size_t>>* taskQueue, T goal, std::vector<std::shared_ptr<T>>* sharedResults){
    while(!taskQueue->isEmpty()){
        
        // try-catch because there's a potential race between the isEmpty() timing and
        try{
            std::pair<std::shared_ptr<T>, size_t> currentTaskNode = taskQueue->pop();
            
            // run the ucs search on the task this thread picked up from the task queue
            ForParallelUniformSearchBnB(*(currentTaskNode.first),goal, currentTaskNode.second, sharedResults);
        } catch(std::runtime_error e){
            
            // nothing left in the task queue, so this thread can leave
            return;
        }
    }
}

// Uniform Cost Search with Branch and Bound, to be called by individual threads which will put result into a shared memory array
void ForParallelUniformSearchBnB(T start, T goal, size_t sharedResultIdx, std::vector<std::shared_ptr<T>>* sharedResults) {
    // Eventual solution
    std::vector<T> solution;
    std::shared_ptr<T> best;
    
    // Copy start node (don't reset anything)
    std::shared_ptr<T> node(new T(start));
    
    // Initialize Data Structures
    std::vector<std::shared_ptr<T>> frontier;
    std::make_heap(frontier.begin(), frontier.end(), costComparator);
    std::unordered_set<std::shared_ptr<T>, THash<T>, TEqual<T>> explored;
    //exploredCount = 0;
    size_t upperBound = std::numeric_limits<size_t>::max();
    
    // Enqueue start
    frontier.push_back(node);
    std::push_heap(frontier.begin(), frontier.end(), costComparator);
    
    for (;;) {
        if (frontier.empty()) {
            throw std::runtime_error("No solution was found.");
        }
        
        // Dequeue top node from frontier
        node = frontier.front();
        std::pop_heap(frontier.begin(), frontier.end(), costComparator);
        frontier.pop_back();
        
        // Place in explored
        explored.emplace(node);
        
        // Is this the goal state?
        if (node->isGoal(goal)) {
            best = node;
            upperBound = node->getCost();
            break;
        }
        
        // Is this above the upper bound?
        if (node->getCost() >= upperBound) {
            continue;
        }
        
        // Find neighbors
        std::vector<std::shared_ptr<T>> candidates = node->neighbors();
        for (auto& n : candidates) {
            // Skip if >= upperBound
            if ((n->getCost() + node->getCost()) >= upperBound) {
                continue;
            }
            // If its the goal and lower, update best
            if (n->isGoal(goal)) {
                best = n;
                upperBound = n->getCost() + node->getCost();
            }
            // Update previous pointer
            n->setPrevious(node);
            // Check if already explored
            if (explored.find(n) == explored.end()) {
                // Check if already in frontier
                auto it = std::find(frontier.begin(), frontier.end(), n);
                if (it == frontier.end()) {
                    // Add to frontier
                    n->setCost(n->getCost() + node->getCost());
                    n->setPrevious(node);
                    frontier.push_back(n);
                    std::push_heap(frontier.begin(), frontier.end(), costComparator);
                }
            }
        }
    }
    
    exploredCountAtomic += explored.size();
    
    // throw the best result into the shared array
    // the reduction happens in thread0 after all is done anyways, so don't bother generating the whole path list here
    (*sharedResults)[sharedResultIdx] = best;
}

*/