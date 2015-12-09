//
//  SerialTravellingSalesmanDFS.cpp
//  FinalProjectParallel
//
//  Created by Max on 12/7/15.
//  Copyright (c) 2015 Matt. All rights reserved.
//

#include <stdio.h>
#include "SerialTravellingSalesmanDFS.h"

Tour best(UINT_MAX);

Tour::Tour(){
    cities.push_back(0);
    contains.reset();
    contains[0] = 1;
    mileage = 0;
}

Tour::Tour(unsigned int cost){
     mileage = cost;
}

Tour::Tour(const Tour& t){
    cities = t.cities;
    mileage = t.mileage;
    contains = t.contains;
}

unsigned int Tour::get_num_cities() {
    return cities.size();
}

bool Tour::is_present(unsigned int c) {
    return contains[c] == 1;
}

bool Tour::better_than(const Tour& t) {
    return (mileage < t.mileage);
}

unsigned int Tour::mileage_delta(unsigned int c) {
    unsigned int last = cities.back(); // current last city
    unsigned int test_mileage = mileage_table[last][c];
    
    // If this would be our last city, add the mileage
    // to return home
    if (cities.size() + 1 == num_cities) {
        test_mileage += mileage_table[c][0];
    }
    return test_mileage;
}

void Tour::add_city(unsigned int c) {
    mileage += mileage_delta(c);
    contains[c] = 1;
    cities.push_back(c);
}

bool Tour::add_city(unsigned int c, const Tour& best_so_far) {
    // If city is already in the list, return false
    if (is_present(c)) {
        return false;
    }
    // If this would be worse than our best completed tour,
    // return false
    if (mileage + mileage_delta(c) > best_so_far.mileage) {
#if DFS_DEBUG
        cout << "Not adding " << city_names[c]
        << " -- mileage would be " << mileage + mileage_delta(c)
        << " -- best is " << best_so_far.mileage << endl;
#endif
        return false;
    }
    add_city(c);
#if DFS_DEBUG
    cout << "Added " << city_names[c] << endl;
    cout << "Tour is now " << *this;
#endif
    return true;
}

void Tour::remove_last() {
    unsigned int c = cities.back();
    cities.pop_back();
    contains[c] = 0;
    
    // Subtract the mileage that c added to the cost
    mileage -= mileage_delta(c);
#if DFS_DEBUG
    cout << "Removed " << city_names[c] << endl;
    cout << "Tour is now " << *this;
#endif
}

// Output operator: print cost and list of names
ostream& operator<<(ostream &outs, Tour t) {
    cout << "\nCost = " << t.mileage << endl;
    for (vector<unsigned int>::iterator it = t.cities.begin();
         it != t.cities.end(); it++) {
        cout << city_names[*it] << endl;
    }
    cout << endl;
    return outs;
}

void DFS(Tour& t) {
    if (!t.better_than(best)) return; // if we've already seen something better
    
    // If we've added all the cities and we're still here, this must be
    // the best tour
    if (t.get_num_cities() == num_cities) {
        best = t;
#ifdef DFS_DEBUG
        cout << "New best:" << best;
#endif
        
    }
    // Otherwise, try to add the next city and recursively call DFS
    // if successful
    else {
        for (unsigned int i=0; i < num_cities; i++) {
            if (t.add_city(i, best)) {
                DFS(t);
                t.remove_last();
            }
        }
    }
}
