#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <iostream>

using namespace std;

namespace TopoOsciSim
{
    
    class ParameterContainer
    {

    public: 

        string runName;
        
        // Angular momentum
        double I;
        
        double theta;
        
        // Lattice spacing
        double a;

        // Number of lattice points (in x direction)
        int xdim;

        // Number of simulation steps (MC or Symm)
        int Nsteps;
        int NPermutationSets;
        
        // Number of symmetrizations
        int Nsym;

        double minNormConst;
        
        // Path to folder to read from and write to
        string configDirectory;
        string outputDirectory;
    
        // Number of thermalization steps
        int Nthermal;
        
        // metropolis delta
        string equilibrationAlgorithm;
        double deltaMetro;
        
        // Name of configuration file to use
        string configFile;
    
        // Configuration number to read
        int fileId;

        // How verbose should the output be
        int verbosity;

        // Create paramter container
        ParameterContainer();

        /**
           Return ostream for ParameterContainer class
           
           @param out Ostream where output goes
           @param p   This class
           @return    Ostream including p
        */
        friend ostream& operator<<   (ostream& out, const ParameterContainer& p);

        /**
           Comparing all parameter variables

           @param p2 ParameterObject to compare with
           @return   true if all variables are same, otherwise false
        */
        bool operator==(const ParameterContainer& p2);
        
        /**
           Prints out input options for the parameters.
           
           @param argv0  Name of executable
        */
        void showOptions (const string& argv0);

        /**
           Check whether to read input parameters from
           file or command line and do it

           @param argc   Number of input strings from cmd line
           @param argv[] Input strings from cmd line
        */
        void readInput (int argc, char *argv[]);

        /**
           Read input parameters from file and process them

           @param inFile Name of parameter input file
        */
        void readInputFile(char *inFile);

        void extractAndProcessParametersFromFile(ifstream& f);
        void extractAndProcessParameterFromLine(string& line);
        string removeSubstringFrom(string& s, string p) ;
        
        /**
           Read input parameters from command line and process them

           @param argc   Number of input strings from cmd line
           @param argv[] Input strings from cmd line
        */            
        void readInputCommand(int argc, char **argv);
        
        /**
           Process one parameter

           @param name  Name of parameter
           @param value Value of parameter
        */
        void process (string name, string value);
    
  };
  
} // namespace

#endif // PARAMETERS_H
