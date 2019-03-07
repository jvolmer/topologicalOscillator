#include <iostream>
#include "metropolis.hpp"

using namespace std;

namespace TopoOsciSim
{

    // Create MetropolisContainer on lattice
    MetropolisContainer::MetropolisContainer(LatticeContainer* l, const ParameterContainer& p) : 
        lattice     {l},
        delta       {p.deltaMetro},
        acceptance  (0.),
        fAcc        ("MetropolisAcc", p),
        fMeanPhiSq  ("MeanPhiSq", p)
    {
        fAcc.create();
        fMeanPhiSq.create();
    }
    
    
    // Return ostream for MetropolisContainer class
    ostream& operator<<(ostream& out, const MetropolisContainer &c)
    {
        out << *(c.lattice);
    
        return out;
    }
    
    void MetropolisContainer::doStep(mt19937_64& seed, double deltaIn)
    {
        uniform_real_distribution< > dist_newPhi( 0 , 1 );
        uniform_real_distribution< > dist_metro(0 , 1 );
        
        int acceptCount = 0;
        double phiOld, phiNew, deltaS;
        double r, r2;
        for (int i=0; i<lattice->xdim; i++)
        {
            r = 2*dist_newPhi(seed) - 1;
            
            phiOld = lattice->tslice[i].phi;
            phiNew = lattice->tslice[i].phi + deltaIn * r;
      
            deltaS = lattice->getLocalAction(i, phiNew) - lattice->getLocalAction(i, phiOld);
            
            r2 = dist_metro(seed);
            
            if (r2 <= exp(-deltaS))
            {
                lattice->tslice[i].phi = phiNew;
                acceptCount += 1;

            }

        }
        lattice->algorithm = 'm';
        acceptance = acceptCount / (double) lattice->xdim;
    }

    void MetropolisContainer::doStep(mt19937_64& seed)
    {
        doStep(seed, delta);
    }
    
    void MetropolisContainer::writeInfosToFile()
    {
        fAcc.printValueToFile(acceptance);
        fMeanPhiSq.printValueToFile(lattice->meanPhiSq);
    }
    

} // TopoOsciSim
