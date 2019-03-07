#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include "parameters.hpp"

using namespace std;

namespace TopoOsciSim
{

    // Create paramter container
    ParameterContainer::ParameterContainer() :
        runName   { "test" },
        I         { 1.  },
        theta   { 0.  },
        a         { 1.  },
        xdim      { 4   },
        Nsteps    { 10000},
        NPermutationSets {1},
        Nsym      { -1   },
        minNormConst {0},
        configDirectory{ "./out/"  },
        outputDirectory{ "./out/"  },
        Nthermal  { 1000  },
        equilibrationAlgorithm{ "cluster" },
        deltaMetro{ 0.5  },
        fileId    { 0    },
        verbosity { 10   }  {}
    
    // Return ostream for ParameterContainer class
    ostream& operator<<(ostream& out, const ParameterContainer& p)
    {
        out << "\t runName      = " << p.runName << endl;
        out << "\t I            = " << p.I << endl;
        out << "\t theta        = " << p.theta << endl;
        out << "\t a            = " << p.a << endl;
        out << "\t xdim         = " << p.xdim << endl;
        out << "\t Nsteps       = " << p.Nsteps << endl;    
        out << "\t NPermutationSets = " << p.NPermutationSets << endl;    
        out << "\t Nsym         = " << p.Nsym << endl;
        out << "\t minNormConst = " << p.minNormConst << endl;
        out << "\t Nthermal     = " << p.Nthermal << endl;
        out << "\t equilibrationAlgorithm = " << p.equilibrationAlgorithm << endl;
        out << "\t delta (Metro)= " << p.deltaMetro << endl;
        out << "\t configDirectory = " << p.configDirectory << endl;    
        out << "\t outputDirectory = " << p.outputDirectory << endl;    
        out << "\t fileId       = " << p.fileId << endl;
        out << "\t verbosity    = " << p.verbosity << endl;
        
        return out;        
    }

    bool ParameterContainer::operator==(const ParameterContainer& p2)
    {
        return ( (runName    == p2.runName   ) &&
                 (I          == p2.I         ) &&
                 (theta    == p2.theta   ) &&
                 (a          == p2.a         ) &&
                 (xdim       == p2.xdim      ) &&
                 (Nsteps     == p2.Nsteps    ) &&
                 (NPermutationSets == p2.NPermutationSets) &&
                 (Nsym       == p2.Nsym      ) &&
                 (minNormConst == p2.minNormConst) &&
                 (configDirectory == p2.configDirectory) &&
                 (outputDirectory == p2.outputDirectory) &&
                 (Nthermal   == p2.Nthermal  ) &&
                 (equilibrationAlgorithm == p2.equilibrationAlgorithm) &&
                 (deltaMetro == p2.deltaMetro) &&
                 (fileId   == p2.fileId  ) &&
                 (verbosity  == p2.verbosity )
               );        
    }
    

    //Prints out input options for the parameters.
    void ParameterContainer::showOptions(const string& argv0)
    {        
        cout << endl;
        cout << "Usage: " << argv0 << " [Options]" << endl;
        cout << "Options:" << endl;
        cout << "\t --runName    <string> # Name of this run" << endl;        
        cout << "\t --I          <double> # Set moment of inertia" << endl;
        cout << "\t --theta    <double> # Set complex action" << endl;
        cout << "\t --a          <double> # Set lattice spacing" << endl;
        cout << "\t --xdim       <int>    # Set number of lattice points" << endl;
        cout << "\t --Nsteps     <int>    # Set number of repetitions" << endl;
        cout << "\t --NPermutationSets<int> # Set number of different permutations" << endl;
        cout << "\t --Nsym       <int>    # Set number of symmetrizations" << endl;
        cout << "\t --minNormConst<double># Set minimal absolute value of normalization constant" << endl;
        cout << "\t --Nthermal   <int>    # Set number of thermalization steps" << endl;
        cout << "\t --equilibrationAlgorithm <string> # Set algorithm to use for equilibration of lattice" << endl;
        cout << "\t --deltaMetro <double> # Set area where to choose next Metroplis step from" << endl;
        cout << "\t --configDirectory <string> # Choose path configuration folder for in- and output" << endl;
        cout << "\t --outputDirectory <string> # Choose path output folder for in- and output" << endl;
        cout << "\t --fileId   <int>    # Choose number of created configuration to use" << endl;
        cout << endl;   
    }

