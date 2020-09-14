#!/bin/bash
for name in proton pi+ pi-
do
    rm $name.depth $name.dedx
    sed "s|/gun/particle .*$|/gun/particle $name|g" run.mac -i
    for i in $(cat energy_list)
    do
        echo doing sim for $name  $i
        sed "s|.* MeV$|/gun/energy $i MeV|g" run.mac -i
        ./protoninlxr run.mac 2>&1 >> /dev/null
        ./analysis/getpeak $name 2>&1 >> /dev/null
    done
done