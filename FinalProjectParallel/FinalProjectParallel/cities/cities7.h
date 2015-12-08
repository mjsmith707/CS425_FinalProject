#ifndef CITIES_H
#define CITIES_H
#include <string>
using namespace std;
const unsigned int num_cities = 7;

const string city_names[num_cities] = {
	"Cupertino",
	"Beja",
	"LomÃ©",
	"Grytviken",
	"Santo Domingo",
	"DurrÃ«s",
	"Gogolesubani"
};

const unsigned int mileage_table[][num_cities] = {
	{0, 968, 1399, 2688, 2423, 2487, 3532},
	{968, 0, 2541, 2684, 3074, 680, 3768},
	{1399, 2541, 0, 2885, 3075, 1733, 767},
	{2688, 2684, 2885, 0, 680, 3125, 3296},
	{2423, 3074, 3075, 680, 0, 1689, 3114},
	{2487, 680, 1733, 3125, 1689, 0, 1805},
	{3532, 3768, 767, 3296, 3114, 1805, 0}
};
#endif
