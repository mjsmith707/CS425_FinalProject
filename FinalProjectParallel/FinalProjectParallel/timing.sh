#!/bin/bash
counter=2
citiesbase="cities/cities"
citiesext=".h"
citiesfile=""
numthreads=1
output="times.txt"
retcode=0


make clean
rm ./$output

while [ $numthreads -lt 33 ]; do
	while [ $counter -lt 20 ]; do
		citiesfile=$citiesbase$counter$citiesext
		cp ./$citiesfile ./cities.h
		make clean && make
		echo $numthreads >> $output
		echo $counter >> $output
		./project.x $numthreads >> $output
		retcode=$?
		if [[ $retcode != 0 ]];
		then
			exit $rc;
		fi
		let counter=counter+1
	done
	let numthreads=numthreads+1
done
