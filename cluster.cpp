#include <iostream>
#include "cluster.hpp"

using namespace std;

namespace TopoOsciSim
{

    // Create ClusterContainer on lattice
    ClusterContainer::ClusterContainer(LatticeContainer* l, const ParameterContainer& p) : 
        lattice     {l},
        start       {0},
        angle       {0},
        leftBorder  {0},
        rightBorder {0},
        size        {0},
        bondProb    {0.},
        fSize ("ClusterSize", p),
        fProb ("ClusterProb", p),
        fMeanPhiSq ("MeanPhiSq", p)
    {
        fSize.create();
        fProb.create();
        fMeanPhiSq.create();
    }

    // Return ostream for ClusterContainer class
    ostream& operator<<(ostream& out, const ClusterContainer &c)
    {

        string colorStart   = "\033[34m";
        string colorCluster = "\033[36m";
        string colorNorm    = "\033[0m";

        for (int i=0; i<c.lattice->xdim; i++)
        {
            if (i == c.start)
                out << colorStart;
            else if ( ((i >= c.leftBorder) && (i <= c.rightBorder)) ||
                      ((c.leftBorder < 0) && (i >=c.leftBorder + c.lattice->xdim)) ||
                      ((c.rightBorder > c.lattice->xdim - 1) && (i<= c.rightBorder % c.lattice->xdim)) )
                out << colorCluster;
            out << " - ";
            out << c.lattice->tslice[i];
            out << colorNorm;
        }
        out << colorNorm << " - ";


        return out;
    }    

    // Return probability for bond between index and next timestep
    double ClusterContainer::getBondProbability(int index)
    {
        return 1 - exp( -2 * (lattice->I / lattice->a)
                        * cos( angle - lattice->tslice[index].phi )
                        * cos( angle - lattice->tslice[lattice->tslice[index].idAfter].phi ) );   
    }

    // Create cluster on lattice (find leftBorder and rightBorder)
    void ClusterContainer::createCluster(mt19937_64& seed)
    {
        // create distribution to choose random number
        uniform_real_distribution< > dist(0,1);
        double r, prob=0., probAdd=0.;

        // go to start
        int i = lattice->tslice[start].id;

        // var to check overflow of cluster
        int overflow = 0;

        // go right
        do 
        {
            // add up bond probabilities of bonds inside cluster
            probAdd += prob;
            
            // go one step right
            i = lattice->tslice[i].idAfter;

            // check overflow of cluster
            if (i == 0) overflow = 1;
                        
            // choose random number
            r = dist(seed);
            
            // probability for i-1 to i beeing a bond
            prob = getBondProbability(lattice->tslice[i].idBefore);
        }
        while ((r <= prob) && (i != start ));

        // make sure that overflow is right
        if (i == 0) overflow = 0;

        // last accepted tslice id is right border of cluster        
        rightBorder = lattice->tslice[i].idBefore + overflow * lattice->xdim;

        // go back to start
        i = lattice->tslice[start].id;
        overflow = 0;

        prob=0.;
        // go left
        do 
        {
            // add up bond probabilities of bonds inside cluster
            probAdd += prob;

            // go one step left
            i = lattice->tslice[i].idBefore;            

            // check overflow of cluster
            if (i == lattice->xdim - 1) overflow = 1;

            // choose random number
            r = dist(seed);
            
            // probability for i to i+1 beeing a bond
            prob = getBondProbability(lattice->tslice[i].id);
        }
        while ((r <= prob) && (i != rightBorder % lattice->xdim ));
        
        // make sure that overflow is right
        if (i == lattice->xdim - 1) overflow = 0;

        // last accepted tslice id is left border of cluster        
        leftBorder = lattice->tslice[i].idAfter - overflow * lattice->xdim;
        // cout << "left = " << leftBorder << endl;

        // set cluster variables
        size = rightBorder + 1 - leftBorder;
        if (size > 1)
            bondProb = probAdd / (double)(size-1);
        else
            bondProb = 0;
    }
    
    // Perform one cluster step (create cluster and flip in cluster)
    void ClusterContainer::doStep(mt19937_64& seed)
    {
        // choose random reflection vector (in our case just an angle)
        uniform_real_distribution< > dist_angle( 0 , 2*M_PI );
        angle = dist_angle(seed);
    
        //choose random start point in lattice
        uniform_int_distribution< > dist_latticePoint( 0 , lattice->xdim-1 );
        start = dist_latticePoint(seed);

        // create cluster
        createCluster(seed);

        // project all points inside cluster
        int index;
        for (int i=leftBorder; i<=rightBorder; i++)
        {
            index = (i+lattice->xdim) % lattice->xdim;
            lattice->tslice[index].phi = lattice->tslice[index].getProjectedAngle(angle);
        }

        lattice->algorithm = 'c';
    }

    void ClusterContainer::doStep(mt19937_64& seed, double deltaIn)
    {
        doStep(seed);
    }

    void ClusterContainer::writeInfosToFile()
    {
        fSize.printValueToFile(size);
        fProb.printValueToFile(bondProb);
        fMeanPhiSq.printValueToFile(lattice->meanPhiSq);
    }

} // TopoOsciSim
