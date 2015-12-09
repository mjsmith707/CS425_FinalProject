#ifndef CITIES_H
#define CITIES_H
#include <string>
using namespace std;
const unsigned int num_cities = 17;

const string city_names[num_cities] = {
	"Antarctica",
	"Cupertino",
	"Bandar Seri Begawan",
	"Surco",
	"Dorp Nikiboko",
	"Egbeda",
	"Georgetown",
	"Biezanow-Prokocim",
	"Roma",
	"Honiara",
	"Rohnert Park",
	"Grytviken",
	"Peroshki",
	"Mata-Utu",
	"Perote",
	"Victoria",
	"Vredeberg"
};

const unsigned int mileage_table[][num_cities] = {
	{0, 1897, 2030, 3860, 3540, 2615, 2521, 2006, 1444, 1679, 1083, 3876, 791, 3588, 977, 612, 1484},
	{1897, 0, 1300, 3120, 3264, 3225, 2774, 3875, 1938, 3364, 1192, 3620, 1417, 1823, 2299, 1188, 1992},
	{2030, 1300, 0, 718, 3775, 2961, 2119, 306, 779, 563, 2587, 3261, 3955, 2264, 1227, 2645, 918},
	{3860, 3120, 718, 0, 1848, 642, 2817, 966, 3547, 662, 1483, 2354, 2157, 3813, 2856, 3081, 2745},
	{3540, 3264, 3775, 1848, 0, 3549, 3732, 1746, 3806, 1732, 3309, 71, 3598, 799, 27, 1366, 809},
	{2615, 3225, 2961, 642, 3549, 0, 3150, 45, 428, 2148, 625, 2253, 2698, 1002, 1932, 1811, 681},
	{2521, 2774, 2119, 2817, 3732, 3150, 0, 2001, 1112, 1356, 3233, 67, 1228, 1561, 2744, 2964, 1789},
	{2006, 3875, 306, 966, 1746, 45, 2001, 0, 3741, 3, 1558, 3002, 403, 114, 3302, 2331, 1446},
	{1444, 1938, 779, 3547, 3806, 428, 1112, 3741, 0, 1921, 1847, 943, 483, 1874, 3905, 171, 3820},
	{1679, 3364, 563, 662, 1732, 2148, 1356, 3, 1921, 0, 2283, 2277, 2501, 1768, 1999, 2822, 1538},
	{1083, 1192, 2587, 1483, 3309, 625, 3233, 1558, 1847, 2283, 0, 1231, 1726, 1581, 978, 1329, 466},
	{3876, 3620, 3261, 2354, 71, 2253, 67, 3002, 943, 2277, 1231, 0, 3286, 1471, 1603, 1395, 784},
	{791, 1417, 3955, 2157, 3598, 2698, 1228, 403, 483, 2501, 1726, 3286, 0, 321, 1375, 2052, 856},
	{3588, 1823, 2264, 3813, 799, 1002, 1561, 114, 1874, 1768, 1581, 1471, 321, 0, 263, 3162, 2952},
	{977, 2299, 1227, 2856, 27, 1932, 2744, 3302, 3905, 1999, 978, 1603, 1375, 263, 0, 1260, 550},
	{612, 1188, 2645, 3081, 1366, 1811, 2964, 2331, 171, 2822, 1329, 1395, 2052, 3162, 1260, 0, 2096},
	{1484, 1992, 918, 2745, 809, 681, 1789, 1446, 3820, 1538, 466, 784, 856, 2952, 550, 2096, 0}
};
#endif