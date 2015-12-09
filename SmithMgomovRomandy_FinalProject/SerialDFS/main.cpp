//
//  main.cpp
//  StartOver
//
//  Created by Matt on 12/6/15.
//  Copyright (c) 2015 Matt. All rights reserved.
//

#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
#include <memory>
#include <chrono>
#include "cities.h"

#define GOALCITY 0

// #undef to enable csv output
#define VERBOSE

typedef struct Node {
    unsigned int city;
    size_t cost;
    size_t previousCities;
    std::shared_ptr<Node> previous;
} Node;

typedef std::chrono::high_resolution_clock::time_point timept_t;
typedef std::chrono::duration<double> timer_duration_t;

void dfs(std::shared_ptr<Node> start);
std::vector<std::shared_ptr<Node>> neighbors(std::shared_ptr<Node> v, size_t& fullCities);

int main(int argc, const char * argv[]) {
    std::shared_ptr<Node> start(new Node());
    start->city = 0;
    start->cost = 0;
    start->previousCities = 0;
    start->previousCities |= (0x1ul << 0);
    start->previous = nullptr;
    timept_t startTime, endTime;
    timer_duration_t duration;
    startTime = std::chrono::high_resolution_clock::now();
    dfs(start);
    endTime = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);
#ifndef VERBOSE
    std::cout << "2,1," << num_cities << "," << duration.count() << std::endl;
#endif
    return 0;
}

void dfs(std::shared_ptr<Node> start) {
    std::stack<std::shared_ptr<Node>> S;
    S.push(start);
    size_t upperBound = std::numeric_limits<size_t>::max();
    std::shared_ptr<Node> best(nullptr);

    size_t tmp = 0;
    for(int i = 0; i < num_cities; i++){
        tmp |= (0x1ul << i);
    }
    size_t fullCities = 0;
    fullCities |= tmp;
    
    while (!S.empty()) {
        std::shared_ptr<Node> v = S.top();
        S.pop();
        
        if (v->cost > upperBound) {
            continue;
        }
        
        if ((v->city == GOALCITY) && ((v->previousCities & fullCities) == fullCities)) {
            if (upperBound > v->cost) {
                upperBound = v->cost;
                best = v;
            }
        }
        
        std::vector<std::shared_ptr<Node>> edges = neighbors(v, fullCities);
        for (size_t i=0; i<edges.size(); i++) {
            if (edges[i]->cost < upperBound) {
                S.push(edges[i]);
            }
        }
    }
#ifdef VERBOSE
    std::cout << "num_cities: " << num_cities << std::endl;
    if ((best != nullptr) && (best->previous != nullptr)) {
        while (best != nullptr) {
            std::cout << "City: " << city_names[best->city] << ", Cost: " << best->cost << std::endl;
            best = best->previous;
        }
    }
    else {
        std::cout << "No solution was found." << std::endl;
    }
#endif
}

std::vector<std::shared_ptr<Node>> neighbors(std::shared_ptr<Node> v, size_t& fullCities ) {
    std::vector<std::shared_ptr<Node>> candidates;
    candidates.reserve(num_cities + 1);
    
    // Generate neighbor cities
    for (unsigned int i=0; i<num_cities; i++) {
        if (i != v->city) {
            if (!(v->previousCities & (0x1ul << i))) {
                std::shared_ptr<Node> city(new Node());
                city->city = i;
                city->cost = v->cost + mileage_table[v->city][i];
                city->previousCities = v->previousCities;
                city->previousCities |= (0x1ul << i);
                city->previous = v;
                candidates.push_back(city);
            }
            else if (((v->previousCities & fullCities) == fullCities) && (i == GOALCITY)) {
                std::shared_ptr<Node> city(new Node());
                city->city = i;
                city->cost = v->cost + mileage_table[v->city][i];
                city->previousCities = v->previousCities;
                city->previousCities |= (0x1ul << i);
                city->previous = v;
                candidates.push_back(city);
            }
        }
    }
    return candidates;
}





















