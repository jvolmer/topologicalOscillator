/**
   TopoOsciSim
   computeTopoSuscMC.cpp
   Purpose: Compute Correlation of lattice variables from created MC configurations

   @author Julia Volmer
   @version 1.0 
*/

#include <iostream>
#include <random>
#include "parameters.hpp"
#include "file.hpp"
#include "timestep.hpp"
#include "lattice.hpp"
#include "cluster.hpp"

using namespace std;

int main (int argc, char *argv[])
{
    // initialize random generator
    random_device rd;
    mt19937_64 generator(rd());

    // define and initialize parameters
    TopoOsciSim::ParameterContainer parameters;

    // process command line input
    parameters.readInput(argc, argv);

    // open file to save correlations in
    TopoOsciSim::FileObs fCorr("Corr", parameters);
    fCorr.create();

    // open config file
    TopoOsciSim::FileConfig fConf(parameters);
    fConf.open();
    
    // set lattice
    TopoOsciSim::LatticeContainer lattice(parameters);
    // read configuration constants to lattice
    lattice.readHeader(fConf);
    
    // read configuration
    for (int i=0; i<parameters.Nsteps; i++)
    {
        // read conf and check for error (e.g. eof)
        if (!lattice.readConf(fConf))
            break;
        
        // compute correlation of lattice variable
        lattice.computeCorr();

        // save correlation in file
        lattice.dumpCorr(fCorr, i);
    }
    


}
