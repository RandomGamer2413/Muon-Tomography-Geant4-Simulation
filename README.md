I learned C++ and Geant4 to build a physics simulation consisting of muon detectors, dense volume overburdens, and a muon generation plane to validate the hardware of a startup I was working at.
This also includes Python code I wrote to perform data analysis on the simulation data I generated!

In terms of the structure of the files, all files beginning with "CRY", as well as the "data" directory, are part of a library I setup to aid with muon generation (Cosmic Ray Shower Library).
"EcoMug.h" is also a library for muon generation that I looked into as an alternative to CRY.
The "sslg4" and "OPSim" directories contain code I found on the internet that helped implement scintillating materials into our simulation, although the current version of the simulation doesn't use them.

Most all other files have code that I either directly wrote or extensively modified from a template to achieve the intended experiment I was building.
There are 4 build folders to aid with parallelization of the simulation, and all relevant details about the .cc and .hh files I wrote are commented within them and are hinted at by their names.

To run a simulation, you should compile and build the source code in a build directory, and then execute the executable "sim" by typing "./sim" in the build directory terminal to run the sim with GUI activated, or "./sim run.mac" to run the simulation in batch mode, with the specifics of the run detailed in the "run.mac" macro file.
Once the simulation has successfully finished, a ROOT output file will be generated in the build folder, containing information that the muon detectors collected.
To use this data and analyze it, you can either use ROOT, or use my Python code in "finalExperiment1.ipynb", which has many data processing functions tailored for converting the ROOT output into a clean, filtered, and useful Pandas DataFrame / NumPy Array in the context of my muon tomography simulation.
There is also examples of relevant data analysis in this notebook!
