# Running Requirement
- Geant 4 for simluation process
- ROOT >= 6 for viewing results

# Building
```Bash
mkdir build
cd build
cmake ..
make -j16
```

# Running
- Edit `build/run.mac` as needed.
- In `build` run `./protoninlxr`
- USE
  - `./stack disappear_mode` to see track length histogram stacked by the way it disappears.
  - `./stack stop_mode` to see track length histogram stacked by the way it stops.
  - `./read` to see the length distribution of length of ionizing stopped particle

# Information
physics process recording now supports proton, charged pions.
