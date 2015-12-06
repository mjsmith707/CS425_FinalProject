//
//  ParallelSearch.h
//  FinalProject
//
//  Created by Matt on 12/5/15.
//  Copyright (c) 2015 Matt. All rights reserved.
//

#ifndef FinalProject_ParallelSearch_h
#define FinalProject_ParallelSearch_h

#include <vector>           // std::vector, size_t
#include <algorithm>        // make_heap, push_heap, pop_heap, find
#include <unordered_set>    // hash table
#include <memory>           // std::shared_ptr
#include <limits>           // std::numeric_limits<size_t>
#include <stack>            // std::stack
#include <queue>            // std::queue
#include <thread>           // std::thread
#include <future>           // std::future
#include <atomic>           // std::atomic
#include <iostream>

// unordered_set hash override call to
// T's hashState()
template<class S>
class PTHash {
public:
    std::size_t operator()(std::shared_ptr<S> s) const {
        return s->hashState();
    }
};

// unordered_set equality override call to
// T's isStateEqual()
template<class R>
class PTEqual {
public:
    bool operator()(std::shared_ptr<R> a, std::shared_ptr<R> b) const {
        return a->isStateEqual(b);
    }
};

// GraphSearch Template Class
template <class T>
class ParallelSearch {
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
    
    // Uniform Cost Search with Branch and Bound
    std::vector<T> UniformSearchBnB(std::shared_ptr<T> start, T goal, std::atomic<size_t>* upperBound) {
        // Eventual solution
        std::vector<T> solution;
        std::shared_ptr<T> best;
        
        // Copy start node
        std::shared_ptr<T> node(new T((*start)));
        
        // Initialize Data Structures
        std::vector<std::shared_ptr<T>> frontier;
        std::make_heap(frontier.begin(), frontier.end(), costComparator);
        std::unordered_set<std::shared_ptr<T>, PTHash<T>, PTEqual<T>> explored;
        //exploredCount = 0;
        
        
        // Enqueue start
        frontier.push_back(node);
        std::push_heap(frontier.begin(), frontier.end(), costComparator);
        
        for (;;) {
            if (frontier.empty()) {
                return solution;
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
                (*upperBound) = node->getCost();
                std::cout << "New Upper Bound = " << (*upperBound) << std::endl;
                break;
            }
            
            // Is this above the upper bound?
            if (node->getCost() >= (*upperBound)) {
                continue;
            }
            
            // Find neighbors
            std::vector<std::shared_ptr<T>> candidates = node->neighbors();
            for (auto& n : candidates) {
                // Skip if >= upperBound
                if ((n->getCost() + node->getCost()) >= (*upperBound)) {
                    continue;
                }
                // If its the goal and lower, update best
                if (n->isGoal(goal)) {
                    best = n;
                    (*upperBound) = n->getCost() + node->getCost();
                    std::cout << "New Upper Bound = " << (*upperBound) << std::endl;
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
        return solution;
    }
    
    // Breadth First Search
    // Runs BFS to a specified depth and returns nodes in the frontier
    std::vector<T> BreadthFirstSearch(T start, T goal, std::queue<std::shared_ptr<T>>& frontier, std::unordered_set<std::shared_ptr<T>, PTHash<T>, PTEqual<T>>& explored, size_t depth) {
        // Eventual solution
        std::vector<T> solution;
        std::shared_ptr<T> lastNode = nullptr;
        
        // Copy start node
        std::shared_ptr<T> node(new T(start));
        node->setCost(0);
        
        // Initialize Data Structures
        exploredCount = 0;
        
        size_t upperBound = std::numeric_limits<size_t>::max();
        
        frontier.push(node);
        
        for (;;) {
            if (frontier.empty() || frontier.size() >= depth) {
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
                    if (n->isGoal(goal)) {
                        lastNode = n;
                    }
                    
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
    std::vector<T> DepthFirstSearch(std::shared_ptr<T> start, T goal, std::atomic<size_t>* upperBound) {
        // Eventual solution
        std::vector<T> solution;
        std::shared_ptr<T> lastNode = nullptr;
        
        // Copy start node
        std::shared_ptr<T> node(new T((*start)));
        
        // Initialize Data Structures
        std::stack<std::shared_ptr<T>> frontier;
        std::unordered_set<unsigned int> explored;
        
        frontier.push(node);
        
        for (;;) {
            if (frontier.empty()) {
                break;
            }
            
            // Dequeue top node from frontier
            node = frontier.top();
            frontier.pop();
            
            // Is this above the upper bound?
            if (node->getCost() >= (*upperBound)) {
                continue;
            }
            
            // Check if goal and save the pointer if it is
            if (node->isGoal(goal) && node->getCost() < (*upperBound)) {
                lastNode = node;
                (*upperBound) = node->getCost();
                std::cout << "New Upper Bound = " << (*upperBound) << std::endl;
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
        
        if (lastNode == nullptr) {
            throw std::runtime_error("No solution was found.");
        }
        
        // Find path
        while (lastNode->getPrevious() != nullptr) {
            T temp = (*lastNode);
            solution.insert(solution.begin(), temp);
            lastNode = lastNode->getPrevious();
        }
        
        exploredCount = explored.size();
        return solution;
    }
    
public:
    // Default Constructor
    ParallelSearch() : exploredCount(0) {
    }
    
    // Getter for nodes explored
    size_t getLastExploredCount() {
        return exploredCount;
    }
    
    // Parallel Graph Search
    std::vector<T> ParallelGraphSearch(T start, T goal, size_t numThreads) {
        // Initialize BFS structures
        std::queue<std::shared_ptr<T>> frontier;
        std::unordered_set<std::shared_ptr<T>, PTHash<T>, PTEqual<T>> explored;
        try {
            // Run BFS
            std::vector<T> solution = BreadthFirstSearch(start, goal, frontier, explored, numThreads);
            return solution;
        }
        catch (std::runtime_error& err) {
            // No solution was found so run UCS in parallel speculatively
            T temp(start);
            temp.setCost(std::numeric_limits<size_t>::max());
            std::vector<T> lowestSolution;
            lowestSolution.push_back(temp);
            std::atomic<size_t> upperBound(std::numeric_limits<size_t>::max());
            while (frontier.size() > 0) {
                std::vector<std::future<std::vector<T>>> results;
                size_t threadsSpawned = 0;
                // Spawn threads
                for (threadsSpawned=0; (threadsSpawned<numThreads) && (threadsSpawned<frontier.size()); threadsSpawned++) {
                    auto startNode = frontier.front();
                    frontier.pop();
                    results.push_back(std::async(std::launch::async, &ParallelSearch::DepthFirstSearch, this, startNode, goal, &upperBound));
                }
                // Wait for results
                for (auto& i : results) {
                    i.wait();
                }
                // Find the best solution
                for (size_t i=0; i<results.size(); i++) {
                    // Get result
                    try {
                        auto futureRes = results.at(i).get();
                        if (futureRes.size() > 0) {
                            if (futureRes.at(futureRes.size()-1).getCost() < lowestSolution.at(lowestSolution.size()-1).getCost()) {
                                lowestSolution = futureRes;
                            }
                        }
                    }
                    catch (std::runtime_error& e) {
                        // No result
                    }
                }
                
            }
            // Check if solution was actually found
            if (lowestSolution[0].getCost() == std::numeric_limits<size_t>::max()) {
                throw std::runtime_error("No solution was found.");
            }
            else {
                lowestSolution.insert(lowestSolution.begin(), start);
                return lowestSolution;
            }
        }
    }
    
};

#endif
