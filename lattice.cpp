#include <iostream>
#include "lattice.hpp"

using namespace std;

namespace TopoOsciSim
{
    
    LatticeContainer::LatticeContainer(const double& IIn, const double& aIn, const int& xdimIn, const double& thetaIn) :
        I          { IIn    }, 
        a          { aIn    },
        xdim       { xdimIn },
        theta      { thetaIn},
        algorithm  { '\0'   },
        boundary   { '\0'   },
        q          { 0.     },
        meanPhiSq  { 0.     }
    {
        // allocate space
        tslice.reserve(xdim);
        corr.reserve(xdim);
        
        //initialize
        for (int i=0; i<xdim; i++)
        {
            tslice.push_back(TimestepContainer(i, 0.));
            corr.push_back(0.);
        }
    }

    // Constructor
    LatticeContainer::LatticeContainer(const double& IIn, const double& aIn, const int& xdimIn) :
        LatticeContainer(IIn, aIn, xdim, 0.) {}

    // Default Constructor
    LatticeContainer::LatticeContainer() :
        LatticeContainer(.25, .2, 10) {}

    // Constructor using ParameterContainer
    LatticeContainer::LatticeContainer(const ParameterContainer& p) :
        LatticeContainer(p.I, p.a, p.xdim, p.theta) {}

    // Copy Constructor
    LatticeContainer::LatticeContainer(const LatticeContainer &l) :
        LatticeContainer(l.I, l.a, l.xdim, l.theta)
    {
        algorithm = l.algorithm;
        boundary = l.boundary;
        q = l.q;
        tslice = l.tslice;
        corr = l.corr;

        if (boundary == 'p')
            setPeriodicBoundaries();

        for (int i=0; i<xdim; i++)
            tslice[i].phi = l.tslice[i].phi;
    }
    
    // Destructor
    LatticeContainer::~LatticeContainer()
    {    }
    
    // Return ostream for LatticeContainer class
    ostream& operator<<(ostream& out, const LatticeContainer &l)
    {        
        out << l.tslice[0].idBefore << " <-";
        for (int i=0; i<l.xdim; i++)
            out << " " << l.tslice[i] << " -";
        out << "> " << l.tslice[l.xdim-1].idAfter;
        // out << " => " << l.q;
        
        return out;
        
    }

    // Comparing whole lattice
    bool LatticeContainer::operator==(const LatticeContainer& l2)
    {
        bool result;
        double prec = 1E-12;

        // compare general lattice constants
        result = ( (I          == l2.I         ) &&
                   (a          == l2.a         ) &&
                   (xdim       == l2.xdim      ) &&
                   (theta      == l2.theta     ) &&
                   (algorithm  == l2.algorithm ) &&
                   (boundary   == l2.boundary  )
                 );

        // compare lattice
        for (int i=0; i<xdim; i++)
            result = result && ( (tslice[i].phi - l2.tslice[i].phi) <= prec);
        
        return result;
    }

    // Return timestep id on the lattice
    int LatticeContainer::getId(int i)
    {
        return (i+xdim) % xdim;
    }

    void LatticeContainer::copyContent(const LatticeContainer& l)
    {
        for (int i=0; i<xdim; i++)
            tslice[i].phi = l.tslice[i].phi;
    }
    
    //Set periodic boundary conditions on the lattice
    void LatticeContainer::setPeriodicBoundaries()
    {
        // set left boundary
        tslice[0].idBefore = xdim-1;
        // set right boundary
        tslice[xdim-1].idAfter = 0;

        // set lattice variable
        boundary = 'p';
    }
    
    // Set all timeslices to random
    void LatticeContainer::setZero()
    {
        for (int i=0; i<xdim; i++)
            tslice[i].phi = 0.;
    }

    // Set all timeslices to random
    void LatticeContainer::setRandom(mt19937_64& seed)
    {
        for (int i=0; i<xdim; i++)
            tslice[i].setRandom(seed);
    }
    
