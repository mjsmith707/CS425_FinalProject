/*
 * CS425 Final Project
 * Matt Smith, Reuben Romandy, Maksim Gomov
 * ParallelTravellingSalesmanDFS.h
 * 
 * Header for the parallel travelling salesman implementation. Contains the derivative 
 * implementation of the provided tsp_serial_recursive.cpp, modified for parallel execution.
 */

#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <unordered_set>
#include <queue>
#include <thread>
#include <atomic>
#include <bitset>
#include "cities.h"
#include "SharedQueue.h"

using namespace std;

class ParallelTravellingSalesmanDFS{
public:
    class Tour {
    public:
        // Used to initialize new tours.
        Tour() {
            contains.reset();
            cities.push_back(0); mileage = 0;
            contains[0] = 1;
        }
        
        // Used to initialize best tour. Set to a high
        // cost so that the first complete tour will become
        // the new "best."
        Tour(unsigned int cost) { mileage = cost; }
        
        // Copy constructor -- in recursive algorithm,
        // only used to update the "best" tour
        Tour(const Tour& t) {
            cities = t.cities;
            mileage = t.mileage;
            contains = t.contains;
        }
        
        // ----- Accessors -----
        unsigned int get_num_cities() { return cities.size(); }
        
        // Returns True if City c is in the tour
        bool is_present(unsigned int c) {
            return contains[c] == 1;
        }
        
        
        bool better_than(const Tour& t) { return (mileage < t.mileage); }
        // Returns True if this tour is better than Tour t, and in general better than the shared bound
        bool better_than(const Tour& t, const std::atomic<unsigned int>* sharedBound) { return (mileage < *sharedBound) && (mileage < t.mileage); }
        
        // ----- Adding and removing cities -----
        
        // Compute change in mileage if City c is added to the list
        unsigned int mileage_delta(unsigned int c) {
            unsigned int last = cities.back(); // current last city
            unsigned int test_mileage = mileage_table[last][c];
            
            // If this would be our last city, add the mileage
            // to return home
            if (cities.size() + 1 == num_cities) {
                test_mileage += mileage_table[c][0];
            }
            
            return test_mileage;
        }
        
        
        // Add city unconditionally (without checking anything)
        void add_city(unsigned int c) {
            mileage += mileage_delta(c);
            contains[c] = 1;
            cities.push_back(c);
        }
        
        // Add city conditionally
        // Check to make sure that (a) it is not already present
        // and (b) it would not lead to a tour that is worse
        // than the best tour so far
        // Return True if city was added
        bool add_city(unsigned int c, const Tour& best_so_far) {
            // If city is already in the list, return false
            if (is_present(c)) {
                return false;
            }
            // If this would be worse than our best completed tour,
            // return false
            if (mileage + mileage_delta(c) > best_so_far.mileage) {
                return false;
            }
            add_city(c);
            
            return true;
        }
        
        // Delete the last city
        void remove_last() {
            unsigned int c = cities.back();
            cities.pop_back();
            contains[c] = 0;
            
            // Subtract the mileage that c added to the cost
            mileage -= mileage_delta(c);
        }
        
        // Output operator: print cost and list of names
        friend ostream& operator << (ostream &outs, Tour t) {
            cout << "\nCost = " << t.mileage << endl;
            for (vector<unsigned int>::iterator it = t.cities.begin();
                 it != t.cities.end(); it++) {
                cout << city_names[*it] << endl;
            }
            cout << endl;
            return outs;
        }
        
        unsigned int getMileage(){
            return mileage;
        }
        
    private:
        vector<unsigned int> cities;
        unsigned int mileage;
        bitset<100> contains; // bitset for contains in path set, much faster checking than iterating through cities
    };

    // slightly modified serial DFS implementation
    void DFS(Tour& t, Tour& best, std::atomic<unsigned int>* sharedBound);
    
    // function to call for 
    Tour runParallelDFS(unsigned int N_THREADS);
    void RunAgainstTaskQueue(Shared_Queue<std::pair<unsigned int, Tour>>* taskQueue, std::vector<Tour>* sharedResults, std::atomic<unsigned int>* sharedBound);
    
    
};
