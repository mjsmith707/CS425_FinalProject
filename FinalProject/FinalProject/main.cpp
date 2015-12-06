//
//  main.cpp
//  FinalProject
//
//  Created by Matt on 11/27/15.
//  Copyright (c) 2015 Matt. All rights reserved.
//

#include <iostream>
#include "GraphSearch.h"
#include "ParallelSearch.h"
#include "TravellingSalesman.h"
#include "cities.h"

typedef std::chrono::high_resolution_clock::time_point timept_t;
typedef std::chrono::duration<double> timer_duration_t;

int main(int argc, const char * argv[]) {
    GraphSearch<TravellingSalesman> graphSearch;
    std::vector<unsigned int> previous;
    previous.push_back(0);
    TravellingSalesman start(0,0,0,previous);
    timept_t startTime, endTime;
    timer_duration_t duration;
    /*
    std::cout << "======UCS Result======" << std::endl;
    auto result = graphSearch.UniformSearch(start, start);
    std::cout << "Explored Size: " << graphSearch.getLastExploredCount() << std::endl;
    for (size_t i=0; i<result.size(); i++) {
        result.at(i).printNode();
    }
    */
    std::cout << std::endl << "======UCS BnB Result======" << std::endl;
    
    startTime = std::chrono::high_resolution_clock::now();
    auto result = graphSearch.UniformSearchBnB(start, start);
    endTime = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);
    std::cout << "Duration: " << duration.count() << ", Explored Size: " << graphSearch.getLastExploredCount() << std::endl;
    for (size_t i=0; i<result.size(); i++) {
        result.at(i).printNode();
    }
    /*
    std::cout << std::endl << "======A* Result======" << std::endl;
    result = graphSearch.AStarSearch(start, start);
    std::cout << "Explored Size: " << graphSearch.getLastExploredCount() << std::endl;
    for (size_t i=0; i<result.size(); i++) {
        result.at(i).printNode();
    }
    */
    std::cout << std::endl << "======Max's Parallel UCS B&B Result======" << std::endl;
    startTime = std::chrono::high_resolution_clock::now();
    result = graphSearch.ParallelUniformCostSearchBnB(start, start);
    endTime = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);
    std::cout << "Duration: " << duration.count() << ", Explored Size: " << graphSearch.getLastExploredCount() << std::endl;
    for (size_t i=0; i<result.size(); i++) {
        result.at(i).printNode();
    }
    
    // My impl
    ParallelSearch<TravellingSalesman> parallelSearch;
    std::cout << std::endl << "======Matt's Parallel UCS B&B Result======" << std::endl;
    startTime = std::chrono::high_resolution_clock::now();
    result = parallelSearch.ParallelGraphSearch(start, start, 4);
    endTime = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);
    std::cout << "Duration: " << duration.count() << ", Explored Size: " << graphSearch.getLastExploredCount() << std::endl;
    for (size_t i=0; i<result.size(); i++) {
        result.at(i).printNode();
    }
    
    return 0;
}
