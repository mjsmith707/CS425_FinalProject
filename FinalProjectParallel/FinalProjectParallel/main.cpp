//
//  main.cpp
//  FinalProject
//
//  Created by Matt on 11/27/15.
//  Copyright (c) 2015 Matt. All rights reserved.
//

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include "cities.h"
#include "SerialTravellingSalesmanDFS.h"
#include "ParallelTravellingSalesmanDFS.h"

#define N_TASKS_PER_THREAD 10

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


    //std::cout << "num_cities: " << num_cities << ", N_THREADS: " << N_THREADS << std::endl;
    std::cout << N_THREADS << "," << num_cities;
    //std::cout << std::endl << "======Max Parallel Given DFS serial implementation======" << std::endl;
    startTime = std::chrono::high_resolution_clock::now();
    ParallelTravellingSalesmanDFS::Tour bestTour(std::numeric_limits<unsigned int>::max());
    ParallelTravellingSalesmanDFS::Tour startTour;
    ParallelTravellingSalesmanDFS demoRun;
    ParallelTravellingSalesmanDFS::Tour finalSolution = demoRun.runParallelDFS(0, N_THREADS);
    endTime = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);
    //std::cout << "Duration: " << duration.count();
    std::cout << "," << duration.count();
    //std::cout << finalSolution << std::endl;
    
    //std::cout << std::endl << "======Given DFS serial implementation======" << std::endl;
    startTime = std::chrono::high_resolution_clock::now();
    Tour t;
    DFS(t);
    endTime = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);
    //std::cout << "Duration: " << duration.count();
    //std::cout << best << std::endl;
    std::cout << "," << duration.count() << std::endl;

    return 0;
}
