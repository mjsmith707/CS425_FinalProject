//
//  City.h
//  FinalProject
//
//  Created by Matt on 11/27/15.
//  Copyright (c) 2015 Matt. All rights reserved.
//

#ifndef __FinalProject__TravellingSalesman__
#define __FinalProject__TravellingSalesman__

#include "cities.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <algorithm>

// Travelling Salesman Problem Implementation for GraphSearch.h
// No heuristic added yet.
class TravellingSalesman {
    private:
        // The node's state
        unsigned int city;
        std::vector<unsigned int> previousCities;
    
        // The start/end city
        unsigned int goalCity;
        
        // Node cost
        size_t cost;
    
        // Heuristic cost
        size_t heuristic;
    
        // Previous node pointer
        std::shared_ptr<TravellingSalesman> previous;
    
    public:
        // Default Constructor
        TravellingSalesman(unsigned int cityID, unsigned int previousID, unsigned int goalCity, std::vector<unsigned int> previousCities);
        // For copying T
        TravellingSalesman(const TravellingSalesman&);
        // Returns the cost of T
        size_t getCost() const;
        // Sets the cost of T
        void setCost(size_t);
        // Returns a shared_ptr to the previous node
        std::shared_ptr<TravellingSalesman> getPrevious();
        // Sets a shared_ptr to the previous node
        void setPrevious(std::shared_ptr<TravellingSalesman>&);
        // Returns a vector of shared_ptr of neighbors of T
        std::vector<std::shared_ptr<TravellingSalesman>> neighbors();
        // Compares State equality for T with another T
        bool isStateEqual(std::shared_ptr<TravellingSalesman>&);
        // Tests if T is a goal node, optionally with input T Goal
        bool isGoal(TravellingSalesman&);
        // Computes a hash for T's state only (no cost)
        size_t hashState();
        // setHeuristic for node
        void setHeuristic(size_t);
        // getHeuristic cost for node
        size_t getHeuristic() const;
    
        // Print the node out
        void printNode();
};

#endif /* defined(__FinalProject__TravellingSalesman__) */
