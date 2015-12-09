#ifndef CITIES_H
#define CITIES_H
#include <string>
using namespace std;
const unsigned int num_cities = 9;

const string city_names[num_cities] = {
	"Siavonga",
	"Gustavia",
	"Takamaka",
	"Gogolesubani",
	"Machadodorp",
	"Port-au-Prince",
	"Cupertino",
	"Crows Nest",
	"Berekua"
};

const unsigned int mileage_table[][num_cities] = {
	{0, 1473, 1072, 2604, 147, 406, 16, 2004, 1335},
	{1473, 0, 1021, 2543, 2972, 587, 480, 3222, 3530},
	{1072, 1021, 0, 1191, 2840, 3537, 2373, 1865, 3658},
	{2604, 2543, 1191, 0, 2761, 3735, 1860, 1237, 1999},
	{147, 2972, 2840, 2761, 0, 2760, 3562, 2882, 3811},
	{406, 587, 3537, 3735, 2760, 0, 3369, 3677, 1094},
	{16, 480, 2373, 1860, 3562, 3369, 0, 3331, 1485},
	{2004, 3222, 1865, 1237, 2882, 3677, 3331, 0, 278},
	{1335, 3530, 3658, 1999, 3811, 1094, 1485, 278, 0}
};
#endif