    double LatticeContainer::getActionSummand(int xpos)
    {
        return I/a * (1. - cos(tslice[tslice[xpos].idAfter].phi - tslice[xpos].phi));
    }

    double LatticeContainer::getLocalAction(int xpos)
    {
        double phiTest = tslice[xpos].phi;
        return getLocalAction(xpos, phiTest);
    }
        
    double LatticeContainer::getLocalAction(int xpos, double phiTest)
    {
        return I/a * (1. - cos (tslice[tslice[xpos].idAfter].phi - phiTest)
                    + 1. - cos (phiTest - tslice[tslice[xpos].idBefore].phi)
            );
    }

    // Return total lattice action
    double LatticeContainer::getAction()
    {
        double sum = 0.;
        
        for (int i=0; i<xdim; i++)
        {
            sum += getActionSummand(i);
            // cout << "action part " << i << " | " << getActionSummand(i) << endl;
        }
        return sum;
    }

    complex<double> LatticeContainer::getThetaAction()
    {
        computeQ();
        return (complex<double>){0,1} * theta * q * 2.*M_PI;
    }
   
    complex<double> LatticeContainer::getThetaWeight()
    {
        return exp(- getThetaAction() );
    }

    complex<double> LatticeContainer::getAlphaAction()
    {
        complex<double> alphaConst = (complex<double>){0,1} * theta * 2. * M_PI;
        complex<double> res = 0.;
        for (int t=0; t<xdim; t++)
            res += tslice[t].phi;
        return alphaConst * res;
    }

    complex<double> LatticeContainer::getAlphaWeight()
    {
        return exp(- getAlphaAction() );
    }

    // set all lattice points to mod 2pi
    void LatticeContainer::mod2Pi()
    {
        for (int i=0; i<xdim; i++)
            tslice[i].mod2Pi();
    }
    
    // Compute and set topological charge q
    void LatticeContainer::computeQ()
    {
        double sum = 0, diff, add;
        for (int i=0; i<xdim; i++)
        {
            diff = tslice[tslice[i].idAfter].phi - tslice[i].phi;
            if ( (fabs(fmod(diff - M_PI, 2*M_PI)) <= 1E-12) )
                add = M_PI;
            else
                add = diff - 2 * M_PI * round(diff / (2 * M_PI));

            sum += add;
            
        }
        q = 1./(2*M_PI) * sum;
    }

    // Compute correlation of lattice variables
    void LatticeContainer::computeCorr()
    {
        // initialize to zero
        for (int i=0; i<xdim; i++)
            corr[i] = 0.;

        // compute correlation
        for (int j=0; j<xdim; j++) // over Gamma entries
            for (int i=0; i<xdim; i++) // over seperation
                corr[j] += tslice[i].phi * tslice[getId(i+j)].phi;

        for (int i=0; i<xdim; i++)
            corr[i] /= xdim;
    }

    double LatticeContainer::computePlaquette()
    {
        double plaquette = 0.;
        for (int i=0; i<xdim; i++)
            plaquette += computeLocalPlaquetteAt(i);
        return plaquette/(double)xdim;            
    }
    
    double LatticeContainer::computeLocalPlaquetteAt(int tpos)
    {
        return cos( tslice[tslice[tpos].idAfter].phi - tslice[tpos].phi );
    }

    complex<double> LatticeContainer::computeComplexPlaquette()
    {
        complex<double> plaquette = 0.;
        for (int i=0; i<xdim; i++)
            plaquette += computeComplexLocalPlaquetteAt(i);
        return plaquette/(double)xdim;            
    }

    complex<double> LatticeContainer::computeComplexLocalPlaquetteAt(int tpos)
    {
        return exp((complex<double>){0,1} * (tslice[tslice[tpos].idAfter].phi - tslice[tpos].phi) );
    }
    
    double LatticeContainer::computeMeanPhi()
    {
        double res = 0.;
        
        for (int i=0; i<xdim; i++)
            res += tslice[i].phi;

        return res/(double)xdim;
    }

