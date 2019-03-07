#ifndef CLUSTER_H
#define CLUSTER_H

#include <iostream>
#include <random>
#include "lattice.hpp"
#include "latticeEquilibration.hpp"

using namespace std;

namespace TopoOsciSim
{

    class ClusterContainer : public LatticeEquilibration
    {
        
    public:
        
        // Pointer to LatticeContainer
        LatticeContainer* lattice;
        
        // Starting position of cluster algorithm on the lattice
        int start;

        // Projection angle
        double angle;

        // Left border of cluster
        int leftBorder;

        // Right border of cluster
        int rightBorder;

        // clusterSize
        int size;

        // mean cluster bond probability
        double bondProb;

        FileObs fSize;
        FileObs fProb;
        FileObs fMeanPhiSq;
        
        /**
           Create ClusterConainer on lattice

           @param l pointer to LatticeContainer
        */
        ClusterContainer(LatticeContainer* l, const ParameterContainer& p);

        /**
           Return ostream for ClusterContainer class
           
           @param out Ostream where output goes
           @param l   This class
           @return    Ostream including l
        */
        friend ostream& operator<<(ostream& out, const ClusterContainer &l);

        /**
           Return probability for bond between index and next timestep

           @param id  Lattice index
           @return Probability
        */
        double getBondProbability(int id);

        /**
           Create cluster on lattice (find leftBorder and
           rightBorder)

           @param seed Seed number
        */
        void   createCluster(mt19937_64& seed);

        /**
           Perform one cluster step (create cluster and flip in
           cluster)

           @param seed Seed number
        */
        void doStep(mt19937_64& seed);

        void doStep(mt19937_64& seed, double deltaIn);

        void writeInfosToFile();
    };    


} // TopoOsciSim

#endif // CLUSTER_H
