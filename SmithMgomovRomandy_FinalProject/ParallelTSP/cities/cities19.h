#ifndef CITIES_H
#define CITIES_H
#include <string>
using namespace std;
const unsigned int num_cities = 19;

const string city_names[num_cities] = {
	"Butuan",
	"Phanna Nikhom",
	"Roma",
	"Bodden Town",
	"Kampala",
	"Titikaveka",
	"Cambote",
	"Baie Mahault",
	"Al Sohar",
	"Ciudad Piar",
	"Santa Barbara",
	"Tristan Da Cunha",
	"Moanda",
	"Tristan Da Cunha",
	"Cupertino",
	"Mozambique",
	"La Floresta",
	"`Ara",
	"Burutu"
};

const unsigned int mileage_table[][num_cities] = {
	{0, 3105, 2083, 745, 2713, 3137, 2643, 2887, 1883, 2207, 2395, 1875, 1553, 627, 2324, 1557, 3113, 106, 3906},
	{3105, 0, 1158, 1425, 2029, 2731, 228, 63, 2587, 3390, 1229, 2938, 942, 423, 799, 3989, 2239, 2379, 3177},
	{2083, 1158, 0, 2183, 2954, 3926, 2933, 482, 3201, 2293, 2225, 3254, 2212, 848, 810, 2812, 1644, 3333, 2904},
	{745, 1425, 2183, 0, 589, 1652, 3895, 2605, 1471, 29, 31, 3329, 663, 2902, 1645, 2415, 382, 385, 602},
	{2713, 2029, 2954, 589, 0, 1017, 1855, 2130, 3357, 1919, 1381, 1834, 2763, 2079, 3308, 725, 1517, 899, 3330},
	{3137, 2731, 3926, 1652, 1017, 0, 884, 830, 3063, 2476, 995, 2987, 3258, 2532, 3958, 2393, 904, 1828, 3658},
	{2643, 228, 2933, 3895, 1855, 884, 0, 1818, 681, 2281, 2635, 3526, 2956, 2595, 2955, 2271, 1390, 1891, 2124},
	{2887, 63, 482, 2605, 2130, 830, 1818, 0, 856, 476, 1773, 41, 3743, 1628, 1058, 2870, 1726, 258, 2374},
	{1883, 2587, 3201, 1471, 3357, 3063, 681, 856, 0, 3535, 1620, 2339, 2899, 2444, 2861, 1221, 215, 3939, 3995},
	{2207, 3390, 2293, 29, 1919, 2476, 2281, 476, 3535, 0, 1990, 1811, 2734, 3461, 782, 1765, 2494, 1087, 3010},
	{2395, 1229, 2225, 31, 1381, 995, 2635, 1773, 1620, 1990, 0, 2558, 3111, 2551, 3620, 2622, 1591, 233, 1526},
	{1875, 2938, 3254, 3329, 1834, 2987, 3526, 41, 2339, 1811, 2558, 0, 2190, 3965, 2643, 3397, 1878, 96, 3345},
	{1553, 942, 2212, 663, 2763, 3258, 2956, 3743, 2899, 2734, 3111, 2190, 0, 148, 1149, 3786, 403, 54, 1111},
	{627, 423, 848, 2902, 2079, 2532, 2595, 1628, 2444, 3461, 2551, 3965, 148, 0, 1853, 2756, 3960, 2885, 233},
	{2324, 799, 810, 1645, 3308, 3958, 2955, 1058, 2861, 782, 3620, 2643, 1149, 1853, 0, 300, 2021, 678, 890},
	{1557, 3989, 2812, 2415, 725, 2393, 2271, 2870, 1221, 1765, 2622, 3397, 3786, 2756, 300, 0, 3341, 894, 1500},
	{3113, 2239, 1644, 382, 1517, 904, 1390, 1726, 215, 2494, 1591, 1878, 403, 3960, 2021, 3341, 0, 1961, 1302},
	{106, 2379, 3333, 385, 899, 1828, 1891, 258, 3939, 1087, 233, 96, 54, 2885, 678, 894, 1961, 0, 3865},
	{3906, 3177, 2904, 602, 3330, 3658, 2124, 2374, 3995, 3010, 1526, 3345, 1111, 233, 890, 1500, 1302, 3865, 0}
};
#endif
