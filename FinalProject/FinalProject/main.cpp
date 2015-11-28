//
//  main.cpp
//  FinalProject
//
//  Created by Matt on 11/27/15.
//  Copyright (c) 2015 Matt. All rights reserved.
//

#include <iostream>
#include "UniformCostSearch.h"
#include "City.h"
#include "cities.h"

int main(int argc, const char * argv[]) {
    UniformCostSearch<City> ucsSearch;
    std::vector<unsigned int> previous;
    previous.push_back(0);
    City start(0,0,0,previous);
    
    auto result = ucsSearch.UniformSearchBnB(start, start);
    for (size_t i=0; i<result.size(); i++) {
        if (i==0) {
            result.at(0).getPrevious()->printNode();
        }
        result.at(i).printNode();
    }
    
    return 0;
}
