#include "RbConstants.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "StringUtilities.h"
#include "TraceContinuousReader.h"

#include <map>


using namespace RevBayesCore;


/**
 * Constructor. Here we read in immediately the file and then we parse through each line
 * and extract the TraceContinuous information.
 *
 * \param[in]     fn       The name of the file where the data is stored.
 * \param[in]     delim    The delimiter between the columns.
 */
TraceContinuousReader::TraceContinuousReader(const std::string &fn, char delim) : DelimitedDataReader( fn, delim )
{
    
    const std::vector<std::string>& headers = chars[0];
    
    size_t numSkippedCols = 1;
    
    for (size_t j=numSkippedCols; j<headers.size(); j++)
    {
        RevBayesCore::Trace t;
        
        std::string parmName = headers[j];
        t.setParameterName(parmName);
        t.setFileName(fn);
        
        data.push_back( t );
    }
    
    for (size_t i = 1; i < chars.size(); ++i) //going through all the lines
    {
        const std::vector<std::string>& columns = chars[i];
        
        // adding values to the Tracess
        for (size_t j=numSkippedCols; j<columns.size(); j++)
        {
            Trace& t = data[j-numSkippedCols];
            std::string tmp = columns[j];
            double d = atof( tmp.c_str() );
            t.addObject(d);
        }
        
    }

    
}


/**
 * Get the TraceContinuous information read from the file.
 *
 * \return The vector of taxa.
 */
std::vector<Trace>& TraceContinuousReader::getTraces( void )
{
    
    return data;
}


/**
 * Get the TraceContinuous information read from the file.
 *
 * \return The vector of taxa.
 */
const std::vector<Trace>& TraceContinuousReader::getTraces( void ) const
{
    
    return data;
}
