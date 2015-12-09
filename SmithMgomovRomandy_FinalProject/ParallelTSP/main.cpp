/*
 * CS425 Final Project
 * Matt Smith, Reuben Romandy, Maksim Gomov
 * Main.cpp
 * 
 * The main program, contains code for outputting in the CSV format we used for our data 
 * collection.
 */

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include "cities.h"
#include "SerialTravellingSalesmanDFS.h"
#include "ParallelTravellingSalesmanDFS.h"

// Remove to generate csv output
#define VERBOSE

// Remove to disable parallel implementation
#define PARALLEL

// Remove to disable serial implementation
#define SERIAL

static unsigned int N_THREADS;

typedef std::chrono::high_resolution_clock::time_point timept_t;
typedef std::chrono::duration<double> timer_duration_t;

int main(int argc, const char * argv[]) {
    if (argc != 2) {
	std::cout << "Usage: ./project.out numthreads" << std::endl;
	exit(-1);
    }
    if (num_cities < 5) {
	std::cout << "Number of cities is too low for any useable performance comparison." << std::endl;
    }
    N_THREADS = strtol(argv[1],NULL,10);

    timept_t startTime, endTime;
    timer_duration_t duration;

#ifdef PARALLEL
    startTime = std::chrono::high_resolution_clock::now();
    ParallelTravellingSalesmanDFS::Tour bestTour(std::numeric_limits<unsigned int>::max());
    ParallelTravellingSalesmanDFS::Tour startTour;
    ParallelTravellingSalesmanDFS demoRun;
    ParallelTravellingSalesmanDFS::Tour solution = demoRun.runParallelDFS(N_THREADS);
    endTime = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);

  #ifdef VERBOSE
    std::cout << "num_cities: " << num_cities << ", N_THREADS: " << N_THREADS << std::endl;
    std::cout << std::endl << "======Parallel Given DFS serial implementation======" << std::endl;
    std::cout << "Duration: " << duration.count();
    std::cout << solution << std::endl;
  #else
    std::cout << "0," << N_THREADS << "," << num_cities;
    std::cout << "," << duration.count() << std::endl;
  #endif

#endif

#ifdef SERIAL
    startTime = std::chrono::high_resolution_clock::now();
    Tour t;
    DFS(t);
    endTime = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);

  #ifdef VERBOSE
    std::cout << std::endl << "======Given DFS serial implementation======" << std::endl;
    std::cout << "Duration: " << duration.count();
    std::cout << best << std::endl;
  #else
    std::cout << "1," << N_THREADS << "," << num_cities;
    std::cout << "," << duration.count() << std::endl;
  #endif

#endif

    return 0;
}
