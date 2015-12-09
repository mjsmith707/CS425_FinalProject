// json blob from https://github.com/David-Haim/CountriesToCitiesJSON/blob/master/countriesToCities.json
var countries = require('./countries.json');

// arbitrary max distance
var maxDistance = 4000;

var countryNames = [];
for(country in countries){
    countryNames.push(country);
}

if(process.argv.length < 3) {
    console.log('Usage: node <this.js> <number of cities>');
    return -1;
}

var citiesToGenerate = parseInt(process.argv[2]);

if(citiesToGenerate == undefined || isNaN(citiesToGenerate) || citiesToGenerate < 0){
    console.log("Invalid input");
    return -1;
}

console.log(citiesToGenerate);

var alreadyUsedCities = {};

for(var i = 0; i < citiesToGenerate; i++){
    var chosenCountry, chosenCity;

    // potential for picking cities with name "" for some reason, so just grab another city
    while((chosenCity == undefined && alreadyUsedCities[chosenCity] == undefined) || chosenCity == '' || chosenCity.length < 2){
        chosenCountry = countries[countryNames[Math.floor(Math.random() * countryNames.length)]];
        chosenCity = chosenCountry[Math.floor(Math.random() * chosenCountry.length)];
        alreadyUsedCities[chosenCity] = true;
    }
    
    // print out the city name into the python script
    console.log(chosenCity);
    chosenCountry = undefined;
    chosenCity = undefined;
}

// print out some random distance between cities, also into the python script 
for(i = 0; i < citiesToGenerate * citiesToGenerate; i++){
    console.log(Math.round(Math.random() * maxDistance));
}
