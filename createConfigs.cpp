/**
   TopoOsciSim
   createConfigs.cpp
   Purpose: Simulate the Topological Oscillator with Metropolis to create configurations.

   @author Julia Volmer
   @version 1.0 
*/

#include <iostream>
#include <random>
#include "parameters.hpp"
#include "file.hpp"
#include "timestep.hpp"
#include "lattice.hpp"
#include "latticeEquilibrationFactory.hpp"
 
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

    if (parameters.verbosity > 2)
    {
        cout << endl;
        cout << "----------------------------------------------" << endl;
        cout << "TOPOLOGICAL OSCILLATOR SILMULATION       " << endl;
        cout << endl;
        cout << "     Create Configurations  " << endl;
        cout << "----------------------------------------------" << endl;
        cout << parameters << endl;
    }

    // set lattice
    TopoOsciSim::LatticeContainer lattice(parameters);
    lattice.setPeriodicBoundaries();
    lattice.setRandom(generator);
    
    TopoOsciSim::LatticeEquilibration *latticeEquilibration = TopoOsciSim::NewLatticeEquilibrationFor(&lattice, parameters);
    
    
    // do MC thermalization
    if (parameters.verbosity > 5) cout << "Thermalization ... " << endl;

    for (int k = 0; k<parameters.Nthermal; k++)
    {
        if (parameters.verbosity > 5) cout << "\r\t\t\t" << k + 1;

        latticeEquilibration->doStep(generator, 0.5);
    }
    if (parameters.verbosity > 5) cout << endl << "\t\t\t\t ... finished" << endl << endl;

    TopoOsciSim::FileConfig Conf(parameters);
    Conf.create();

    // write header to file
    lattice.dumpHeader(Conf);

    // do MC
    if (parameters.verbosity > 5) cout << "Create Configurations ... " << endl;

    for (int k = 0; k<parameters.Nsteps; k++)
    {
        if (parameters.verbosity > 5) cout << "\r\t\t\t" << k + 1;

        // for (int j=0; j<10;j++)
        latticeEquilibration->doStep(generator);
        
        // write configuration to file
        lattice.mod2Pi();
        lattice.computeMeanPhiSq();
        
        latticeEquilibration->writeInfosToFile();

        if (parameters.Nthermal > 0)
            lattice.dumpConf(Conf);
    }
    if (parameters.verbosity > 5) cout << endl << "\t\t\t\t ... finished" << endl;

}
