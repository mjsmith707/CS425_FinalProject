#ifndef CITIES_H
#define CITIES_H
#include <string>
using namespace std;
const unsigned int num_cities = 10;

const string city_names[num_cities] = {
	"Nova IguaÃ§u",
	"Porto",
	"Saveh",
	"The Valley",
	"Eschenbach",
	"Atafu Village",
	"Gibraltar",
	"Nouakchott",
	"Budva",
	"Narayanganj"
};

const unsigned int mileage_table[][num_cities] = {
	{0, 3022, 2470, 2108, 2858, 2295, 39, 2161, 2256, 2516},
	{3022, 0, 3567, 3469, 3579, 1689, 3664, 279, 3595, 1565},
	{2470, 3567, 0, 3533, 311, 3694, 2111, 1291, 269, 400},
	{2108, 3469, 3533, 0, 1818, 2936, 774, 909, 3704, 3041},
	{2858, 3579, 311, 1818, 0, 3251, 1635, 2124, 1256, 1862},
	{2295, 1689, 3694, 2936, 3251, 0, 3292, 3535, 857, 98},
	{39, 3664, 2111, 774, 1635, 3292, 0, 337, 3814, 1759},
	{2161, 279, 1291, 909, 2124, 3535, 337, 0, 2502, 680},
	{2256, 3595, 269, 3704, 1256, 857, 3814, 2502, 0, 1367},
	{2516, 1565, 400, 3041, 1862, 98, 1759, 680, 1367, 0}
};
#endif
