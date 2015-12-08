#ifndef CITIES_H
#define CITIES_H
#include <string>
using namespace std;
const unsigned int num_cities = 6;

const string city_names[num_cities] = {
	"Thanh Pho Hoa Binh",
	"Montmajor",
	"Cupertino",
	"Bangar",
	"Lisice",
	"Ciorescu"
};

const unsigned int mileage_table[][num_cities] = {
	{0, 1633, 635, 2787, 603, 2781},
	{1633, 0, 3124, 460, 2198, 3444},
	{635, 3124, 0, 2570, 715, 826},
	{2787, 460, 2570, 0, 3358, 880},
	{603, 2198, 715, 3358, 0, 2938},
	{2781, 3444, 826, 880, 2938, 0}
};
#endif
