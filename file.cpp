#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <cmath>
#include "file.hpp"

using namespace std;

namespace TopoOsciSim
{
    FileExtension::FileExtension(const string& filetype, const ParameterContainer& p) :
        fullExtension ("")
    {
        fillExtension(filetype, p);
    }

    void FileExtension::fillExtension(const string& filetype, const ParameterContainer& p)
    {
        if (filetype != "Conf")
        {
            addToExtension(p.runName);
            if (p.minNormConst > 1E-20)
            {
                stringstream normConstAdditionStream;
                normConstAdditionStream << scientific << p.minNormConst;
                string normConstAddition = normConstAdditionStream.str();
                addToExtension("minC" + normConstAddition);
            }
        }
        
        addToExtension("I", p.I);
        if ((filetype != "Conf") && (fabs(p.theta) > 1E-15))
            addToExtension("theta", p.theta);
        
        addToExtension("a", p.a);
        addToExtension("xdim", p.xdim);
        
        if (p.equilibrationAlgorithm == "metropolis")
        {
            addToExtension("Metro");
            addToExtension("delta", p.deltaMetro);
        }
        else if (p.equilibrationAlgorithm == "cluster")
        {
            addToExtension("Cluster");
        }

        if (filetype != "Conf")
            if (p.Nsym >= 0)
                addToExtension("Nsym", p.Nsym);
    }
    
    void FileExtension::addToExtension(const string& variable, const int& value)
    {
        fullExtension.append("_" + variable + to_string(value));
    }

    void FileExtension::addToExtension(const string& variable, const double& value)
    {
        fullExtension.append("_" + variable + to_string(value));
    }

    void FileExtension::addToExtension(const string& variable)
    {
        fullExtension.append("_" + variable);
    }


    FileName::FileName(const string& filetype, const string& directory, const ParameterContainer& p) :
        type      { filetype },
        extension ( filetype, p ),
        directory { directory },
        index     { p.fileId }
    {
        fillFileName();
    }

    void FileName::fillFileName()
    {
        checkAndCreateDirectory();
        addIdToExtension();
        createNameString();
    }
    
    void FileName::checkAndCreateDirectory()
    {

        struct stat st = {0};
        if (stat(directory.c_str(), &st) == -1)
        {
            mkdir(directory.c_str(), 0777);
            // cout << "Directory " << directory << " created." << endl;
        }
        // else
            // cout << "Directory " << directory << " not created." << endl;

    }

    void FileName::addIdToExtension()
    {
        if (index == -1)
            getNextFreeIndex();
        extension.addToExtension("id", index);        
    }

    void FileName::getNextFreeIndex ()
    {
        string dummyName;
        int i = 0;
        
        dummyName = fullName + "_id" + to_string(i);
        while (exist(dummyName))
        {
            i++;
            dummyName = fullName + "_id" + to_string(i);
        }
        index = i;
    }

    bool FileName::exist(string filenameTest)
    {
        ifstream fTest(filenameTest);
        return (fTest.good());
    }
    
    void FileName::createNameString()
    {
            fullName = directory + type + extension.fullExtension;
    }

    
    File::File(const string filetype, const string directory, const ParameterContainer& p) :
        name      (filetype, directory, p)
    {}

    ostream& operator<< (ostream& out, const File& file)
    {
        out << file.name.fullName;
        return out;   
    }
    
    int File::removeStream()
    {
        int err = (remove(name.fullName.c_str()));
        if (!err)
            cout << "File " << name.fullName << " deleted." << endl;
        return err;
    }

    void File::checkStream()
    {
        if (!f.good())
        {
            cerr << "ERROR: File " << name.fullName << " has some problems." << endl;
            exit(0);
        }
                    
    }

        File::~File(void)
    {
        checkStream();
        f.close();
    }

    

    //----- FileObs ----------
    // Constructor
    FileObs::FileObs(const string filetype, const ParameterContainer& p) :
        File(filetype, p.outputDirectory, p) {}
    
    // Create file
    void FileObs::create	()
    {
        f.open(name.fullName, ios::out);
        if(f.is_open());
            //cout << "File " << name << " created." << endl;
        else
        {
            cerr << "ERROR: File creation of " << name.fullName << " does not work" << endl;
            exit(0);
        }

    }

    // Open file
    void FileObs::open	()
    {
        if (name.exist(name.fullName))
            f.open(name.fullName, ios::in | ios::app);
        else
            f.open(name.fullName, ios::in);
        if(f.is_open());
            //cout << "File " << name << " opend for reading." << endl;
        else
        {
            cerr << "ERROR: File " << name.fullName << " cannot be opened" << endl;
            exit(0);
        }
    }
    
    void FileObs::printValueToFile(double value)
    {
        f << value << endl;
    }

    void FileObs::printValueToFile(int value)
    {
        f << value << endl;
    }

    void FileObs::printIndexAndValueToFile(int index, double value)
    {
        f << index << "\t" << value << endl;
    }

    void FileObs::printIndexAndStringToFile(int index, string value)
    {
        f << index << "\t" << value << endl;
    }

    void FileObs::includeSeperationBetweenMeasurements()
    {
        f << "\n\n" << endl;
    }
    
    FileConfig::FileConfig(const ParameterContainer& p) :
        File("Conf", p.configDirectory, p) {}
    
    // Create file
    void FileConfig::create	()
    {
        f.open(name.fullName, ios::out | ios::binary);
        if(f.is_open());
            //cout << "File " << name << " created." << endl;
        else
        {
            cerr << "ERROR: File creation of " << name.fullName << " does not work" << endl;
            exit(0);
        }

    }

    // Open file
    void FileConfig::open	()
    {
        f.open(name.fullName, ios::in | ios::binary);
        if(f.is_open());
            //cout << "File " << name << " opend for reading." << endl;
        else
        {
            cerr << "ERROR: File " << name.fullName << " cannot be opened" << endl;
            exit(0);
        }
    }
    
} // namespace
