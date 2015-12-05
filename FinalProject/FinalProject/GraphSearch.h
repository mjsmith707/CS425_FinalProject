//
//  GraphSearch.h
//
//  Created by Matt on 9/21/15.
//  Copyright (c) 2015 Matt. All rights reserved.
//

/* == Graph Search Super-template ==
 Has algorithms for BFS, DFS, Uniform Cost Search,
 UCS with Branch and Bound and A* Heuristic Search for
 any graph node class that can implement the necessary
 functions.
 */

/* Search Interface - Required T functions:
 // For copying T
 T(const T&);
 // Returns the cost of T
 size_t getCost() const;
 // Sets the cost of T
 void setCost(size_t);
 // Returns a shared_ptr to the previous node
 std::shared_ptr<T> getPrevious();
 // Sets a shared_ptr to the previous node
 void setPrevious(std::shared_ptr<T>&);
 // Returns a vector of shared_ptr of neighbors of T
 std::vector<std::shared_ptr<T>> neighbors();
 // Compares State equality for T with another T
 bool isStateEqual(std::shared_ptr<T>&);
 // Tests if T is a goal node, optionally with input T Goal
 bool isGoal(T&);
 // Computes a hash for T's state only (no cost)
 size_t hashState();
 
 // A* Additions
 // setHeuristic for node
 void setHeuristic(size_t);
 // getHeuristic cost for node
 size_t getHeuristic() const;
 */

#ifndef GraphSearch_h
#define GraphSearch_h
#include <vector>           // std::vector, size_t
#include <algorithm>        // make_heap, push_heap, pop_heap, find
#include <unordered_set>    // hash table
#include <memory>           // std::shared_ptr
#include <limits>           // std::numeric_limits<size_t>
#include <stack>            // std::stack
#include <queue>            // std::queue
#include <thread>           // std::thread
#include <atomic>           // std::atomic
#include <utility>          // std::pair
#include "SharedQueue.h"    // Shared_Queue (task queue)

// number of threads to run the parallel BFS with
#define N_THREADS 4

// number of tasks per thread minimum we're hoping to run
#define N_TASKS_PER_THREAD 2

// unordered_set hash override call to
// T's hashState()
template<class S>
class THash {
public:
    std::size_t operator()(std::shared_ptr<S> s) const {
        return s->hashState();
    }
};

// unordered_set equality override call to
// T's isStateEqual()
template<class R>
class TEqual {
public:
    bool operator()(std::shared_ptr<R> a, std::shared_ptr<R> b) const {
        return a->isStateEqual(b);
    }
};

// GraphSearch Template Class
template <class T>
class GraphSearch {
private:
    // std::heap cost comparison override call to
    // T's getCost()
    static bool costComparator(const std::shared_ptr<T> a, const std::shared_ptr<T> b) {
        return a->getCost() > b->getCost();
    }
    
    // std::heap cost comparison override call to
    // T's getCost() + T's getHeuristic()
    static bool heuristicComparator(const std::shared_ptr<T> a, const std::shared_ptr<T> b) {
        return (a->getCost() + a->getHeuristic()) > (b->getCost() + b->getHeuristic());
    }
    
    // Count of explored nodes for most recently ran search
    size_t exploredCount;
    std::atomic<size_t> exploredCountAtomic;
    
public:
    // Default Constructor
    GraphSearch() : exploredCount(0) {
    }
    
    // Getter for nodes explored
    size_t getLastExploredCount() {
        return exploredCount;
    }
    
    // Getter for nodes explored
    size_t getLastExploredCountAtomic() const {
        size_t tmp = exploredCountAtomic;
        return tmp;
    }
    
    // Standard Uniform Cost Search
    std::vector<T> UniformSearch(T start, T goal) {
        // Eventual solution
        std::vector<T> solution;
        
        // Copy start node
        std::shared_ptr<T> node(new T(start));
        node->setCost(0);
        
        // Initialize Data Structures
        std::vector<std::shared_ptr<T>> frontier;
        std::make_heap(frontier.begin(), frontier.end(), costComparator);
        std::unordered_set<std::shared_ptr<T>, THash<T>, TEqual<T>> explored;
        exploredCount = 0;
        
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
                break;
            }
            
            // Find neighbors
            std::vector<std::shared_ptr<T>> candidates = node->neighbors();
            for (auto& n : candidates) {
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
        
        // node is the solution
        // Find path
        while (node->getPrevious() != nullptr) {
            T temp = (*node);
            solution.insert(solution.begin(), temp);
            node = node->getPrevious();
        }
        solution.insert(solution.begin(), start);
        
        exploredCount = explored.size();
        return solution;
    }
    
    // Uniform Cost Search with Branch and Bound
    std::vector<T> UniformSearchBnB(T start, T goal) {
        // Eventual solution
        std::vector<T> solution;
        std::shared_ptr<T> best;
        
        // Copy start node
        std::shared_ptr<T> node(new T(start));
        node->setCost(0);
        
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
        
        // best is the solution
        // Find path
        while (best->getPrevious() != nullptr) {
            T temp = (*best);
            solution.insert(solution.begin(), temp);
            best = best->getPrevious();
        }
        solution.insert(solution.begin(), start);
        
        //exploredCount = explored.size();
        return solution;
    }
    
