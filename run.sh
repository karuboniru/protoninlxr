#!/bin/bash
for name in proton pi+ pi-
do
    sed "s|/gun/particle .*$|/gun/particle $name|g" run.mac -i
    for i in 10 20 30 40 50 60 70 80 90 100 120 140 180
    do
        echo doing sim for $name  $i
        sed "s|.* MeV$|/gun/energy $i MeV|g" run.mac -i
        ./protoninlxr run.mac 2>&1 >> /dev/null
        ./analysis/getpeak $name 2>&1 >> /dev/null
    done
done