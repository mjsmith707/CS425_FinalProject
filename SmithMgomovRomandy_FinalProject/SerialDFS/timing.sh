#!/bin/bash
# Automated timing script
# Set #undef VERBOSE in main.cpp to generate csv output
# The number of times to run the for number cities
iterations=3

# The first city to start with. This corresponds to cities/city#.h
cityMin=5

# The upper bound on how many cities to attempt
cityUpperBound=15

# Location of cities headers
citiesBase="cities/cities"
citiesExt=".h"

# How long to run the program before giving up
timeoutDuration="10m"

# Output file
output="times.txt"

# Script Start
retcode=0
counter=0
citiesFile=""
make clean
rm ./$output

while [ $counter -lt $iterations ]; do
	let city=$cityMin;
	while [ $city -le $cityUpperBound ]; do
		# Copy in new cities.h
		citiesFile=$citiesBase$city$citiesExt
		cp ./$citiesFile ./cities.h
		# Rebuild
		make clean && make
		timeout $timeoutDuration ./serialdfs.x >> $output
		# Check return code
		# if failed then skip the rest of the cities
		retcode=$?
		if [[ $retcode != 0 ]];
		then
			let city=$cityUpperBound+1
		else
			let city=city+1
		fi
	done
	let counter=counter+1
done
