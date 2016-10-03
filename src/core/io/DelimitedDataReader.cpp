#include "DelimitedDataReader.h"
#include "RbFileManager.h"
#include "RbException.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace RevBayesCore;

DelimitedDataReader::DelimitedDataReader(const std::string &fn, char d, size_t lines_skipped) :
    filename(fn), 
    delimiter(d),
    chars()
{

    readData( lines_skipped );
    
}

void DelimitedDataReader::readData( size_t lines_to_skip )
{
    
    std::vector<std::string> tmpChars;
    
    // open file
    std::ifstream readStream;
    RbFileManager f = RbFileManager(filename);
    if ( f.openFile(readStream) == false )
    {
        throw RbException( "Could not open file " + filename );
    }
    
    chars.clear();
    
    // read file
    // bool firstLine = true;
    std::string read_line = "";
    size_t lines_skipped = 0;
    while (std::getline(readStream,read_line))
    {
        ++lines_skipped;
        if ( lines_skipped <= lines_to_skip)
        {
            continue;
        }
        
        std::string field = "";
        std::stringstream ss(read_line);

        int pos = 0;
        while ( std::getline(ss,field,delimiter) )
        {
            tmpChars.push_back(field);
            pos++;
        };
        chars.push_back(tmpChars);
        tmpChars.clear();
    };
    
    f.closeFile( readStream );
}

const std::vector<std::vector<std::string> >& DelimitedDataReader::getChars(void)
{
    return chars;
}


const std::string& DelimitedDataReader::getFilename(void)
{
    return filename;
}
