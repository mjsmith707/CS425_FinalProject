//
//  main.cpp
//  FinalProject
//
//  Created by Matt on 11/27/15.
//  Copyright (c) 2015 Matt. All rights reserved.
//

#include <iostream>
#include <algorithm>
#include "cities.h"
#include "ParallelTSPDemo.h"

#define N_THREADS 4
#define N_TASKS_PER_THREAD 10

typedef std::chrono::high_resolution_clock::time_point timept_t;
typedef std::chrono::duration<double> timer_duration_t;

int main(int argc, const char * argv[]) {
    timept_t startTime, endTime;
    timer_duration_t duration;


    std::cout << "num_cities: " << num_cities << ", N_THREADS: " << N_THREADS << std::endl;
    std::cout << std::endl << "======Max Parallel Given DFS serial implementation======" << std::endl;
    startTime = std::chrono::high_resolution_clock::now();
    ParallelTSPDemo::Tour bestTour(std::numeric_limits<unsigned int>::max());
    ParallelTSPDemo::Tour startTour;
    ParallelTSPDemo::ParallelTSPDemo demoRun;
    ParallelTSPDemo::Tour finalSolution = demoRun.runParallelDFS(0);
    endTime = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);
    std::cout << "Duration: " << duration.count();
    std::cout << finalSolution << std::endl;
    
    /*
    std::cout << std::endl << "======Given DFS serial implementation======" << std::endl;
    startTime = std::chrono::high_resolution_clock::now();
    Tour t;
    DFS(t);
    endTime = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);
    std::cout << "Duration: " << duration.count();
    std::cout << best << std::endl;
    */
    return 0;
}
