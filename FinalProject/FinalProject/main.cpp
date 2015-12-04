//
//  main.cpp
//  FinalProject
//
//  Created by Matt on 11/27/15.
//  Copyright (c) 2015 Matt. All rights reserved.
//

#include <iostream>
#include "GraphSearch.h"
#include "TravellingSalesman.h"
#include "cities.h"

int main(int argc, const char * argv[]) {
    GraphSearch<TravellingSalesman> graphSearch;
    std::vector<unsigned int> previous;
    previous.push_back(0);
    TravellingSalesman start(0,0,0,previous);
    
    std::cout << "======UCS Result======" << std::endl;
    auto result = graphSearch.UniformSearch(start, start);
    std::cout << "Explored Size: " << graphSearch.getLastExploredCount() << std::endl;
    for (size_t i=0; i<result.size(); i++) {
        result.at(i).printNode();
    }
    
    std::cout << std::endl << "======UCS BnB Result======" << std::endl;
    result = graphSearch.UniformSearchBnB(start, start);
    std::cout << "Explored Size: " << graphSearch.getLastExploredCount() << std::endl;
    for (size_t i=0; i<result.size(); i++) {
        result.at(i).printNode();
    }
    
    std::cout << std::endl << "======A* Result======" << std::endl;
    result = graphSearch.AStarSearch(start, start);
    std::cout << "Explored Size: " << graphSearch.getLastExploredCount() << std::endl;
    for (size_t i=0; i<result.size(); i++) {
        result.at(i).printNode();
    }
    
    std::cout << std::endl << "======BFS Result======" << std::endl;
    result = graphSearch.BreadthFirstSearch(start, start);
    std::cout << "Explored Size: " << graphSearch.getLastExploredCount() << std::endl;
    for (size_t i=0; i<result.size(); i++) {
        result.at(i).printNode();
    }
    
    std::cout << std::endl << "======DFS Result======" << std::endl;
    result = graphSearch.DepthFirstSearch(start, start);
    std::cout << "Explored Size: " << graphSearch.getLastExploredCount() << std::endl;
    for (size_t i=0; i<result.size(); i++) {
        result.at(i).printNode();
    }
    
    std::cout << std::endl << "======Parallel UCS B&B Result======" << std::endl;
    result = graphSearch.ParallelUniformCostSearchBnB(start, start);
    std::cout << "Explored Size: " << graphSearch.getLastExploredCount() << std::endl;
    for (size_t i=0; i<result.size(); i++) {
        result.at(i).printNode();
    }
    
    return 0;
}