    // A* Heuristic Search
    std::vector<T> AStarSearch(T start, T goal) {
        // Eventual solution
        std::vector<T> solution;
        std::shared_ptr<T> best;
        
        // Copy start node
        std::shared_ptr<T> node(new T(start));
        node->setCost(0);
        
        // Initialize Data Structures
        std::vector<std::shared_ptr<T>> frontier;
        std::make_heap(frontier.begin(), frontier.end(), heuristicComparator);
        std::unordered_set<std::shared_ptr<T>, THash<T>, TEqual<T>> explored;
        exploredCount = 0;
        size_t upperBound = std::numeric_limits<size_t>::max();
        
        // Enqueue start
        frontier.push_back(node);
        std::push_heap(frontier.begin(), frontier.end(), heuristicComparator);
        
        for (;;) {
            if (frontier.empty()) {
                throw std::runtime_error("No solution was found.");
            }
            
            // Dequeue top node from frontier
            node = frontier.front();
            std::pop_heap(frontier.begin(), frontier.end(), heuristicComparator);
            frontier.pop_back();
            
            // Place in explored
            explored.emplace(node);
            
            // Is this the goal state?
            if (node->isGoal(goal)) {
                best = node;
                upperBound = node->getCost() + node->getHeuristic();
                break;
            }
            
            // Is this above the upper bound?
            if ((node->getCost() + node->getHeuristic()) >= upperBound) {
                continue;
            }
            
            // Find neighbors
            std::vector<std::shared_ptr<T>> candidates = node->neighbors();
            for (auto& n : candidates) {
                // Skip if >= upperBound
                if ((n->getCost() + n->getHeuristic() + node->getCost() + node->getHeuristic()) >= upperBound) {
                    continue;
                }
                // If its the goal and lower, update best
                if (n->isGoal(goal)) {
                    best = n;
                    upperBound = n->getCost() + n->getHeuristic() + node->getCost() + node->getHeuristic();
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
                        std::push_heap(frontier.begin(), frontier.end(), heuristicComparator);
                    }
                }
            }
        }
        
        // best is the solution
        // Find path
        while (best->getPrevious() != nullptr) {
            T temp = (*best);
            solution.insert(solution.begin(), temp);
            best = best->getPrevious();
        }
        solution.insert(solution.begin(), start);
        
        exploredCount = explored.size();
        return solution;
    }
    
    // Breadth First Search
    std::vector<T> BreadthFirstSearch(T start, T goal) {
        // Eventual solution
        std::vector<T> solution;
        std::shared_ptr<T> lastNode = nullptr;
        
        // Copy start node
        std::shared_ptr<T> node(new T(start));
        node->setCost(0);
        
        // Initialize Data Structures
        std::queue<std::shared_ptr<T>> frontier;
        std::unordered_set<std::shared_ptr<T>, THash<T>, TEqual<T>> explored;
        exploredCount = 0;
        
        size_t upperBound = std::numeric_limits<size_t>::max();
        
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
            
            // Check if goal and save the pointer if it is
            if (node->isGoal(goal) && node->getCost() < upperBound) {
                lastNode = node;
                upperBound = node->getCost();
            }
            
            // Find neighbors
            std::vector<std::shared_ptr<T>> candidates = node->neighbors();
            for (auto& n : candidates) {
                // Check if already explored
                if (explored.find(n) == explored.end()) {
                    n->setCost(n->getCost() + node->getCost());
                    n->setPrevious(node);
                    
                    // Place in explored
                    explored.emplace(n);
                    
                    // Check if goal and save the pointer if it is
                    
                    //if (n->isGoal(goal)) {
                    //    lastNode = n;
                    //}
                    
                    // Place in queue
                    frontier.push(n);
                }
            }
        }
        
        if (lastNode == nullptr) {
            throw std::runtime_error("No solution was found.");
        }
        
        // Find path
        while (lastNode->getPrevious() != nullptr) {
            T temp = (*lastNode);
            solution.insert(solution.begin(), temp);
            lastNode = lastNode->getPrevious();
        }
        solution.insert(solution.begin(), start);
        
        exploredCount = explored.size();
        return solution;
    }
    
    // Depth First Search
    std::vector<T> DepthFirstSearch(T start, T goal) {
        // Eventual solution
        std::vector<T> solution;
        std::shared_ptr<T> lastNode = nullptr;
        
        // Copy start node
        std::shared_ptr<T> node(new T(start));
        node->setCost(0);
        
        // Initialize Data Structures
        std::stack<std::shared_ptr<T>> frontier;
        std::unordered_set<std::shared_ptr<T>, THash<T>, TEqual<T>> explored;
        exploredCount = 0;
        
        frontier.push(node);
        
        for (;;) {
            if (frontier.empty()) {
                break;
            }
            
            // Dequeue top node from frontier
            node = frontier.top();
            frontier.pop();
            
            // Check if visited
            if (explored.find(node) == explored.end()) {
                // Mark as visited
                explored.emplace(node);
                
                // Check if goal and save the pointer if it is
                if (node->isGoal(goal)) {
                    lastNode = node;
                    break;
                }
                
                // Find neighbors
                std::vector<std::shared_ptr<T>> candidates = node->neighbors();
                for (auto& n : candidates) {
                    n->setCost(n->getCost() + node->getCost());
                    n->setPrevious(node);
                    
                    // Place in queue
                    frontier.push(n);
                }
            }
        }
        
        if (lastNode == nullptr) {
            throw std::runtime_error("No solution was found.");
        }
        
        // Find path
        while (lastNode->getPrevious() != nullptr) {
            T temp = (*lastNode);
            solution.insert(solution.begin(), temp);
            lastNode = lastNode->getPrevious();
        }
        solution.insert(solution.begin(), start);
        
        exploredCount = explored.size();
        return solution;
    }
    
    std::vector<T> ParallelUniformCostSearchBnB(T start, T goal){
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
    
    void RunTasks(Shared_Queue<std::pair<std::shared_ptr<T>, size_t>>* taskQueue, T goal, std::vector<std::shared_ptr<T>>* sharedResults){
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
    
};

#endif