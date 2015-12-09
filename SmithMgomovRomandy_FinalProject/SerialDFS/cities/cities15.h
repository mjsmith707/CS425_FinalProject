#ifndef CITIES_H
#define CITIES_H
#include <string>
using namespace std;
const unsigned int num_cities = 15;

const string city_names[num_cities] = {
	"Moroni",
	"Sainte-Clotilde",
	"Ar Ruways",
	"Bishkek",
	"Aden",
	"Ura Vajgurore",
	"Adamstown",
	"Niksic",
	"Damietta",
	"Kathmandu",
	"Kavaratti",
	"Christiansted",
	"New Amsterdam",
	"Constitucion",
	"Termez"
};

const unsigned int mileage_table[][num_cities] = {
	{0, 2474, 3715, 59, 1575, 3408, 1036, 3907, 3155, 2219, 2753, 3932, 1838, 1362, 1464},
	{2474, 0, 3885, 3076, 1251, 1376, 2290, 460, 475, 3540, 181, 406, 3553, 1247, 2495},
	{3715, 3885, 0, 96, 267, 3444, 79, 2383, 1568, 3979, 2709, 1786, 2886, 732, 2312},
	{59, 3076, 96, 0, 3921, 2356, 3266, 1029, 2687, 2899, 1983, 1054, 3407, 2757, 758},
	{1575, 1251, 267, 3921, 0, 2603, 3137, 3449, 1084, 2118, 3246, 2394, 3665, 2599, 108},
	{3408, 1376, 3444, 2356, 2603, 0, 1596, 2976, 2491, 2786, 1238, 2987, 2865, 848, 2397},
	{1036, 2290, 79, 3266, 3137, 1596, 0, 1935, 2619, 895, 3422, 3960, 186, 3034, 3041},
	{3907, 460, 2383, 1029, 3449, 2976, 1935, 0, 2979, 985, 26, 1353, 3530, 2257, 1965},
	{3155, 475, 1568, 2687, 1084, 2491, 2619, 2979, 0, 335, 1578, 2585, 884, 2005, 2489},
	{2219, 3540, 3979, 2899, 2118, 2786, 895, 985, 335, 0, 3748, 1100, 3149, 3869, 3435},
	{2753, 181, 2709, 1983, 3246, 1238, 3422, 26, 1578, 3748, 0, 2068, 1166, 2363, 3228},
	{3932, 406, 1786, 1054, 2394, 2987, 3960, 1353, 2585, 1100, 2068, 0, 1075, 60, 88},
	{1838, 3553, 2886, 3407, 3665, 2865, 186, 3530, 884, 3149, 1166, 1075, 0, 2503, 1405},
	{1362, 1247, 732, 2757, 2599, 848, 3034, 2257, 2005, 3869, 2363, 60, 2503, 0, 1991},
	{1464, 2495, 2312, 758, 108, 2397, 3041, 1965, 2489, 3435, 3228, 88, 1405, 1991, 0}
};
#endif