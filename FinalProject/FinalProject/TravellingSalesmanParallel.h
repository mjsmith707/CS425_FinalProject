//
//  TravellingSalesmanParallel.h
//  FinalProject
//
//  Created by Max on 12/6/15.
//  Copyright (c) 2015 Matt. All rights reserved.
//

#ifndef FinalProject_TravellingSalesmanParallel_h
#define FinalProject_TravellingSalesmanParallel_h
#define BITS 100

#include <stack>
#include <vector>
#include <bitset>
#include "cities.h"

class TravellingSalesmanParallel {
    
public:
    struct TSPNode {
        // initial node, this ctor should only be called on start of the search
        TSPNode(int goalCity){
            this->goalCity = goalCity;
            this->containsCities.set(goalCity);
            this->path.push_back(goalCity);
        }
        
        // copy constructor (not necessary?)
        TSPNode(TSPNode& other){
            this->containsCities = other.containsCities;
            this->path = other.path;
            this->cumulativeCost = other.cumulativeCost;
            this->neighbors = other.neighbors;
            this->currentBound = other.currentBound;
        }
        
        // copy constructor which adds thisCity on to the path, should be called from neighbors gen
        TSPNode(TSPNode& other, size_t thisCity){
            this->containsCities = other.containsCities;
            this->path = other.path;
            this->cumulativeCost = other.cumulativeCost;
            this->neighbors = other.neighbors;
            this->currentBound = other.currentBound;
            
            this->currentCity = thisCity;
            this->containsCities.set(thisCity);
            this->path.push_back(thisCity);
        }
        
        std::vector<TSPNode*> neighbors;
        std::bitset<BITS> containsCities;
        std::vector<size_t> path;
        size_t cumulativeCost;
        size_t currentCity;
        std::atomic<size_t>* currentBound;
        size_t goalCity;
        
        bool equals(TSPNode& other) const;
        void generateNeighbors();
    };
    
    TSPNode runParallelSearch(int startCity);
    
private:
    
    void runSerialSearch(TSPNode* start);
    
    
    
};

#endif
