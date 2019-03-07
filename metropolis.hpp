#ifndef METROPOLIS_H
#define METROPOLIS_H

#include <iostream>
#include <random>
#include "lattice.hpp"
#include "latticeEquilibration.hpp"

using namespace std;

namespace TopoOsciSim
{

    class MetropolisContainer : public LatticeEquilibration
    {
        
    public:
        
        // Pointer to LatticeContainer
        LatticeContainer* lattice;

        // vicinity of old link to look for new one
        double delta;

        double acceptance;
        FileObs fAcc;
        FileObs fMeanPhiSq;
        
        /**
           Create ClusterConainer on lattice

           @param l pointer to LatticeContainer
        */
        MetropolisContainer(LatticeContainer* l, const ParameterContainer& p);

        /**
           Return ostream for ClusterContainer class
           
           @param out Ostream where output goes
           @param l   This class
           @return    Ostream including l
        */
        friend ostream& operator<<(ostream& out, const MetropolisContainer &l);

        void doStep(mt19937_64& seed, double deltaIn);

        void doStep(mt19937_64& seed);

        void writeInfosToFile();        
    };

    
} // TopoOsciSim

#endif // METROPOLIS_H
