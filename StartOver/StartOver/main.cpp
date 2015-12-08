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
#include "cities.h"

#define GOALCITY 0

typedef struct Node {
    unsigned int city;
    size_t cost;
    size_t previousCities;
    size_t previousCityCount;
    Node* previous;
} Node;

void dfs(Node* start);
std::vector<Node*> neighbors(Node* v, std::vector<Node*>& goalCities);

int main(int argc, const char * argv[]) {
    Node* start = new Node();
    start->city = 0;
    start->cost = 0;
    start->previousCities = 0;
    start->previousCities |= (0x1ul << 0);
    start->previousCityCount = 1;
    start->previous = nullptr;
    
    dfs(start);
    return 0;
}

void dfs(Node* start) {
    std::vector<Node*> goalCities;
    std::stack<Node*> S;
    S.push(start);
    size_t upperBound = std::numeric_limits<size_t>::max();
    Node* best = nullptr;
    
    while (!S.empty()) {
        Node* v = S.top();
        S.pop();
        
        if (v->cost > upperBound) {
            continue;
        }
        
        if ((v->city == GOALCITY) && (v->previousCityCount == num_cities+1)) {
            if (upperBound > v->cost) {
                upperBound = v->cost;
                best = v;
                std::cout << "New Upper Bound = " << upperBound << std::endl;
            }
        }
        
        std::vector<Node*> edges = neighbors(v, goalCities);
        for (size_t i=0; i<edges.size(); i++) {
            if (edges[i]->cost < upperBound) {
                S.push(edges[i]);
            }
        }
    }
    
    if ((best != nullptr) && (best->previous != nullptr)) {
        while (best != nullptr) {
            std::cout << "City: " << city_names[best->city] << ", Cost: " << best->cost << " Previous Cities: " << best->previousCityCount << std::endl;
            best = best->previous;
        }
    }
    else {
        std::cout << "No solution was found." << std::endl;
    }
}

std::vector<Node*> neighbors(Node* v, std::vector<Node*>& goalCities) {
    std::vector<Node*> candidates;
    if (v->previousCityCount > num_cities+1) {
        return candidates;
    }
    
    // Generate neighbor cities
    for (unsigned int i=0; i<num_cities; i++) {
        if (i != v->city) {
            if (!(v->previousCities & (0x1ul << i))) {
                Node* city = new Node();
                city->city = i;
                city->cost = v->cost + mileage_table[v->city][i];
                city->previousCities = v->previousCities;
                city->previousCities |= (0x1ul << i);
                city->previousCityCount = v->previousCityCount;
                city->previousCityCount++;
                city->previous = v;
                candidates.push_back(city);
            }
            else if ((i == GOALCITY) && (v->previousCityCount == num_cities)) {
                Node* city = new Node();
                city->city = i;
                city->cost = v->cost + mileage_table[v->city][i];
                city->previousCities = v->previousCities;
                city->previousCities |= (0x1ul << i);
                city->previousCityCount = v->previousCityCount;
                city->previousCityCount++;
                city->previous = v;
                candidates.push_back(city);
                goalCities.push_back(city);
            }
        }
    }
    return candidates;
}





















