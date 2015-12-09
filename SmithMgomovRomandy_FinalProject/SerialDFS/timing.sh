#!/bin/bash
counter=0
city=5
citiesbase="cities/cities"
citiesext=".h"
citiesfile=""
numthreads=1
output="times.txt"
retcode=0


make clean
rm ./$output

#while [ $counter -lt 4 ]; do
	let numthreads=1
	while [ $numthreads -lt 2 ]; do
		let city=5;
		while [ $city -lt 21 ]; do
			citiesfile=$citiesbase$city$citiesext
			cp ./$citiesfile ./cities.h
			make clean && make
			timeout 10m ./serialdfs.x $numthreads >> $output
			retcode=$?
			if [[ $retcode != 0 ]];
			then
				let city=21
			else
				let city=city+1
			fi
		done
		let numthreads=numthreads*2
	done
	let counter=counter+1
#done
