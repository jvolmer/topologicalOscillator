/**
   TopoOsciSim
   computeTopoSuscMC.cpp
   Purpose: Compute Topological Susceptibility from created MC configurations

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
    // define and initialize parameters
    TopoOsciSim::ParameterContainer parameters;

    // process command line input
    parameters.readInput(argc, argv);
    parameters.Nthermal = -1;
    parameters.Nsym = -1;

    if (parameters.verbosity > 2)
    {
        cout << endl;
        cout << "----------------------------------------------" << endl;
        cout << "TOPOLOGICAL OSCILLATOR SILMULATION            " << endl;
        cout << endl;
        cout << "     Cluster  --  Compute Topological Charge  " << endl;
        cout << "----------------------------------------------" << endl;
        cout << parameters << endl;
    }
    
    // open config file
    TopoOsciSim::FileConfig fConf(parameters);
    fConf.open();
    
    // set lattice
    TopoOsciSim::LatticeContainer lattice(parameters);
    
    // read configuration constants
    lattice.readHeader(fConf);

    TopoOsciSim::FileObs fCharge("Q", parameters);
    fCharge.create();

    TopoOsciSim::FileObs fPlaquette("Plaq", parameters);
    fPlaquette.create();

    TopoOsciSim::FileObs fCorrelation("Corr", parameters);
    fCorrelation.create();

    TopoOsciSim::FileObs fS("S", parameters);
    fS.create();

    // complex<double> plaqComplex;
    double plaq;
    double link=0., link2=0.;
    // read configuration
    if (parameters.verbosity > 5) cout << "Read Configuration and Compute Q ..." << endl;
    for (int i=0; i<parameters.Nsteps; i++)
    {
        if (parameters.verbosity > 5) cout << "\r\t\t\t" << i + 1;
        
        // read conf and check for error (e.g. eof)
        if (!lattice.readConf(fConf))
            break;

        
        // compute topological charge
        lattice.computeQ();
        fCharge.f << lattice.q << endl;
        fS.f << lattice.getAction() << endl;

        // compute link
        lattice.mod2Pi();
        plaq = lattice.computePlaquette();
        fPlaquette.f << plaq << endl;
        link += plaq;
        link2 += plaq*plaq;

    }
    if (parameters.verbosity > 5) cout << endl << "\t\t\t\t ... finished" << endl;
    link /= (double)parameters.Nsteps;
    link2 /= (double)parameters.Nsteps;
    cout << "link = " << link << " +- " << sqrt((link2 - link*link)/(double)(parameters.Nsteps)) << endl;
}
