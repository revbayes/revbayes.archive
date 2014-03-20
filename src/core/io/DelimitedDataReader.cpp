#include "RbFileManager.h"
#include "DelimitedDataReader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace RevBayesCore;

DelimitedDataReader::DelimitedDataReader(const std::string &fn, char d) : 
    filename(fn), 
    delimiter(d),
    chars()
{
    
    readData();
    
}

void DelimitedDataReader::readData(void)
{
    
    std::vector<std::string> tmpChars;
    
    // open file
    std::ifstream readStream;
    RbFileManager* f = new RbFileManager(filename);
    if (!f->openFile(readStream))
        std::cout << "ERROR: Could not open file " << filename << "\n";
    
    chars.clear();
    
    // read file
    // bool firstLine = true;
    std::string readLine = "";
    while (std::getline(readStream,readLine))
    {
        std::string field = "";
        std::stringstream ss(readLine);

        int pos = 0;
        while (std::getline(ss,field,delimiter))
        {
            tmpChars.push_back(field);
            pos++;
        };
        chars.push_back(tmpChars);
        tmpChars.clear();
    };
    
}

const std::vector<std::vector<std::string> >& DelimitedDataReader::getChars(void)
{
    return chars;
}
