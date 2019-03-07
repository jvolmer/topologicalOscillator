#ifndef LATTICEEQUILIBRATION_H
#define LATTICEEQUILIBRATION_H

using namespace std;

namespace TopoOsciSim
{
    // abstract class
    class LatticeEquilibration
    {   
    public:        
        virtual void doStep(mt19937_64& seed, double deltaIn) = 0;
        virtual void doStep(mt19937_64& seed) = 0;
        virtual void writeInfosToFile() = 0;
    };
    
} // TopoOsciSim

#endif
