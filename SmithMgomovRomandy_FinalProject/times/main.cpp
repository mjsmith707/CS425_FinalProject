#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <string>

int main(int argc, char** argv) {
	if (argc < 4) {
		std::cout << "Usage: ./a.out ID numIterations inputTimes.txt" << std::endl;
		std::cout << "numEntries is how many overall iteration were run for averaging purposes" << std::endl;
		std::cout << "Example Tags: 0 for parallel, 1 for Rivoire's DFS, 2 for serialDfs" << std::endl;
		exit(-1);
	}

	std::fstream input;
	input.open(argv[3]);
	if (!input.is_open()) {
		std::cout << "Failed to open file!" << std::endl;
		exit(-2);
	}

	int tag = std::stoi(argv[1]);
	int iterations = std::stoi(argv[2]);
	if (iterations == 0) {
		std::cout << "Don't divide by zero!" << std::endl;
		exit(-3);
	}

	double times[33][40];
	for (int i=0; i<33; i++) {
		for (int j=0; j<40; j++) {
			times[i][j] = 0.0f;
		}
	}
	
	int algo;
	int numthreads;
	int numcities;
	double time;
	char comma;

	while (input >> algo >> comma >> numthreads >> comma >> numcities >> comma >> time) {
		times[numthreads][numcities] += time;
	}

	for (int i=0; i<33; i++) {
		for (int j=0; j<40; j++) {
			times[i][j] /= iterations;
		}
	}

	for (int i=1; i<33; i*=2) {
		for (int j=0; j<40; j++) {
			if (times[i][j] > 0.0f) {
				std::cout << std::fixed << std::setprecision(9) << tag << "," << i << "," << j << "," << times[i][j] << std::endl;
			}
		}
	}
}
