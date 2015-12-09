#ifndef CITIES_H
#define CITIES_H
#include <string>
using namespace std;
const unsigned int num_cities = 8;

const string city_names[num_cities] = {
	"Paeau",
	"Freeport",
	"Saint-Germain-Laxis",
	"Port-aux-Francais",
	"Umwa Village",
	"Buren",
	"Faaa",
	"Antarctica"
};

const unsigned int mileage_table[][num_cities] = {
	{0, 604, 2376, 1369, 3642, 3059, 439, 3173},
	{604, 0, 1245, 3171, 847, 906, 432, 3525},
	{2376, 1245, 0, 680, 1470, 704, 3852, 2932},
	{1369, 3171, 680, 0, 3077, 3945, 698, 3061},
	{3642, 847, 1470, 3077, 0, 3962, 2842, 1490},
	{3059, 906, 704, 3945, 3962, 0, 1183, 3889},
	{439, 432, 3852, 698, 2842, 1183, 0, 1358},
	{3173, 3525, 2932, 3061, 1490, 3889, 1358, 0}
};
#endif
