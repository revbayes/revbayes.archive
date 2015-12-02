#include "DelimitedDataReader.h"
#include "RbFileManager.h"
#include "RbException.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace RevBayesCore;

DelimitedDataReader::DelimitedDataReader(const std::string &fn, char d, size_t linesSkipped) :
    filename(fn), 
    delimiter(d),
    chars()
{

    readData( linesSkipped );
    
}

void DelimitedDataReader::readData( size_t linesToSkipped )
{
    
    std::vector<std::string> tmpChars;
    
    // open file
    std::ifstream readStream;
    RbFileManager* f = new RbFileManager(filename);
    if ( !f->openFile(readStream) )
    {
        throw RbException( "Could not open file " + filename );
    }
    
    chars.clear();
    
    // read file
    // bool firstLine = true;
    std::string readLine = "";
    size_t linesSkipped = 0;
    while (std::getline(readStream,readLine))
    {
        ++linesSkipped;
        if ( linesSkipped <= linesToSkipped)
        {
            continue;
        }
        
        std::string field = "";
        std::stringstream ss(readLine);

        int pos = 0;
        while ( std::getline(ss,field,delimiter) )
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


const std::string& DelimitedDataReader::getFilename(void)
{
    return filename;
}
