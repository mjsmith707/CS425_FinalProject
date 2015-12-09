#ifndef CITIES_H
#define CITIES_H
#include <string>
using namespace std;
const unsigned int num_cities = 5;

const string city_names[num_cities] = {
	"Kampala",
	"Pointe Noire",
	"Mandalgovi",
	"Charlestown",
	"Sabak Bernam"
};

const unsigned int mileage_table[][num_cities] = {
	{0, 791, 3960, 654, 2755},
	{791, 0, 2907, 578, 2197},
	{3960, 2907, 0, 3316, 3709},
	{654, 578, 3316, 0, 389},
	{2755, 2197, 3709, 389, 0}
};
#endif
