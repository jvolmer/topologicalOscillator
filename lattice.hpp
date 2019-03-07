#ifndef LATTICE_H
#define LATTICE_H

//#include <iostream>
#include <random>
#include <cmath>
#include <complex>
#include "file.hpp"
#include "parameters.hpp"
#include "timestep.hpp"
#include "lattice.hpp"

using namespace std;

namespace TopoOsciSim
{
    
    class LatticeContainer
    {
        
    public:

        double I;
        double a;
        int xdim;
        double theta;
        vector<TimestepContainer> tslice;
        char algorithm;
        char boundary;
        double q;
        double meanPhiSq;
        vector<double> corr;
        
        LatticeContainer(const double& IIn, const double& aIn, const int& xdimIn, const double& thetaIn);
        LatticeContainer(const double& IIn, const double& aIn, const int& xdimIn);
        LatticeContainer();
        LatticeContainer(const ParameterContainer& p);
        LatticeContainer(const LatticeContainer& l);
        ~LatticeContainer();
        
        friend ostream& operator<<(ostream& out, const LatticeContainer &l);

        bool operator==(const LatticeContainer& l2);

        void copyContent(const LatticeContainer& l);        
        int getId(int i);
        void setPeriodicBoundaries();
        void setZero();
        void setRandom(mt19937_64& seed);
        double getActionSummand(int xpos);
        double getLocalAction(int xpos);
        double getLocalAction(int xpos, double phiTest);
        double getAction();
        complex<double> getThetaAction();
        complex<double> getThetaWeight();
        complex<double> getAlphaAction();
        complex<double> getAlphaWeight();
        void mod2Pi();
        void computeQ();
        void computeCorr();
        double computePlaquette();
        double computeLocalPlaquetteAt(int tpos);
        complex<double> computeComplexPlaquette();
        complex<double> computeComplexLocalPlaquetteAt(int tpos);
        double computeMeanPhi();
        double computeMeanPhiSq();
        
        void dumpQ(FileObs& Out);
        void dumpCorr(FileObs& Out, int iConf);

        void dumpHeader(FileConfig& Out);
        void readHeader(FileConfig& In);
        void dumpConf(FileConfig& Out);
        bool readConf(FileConfig& In);
    };
    
} // TopoOsciSim

#endif // LATTICE_H
