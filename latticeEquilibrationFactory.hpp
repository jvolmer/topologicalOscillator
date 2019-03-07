#ifndef LATTICEEQUILIBRATIONFACTORY_H
#define LATTICEEQUILIBRATIONFACTORY_H

#include "latticeEquilibration.hpp"
#include "metropolis.hpp"
#include "cluster.hpp"

using namespace std;

namespace TopoOsciSim
{
    
    LatticeEquilibration *NewLatticeEquilibrationFor(
        LatticeContainer *l,
        const ParameterContainer& p)
    {
        if (p.equilibrationAlgorithm == "metropolis")
        {
            return new MetropolisContainer(l, p);
        }
        else if (p.equilibrationAlgorithm == "cluster")
        {
            return new ClusterContainer(l, p);
        }
        else
        {
            return NULL;
        }
    }
    
} // TopoOsciSim

#endif
