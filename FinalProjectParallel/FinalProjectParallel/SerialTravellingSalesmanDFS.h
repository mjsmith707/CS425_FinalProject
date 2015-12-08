//
//  SerialTravellingSalesmanDFS.h
//  FinalProjectParallel
//
//  Created by Max on 12/7/15.
//  Copyright (c) 2015 Matt. All rights reserved.
//

#ifndef FinalProjectParallel_SerialTravellingSalesmanDFS_h
#define FinalProjectParallel_SerialTravellingSalesmanDFS_h

#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include "cities.h"
using namespace std;

/* Recursive, depth-first implementation of tree search. */
/* ----------------------------------------------------- */


// Class to keep track of tour so far
//
// Right now, we only have 2 Tour objects (the current Tour,
// and the best Tour we've seen).
//
// Non-recursive implementations will require us to save more tours.
class Tour {
public:
    // Used to initialize new tours.
    Tour();
    
    // Used to initialize best tour. Set to a high
    // cost so that the first complete tour will become
    // the new "best."
    Tour(unsigned int cost);
    
    // Copy constructor -- in recursive algorithm,
    // only used to update the "best" tour
    Tour(const Tour& t);
    
    // ----- Accessors -----
    
    unsigned int get_num_cities();
    
    // Returns True if City c is in the tour
    bool is_present(unsigned int c);
    
    // Returns True if this tour is better than Tour t
    bool better_than(const Tour& t);
    
    // ----- Adding and removing cities -----
    
    // Compute change in mileage if City c is added to the list
    unsigned int mileage_delta(unsigned int c);
    
    // Add city unconditionally (without checking anything)
    void add_city(unsigned int c);
    
    // Add city conditionally
    // Check to make sure that (a) it is not already present
    // and (b) it would not lead to a tour that is worse
    // than the best tour so far
    // Return True if city was added
    bool add_city(unsigned int c, const Tour& best_so_far);
    
    // Delete the last city
    void remove_last();
    
    // Output operator: print cost and list of names
    friend ostream& operator << (ostream &outs, Tour t);
    
private:
    vector<unsigned int> cities;  // Visited cities, in order
    unsigned int mileage; // Total mileage
};

// global for best tour so far - initialized high
extern Tour best;

// Do a depth-first search to find the best tour
// starting with the partially complete tour t
void DFS(Tour& t);

#endif