    void ParameterContainer::readInput(int argc, char **argv)
    {

        string paramName;
        string paramValue;

        // not options given
        if (argc == 1)
        {
            cout << "ERROR: No Input parameters given" << endl;
            showOptions(argv[0]);
            exit(0);
        }
        
        // input file
        else if (argc == 2)
            readInputFile(argv[1]);
        
        // else read in from command line
        else
            readInputCommand(argc, argv);
    }
    
    void ParameterContainer::readInputFile(char *inFile)
    {
        ifstream f (inFile);
        
        if (f.is_open())
        {
            extractAndProcessParametersFromFile(f);   
            f.close();
        }
        else
            cout << "Unable to open parameter file " << inFile << endl;
        
    }

    void ParameterContainer::extractAndProcessParametersFromFile(ifstream& f)
    {
        string line;
        
        while (getline(f, line))
        {
            extractAndProcessParameterFromLine(line);
        }
    }

    void ParameterContainer::extractAndProcessParameterFromLine(string& line)
    {        
        // find delimiter "tab" in line
        size_t delimPos = line.find("\t");
        line = removeSubstringFrom(line, " ");
        line = removeSubstringFrom(line, "\t");
   
        // if there is not "tab" in line: Error
        if (delimPos == string::npos)
        {
            cout << "ERROR: wrong input format in file" << endl;
            exit(0);
        }
        // if there is a "tab"
        else
        {                    
            string paramName = line.substr(0, delimPos);
            string paramValue = line.substr(delimPos);
            
            process(paramName, paramValue);
        }   
        
    }

    string ParameterContainer::removeSubstringFrom(string& s, string p) 
    {
        string::size_type n = p.length();

        for (string::size_type i = s.find(p);
             i != string::npos;
             i = s.find(p))
            s.erase(i, n);
        return s;
    }
    
    
    void ParameterContainer::readInputCommand(int argc, char **argv)
    {
        string paramName;
        string paramValue;

        // go over all input parameters
        for (int i=1; i<argc; i++)
        {
            // if next one is a new parameter
            if ( (argv[i][0] == '-') && (argv[i][1] == '-'))
            {
                // read parameter name without '--'
                paramName = argv[i] + 2;

                // if parameter has no value: Error
                if ( (argv[i+1][0] == '-') && (argv[i+1][1] == '-'))
                {
                    cout << "ERROR: wrong input format" << endl;
                    showOptions(argv[0]);
                    exit(0);
                }
                // if parameter has value
                else
                {
                    paramValue = argv[i+1];

                    // process parameter
                    process(paramName, paramValue);
                }
                
                
            }

            // we read 2 strings (paramName and paramValue)
            i++;
        }    
        
    }
    
    // Read in input parameters and process them
    void ParameterContainer::process(string name, string value)
    {
        if ( name == "runName")
        {
            runName = value;
        }

        else if ( name == "I")
        {
            I = stof(value);
        }

        else if ( name == "theta")
        {
            theta = stof(value);
        }

        else if ( name == "xdim")
        {
            xdim = stoi(value);            
        }

        else if ( name == "a")
        {
            a = stof(value);            
        }
        
        else if ( name == "Nsteps")
        {                    
            Nsteps = stoi(value);
        }        
                
        else if ( name == "NPermutationSets")
        {                    
            NPermutationSets = stoi(value);
        }        
                
        else if ( name == "Nsym")
        {                    
            Nsym = stoi(value);
        }        

        else if ( name == "minNormConst")
        {                    
            minNormConst = stof(value);
        }        

        else if ( name == "Nthermal")
        {                    
            Nthermal = stoi(value);
        }
                
        else if ( name == "equilibrationAlgorithm")
        {                    
            equilibrationAlgorithm = value;
        }

        else if ( name == "deltaMetro")
        {                    
            deltaMetro = stof(value);
        }
                
        else if (name == "configDirectory")
        {
            configDirectory = value;
        }

        else if (name == "outputDirectory")
        {
            outputDirectory = value;
        }

        else if (name == "fileId")
        {
            fileId = stoi(value);
        }

        else if (name == "verbosity")
        {        
            verbosity = stoi(value);
        }        

        
    }
    
} // namespace
