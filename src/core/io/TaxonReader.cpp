#include "RbConstants.h"
#include "RbException.h"
#include "StringUtilities.h"
#include "TaxonReader.h"


using namespace RevBayesCore;


/**
 * Constructor. Here we read in immediately the file and then we parse through each line 
 * and extract the taxon information.
 *
 * \param[in]     fn       The name of the file where the data is stored.
 * \param[in]     delim    The delimiter between the columns.
 */
TaxonReader::TaxonReader(const std::string &fn, char delim) : DelimitedDataReader( fn, delim )
{
    
    //Reading the header
    std::vector<std::string>& line = chars[0];
    std::vector<size_t> columnIndices (line.size(), RbConstants::Size_t::nan);
    
    for (size_t i = 0 ; i < line.size() ; ++i)
    {
        std::string tmp = line[i];
        StringUtilities::toLower( tmp );
        if ( tmp == "taxon" )
        {
            columnIndices[0] = i;
        }
        else if ( tmp == "species" )
        {
            columnIndices[1] = i;
        }
        else if ( tmp == "date")
        {
            columnIndices[2] = i;
        }
        else
        {
            throw RbException("Wrong header in the taxa definition file. It should contain 'Species','Taxon', 'Date' fields.");
        }
    }
    if (columnIndices[0] == RbConstants::Size_t::nan)
    {
//        throw RbException("Wrong header in the taxa definition file. It should contain 'Species','Taxon', 'Date' fields.");
    }
    
    for (size_t i = 1; i < chars.size(); ++i) //going through all the lines
    {
        const std::vector<std::string>& line = chars[i];
        Taxon t = Taxon(line[ columnIndices[0] ]);
                
        if ( columnIndices[2] != RbConstants::Size_t::nan)
        {
            TimeAndDate d = TimeAndDate( ); // line[ columnIndices[2] ]
            t.setDate( d );
        }
        if ( columnIndices[1] != RbConstants::Size_t::nan)
        {
            t.setSpeciesName( line[columnIndices[1] ] );
        }
        taxa.push_back( t );
    }
    
}


/**
 * Get the taxon information read from the file.
 *
 * \return The vector of taxa.
 */
const std::vector<Taxon>& TaxonReader::getTaxa( void ) const
{
    
    return taxa;
}