    double LatticeContainer::computeMeanPhiSq()
    {
        double res = 0.;
        
        for (int i=0; i<xdim; i++)
            res += tslice[i].phi * tslice[i].phi;

        meanPhiSq = res / (double)xdim;
        return meanPhiSq;
    }

    // Write lattice charge to file
    void LatticeContainer::dumpQ(FileObs& Out)
    {
        Out.f << q << endl;

        if (!Out.f.good())
            cerr << "ERROR in writing topological charge to " << Out.name.fullName << endl;

    }

    // Write correlation of lattice variables to file
    void LatticeContainer::dumpCorr(FileObs& Out, int iConf)
    {
        for (int j=0; j<xdim; j++)
            Out.f << iConf << "\t" << j << "\t" << corr[j] << endl;

        if (!Out.f.good())
            cerr << "ERROR in writing correlation to " << Out.name.fullName << endl;

    }

    // Write Lattice infos to file
    void LatticeContainer::dumpHeader(FileConfig& Out)
    {
        
	// IO_RND_Dump	(Out);

        // write
        Out.f.write(reinterpret_cast<char*>(&I),    sizeof(I));
        Out.f.write(reinterpret_cast<char*>(&a),    sizeof(a));
        Out.f.write(reinterpret_cast<char*>(&xdim), sizeof(xdim));
        Out.f.write(&boundary,                      sizeof(boundary));
        Out.f.write(&algorithm,                     sizeof(algorithm));

        if (Out.f.good());
            //cout << "Header written successfully to " << Out.name.fullName << endl;
        else
            cerr << "ERROR in header writing to " << Out.name.fullName << endl;
            
    }

    // Read Lattice infos from file
    void LatticeContainer::readHeader(FileConfig& In)
    {

	// IO_RND_Read	(In);

        // read
        In.f.read(reinterpret_cast<char*>(&I),    sizeof(I));
        In.f.read(reinterpret_cast<char*>(&a),    sizeof(a));
        In.f.read(reinterpret_cast<char*>(&xdim), sizeof(xdim));
        In.f.read(&boundary,                      sizeof(boundary));
        In.f.read(&algorithm,                     sizeof(algorithm));

        // set periodic boundaries of lattice
        if (boundary == 'p')
            setPeriodicBoundaries();

	if (In.f.good())
	{
            ;//cout << "Header read successfully."         << endl;
            // cout << "    I    = " << I     << endl;
            // cout << "    a    = " << a     << endl;
            // cout << "    xdim = " << xdim  << endl;
            // cout << "    boundary  = " << boundary   << endl;
            // cout << "    algorighm = " << algorithm  << endl;
	}
        else
            cerr << "ERROR in header reading from file " << In.name.fullName << endl;
    }
    
    // Write Lattice to file
    void LatticeContainer::dumpConf(FileConfig& Out)
    {
        for (int i=0; i<xdim; i++)
            Out.f.write(reinterpret_cast<char*>(&(tslice[i].phi)), sizeof(double));

        if (Out.f.good())
            ;//cout << "Configuration written successfully to " << Out.name.fullName << endl;
        else
            cerr << "ERROR in configuration writing" << endl;
        
    }

    // Read Lattice from file
    bool LatticeContainer::readConf(FileConfig& In)
    {
        int i;
        for (i=0; i<xdim; i++)
        {
            In.f.read(reinterpret_cast<char*>(&(tslice[i].phi)), sizeof(double));
            if (In.f.eof()) break;
            // cout << i << ") ... " << In.f.eof() << ", ";
        }
        
        // error messages
        if (In.f.good())
            ;//cout << "Configuration read successfully from " << In.name.fullName << endl;
        // if the whole config cannot be read
        else if ((In.f.eof()) && (i==0))
            cerr << "Regular configuration file end is reached" << endl;
        // if the config can just be read partly
        else if (In.f.eof())
        {
            cerr << "ERROR: Unregular configuration file end is reached" << endl;
            exit(0);
        }
        else
            cerr << "ERROR in Configuration reading from file " << In.name.fullName << endl;

        // return state of file
        return (In.f.good());
    }

    
} // TopoOsciSim
