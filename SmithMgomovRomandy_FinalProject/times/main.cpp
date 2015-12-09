#include <iostream>
#include <fstream>
#include <iomanip>

int main() {
	std::fstream input;
	input.open("dfsserialtimes.txt");
	double times[33][23];
	for (int i=0; i<33; i++) {
		for (int j=0; j<23; j++) {
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
		for (int j=0; j<23; j++) {
			times[i][j] /= 3;
		}
	}

	for (int i=1; i<33; i*=2) {
		for (int j=5; j<23; j++) {
			std::cout << std::fixed << std::setprecision(9) << "2," << i << "," << j << "," << times[i][j] << std::endl;
		}
	}
}
