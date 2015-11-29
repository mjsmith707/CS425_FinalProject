//
//  City.cpp
//  FinalProject
//
//  Created by Matt on 11/27/15.
//  Copyright (c) 2015 Matt. All rights reserved.
//

#include "TravellingSalesman.h"

TravellingSalesman::TravellingSalesman(unsigned int newcityID, unsigned int previousID, unsigned int newgoalCity, std::vector<unsigned int> newpreviousCities)
: city(newcityID), cost(mileage_table[previousID][newcityID]), heuristic(0), previous(nullptr), goalCity(newgoalCity), previousCities(newpreviousCities) {
}

// For copying T
TravellingSalesman::TravellingSalesman(const TravellingSalesman& other) {
    this->city = other.city;
    this->cost = other.cost;
    this->heuristic = other.heuristic;
    this->previous = other.previous;
    this->goalCity = other.goalCity;
    this->previousCities = other.previousCities;
}

// Returns the cost of T
size_t TravellingSalesman::getCost() const {
    return this->cost;
}

// Sets the cost of T
void TravellingSalesman::setCost(size_t newcost) {
    this->cost = newcost;
}

// Returns a shared_ptr to the previous node
std::shared_ptr<TravellingSalesman> TravellingSalesman::getPrevious() {
    return this->previous;
}

// Sets a shared_ptr to the previous node
void TravellingSalesman::setPrevious(std::shared_ptr<TravellingSalesman>& newprevious) {
    this->previous = newprevious;
}

// Returns a vector of shared_ptr of neighbors of T
std::vector<std::shared_ptr<TravellingSalesman>> TravellingSalesman::neighbors() {
    std::vector<std::shared_ptr<TravellingSalesman>> candidates;
    if (previousCities.size() > num_cities+1) {
        return candidates;
    }
    
    // This is really the heart of the problem
    // Does path checking based on a simple vector of city id's
    // Only allows a duplicate goal city at size == num_cities
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
    
    return candidates;
}

// Compares State equality for T with another T
bool TravellingSalesman::isStateEqual(std::shared_ptr<TravellingSalesman>& other) {
    bool result = this->city == other->city;
    if (!result) {
        return false;
    }
    else if (this->previousCities.size() != other->previousCities.size()) {
        return false;
    }
    else {
        for (size_t i=0; i<previousCities.size(); i++) {
            result &= this->previousCities.at(i) == other->previousCities.at(i);
        }
    }
    return result;
}

// Tests if T is a goal node, optionally with input T Goal
bool TravellingSalesman::isGoal(TravellingSalesman& goal) {
    if (this->city != goal.city) {
        return false;
    }
    else {
        bool result = this->previousCities.size() == num_cities+1;
        return result;
    }
}

// Computes a hash for T's state
size_t TravellingSalesman::hashState() {
    std::hash<unsigned int> hasher;
    std::hash<size_t> szhasher;
    size_t hashval = hasher(this->city);
    for (auto i : previousCities) {
        hashval += i;
    }
    return szhasher(hashval);
}

// setHeuristic for node
void TravellingSalesman::setHeuristic(size_t newheuristic) {
    this->heuristic = newheuristic;
}

// getHeuristic cost for node
size_t TravellingSalesman::getHeuristic() const {
    return this->heuristic;
}

// Print out the node
void TravellingSalesman::printNode() {
    std::cout << "City: " << city_names[city] << ", Cost: " << cost << ", prevCitiesSize: " << previousCities.size() << std::endl;
}