#!/bin/bash

echo "Seeing node errors is normal after this point; the generated output is fine."
# don't know why node errors here, node being node

for i in {2..30} 
do
    node gencities.js $i | python3 genmileage.py > CITIES$i.h # run it into a temporary file
    tail -n +2 CITIES$i.h > cities$i.h # trim the python prompts out of the temporary text files
done

rm CITIES* # remove temporary text files
