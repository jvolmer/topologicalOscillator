#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <cstdio>
#include <fstream>
#include "parameters.hpp"

using namespace std;

namespace TopoOsciSim
{

    class FileExtension
    {
    public:
        string fullExtension;

        FileExtension(const string& filetype, const ParameterContainer& p);
        void fillExtension(const string& filetype, const ParameterContainer& p);
        void addToExtension(const string& variable, const int& value);
        void addToExtension(const string& variable, const double& value);
        void addToExtension(const string& variable);
    };

    class FileName
    {
    public:
        string fullName;
        string type;
        FileExtension extension;
        string directory;
        int index;

        FileName(const string& filetype, const string& directory, const ParameterContainer& p);
        void fillFileName();
        void checkAndCreateDirectory();
        void addIdToExtension();
        void getNextFreeIndex();
        bool exist(string filenameTest);
        void createNameString();
    };
    
        
    class File
    {
      
    public:

        FileName name;
        fstream f;

        File();
        ~File();
        File(const string filetype, const string directory, const ParameterContainer& p);
        friend ostream& operator<< (ostream& out, const File& file);
        int removeStream();
        void checkStream();
    };

    
    // Observable file
    class FileObs : public File
    {
    public:
        FileObs(const string filetype, const ParameterContainer& p);
        void create ();
        void open ();
        void printValueToFile(double value);
        void printValueToFile(int value);
        void printIndexAndValueToFile(int index, double value);
        void printIndexAndStringToFile(int index, string value);
        void includeSeperationBetweenMeasurements();
    };
    
    // Configuration file
    class FileConfig : public File
    {
    public:
        FileConfig(const ParameterContainer& p);
        void create ();
        void open ();
    };

} // namespace

#endif // FILE_H
