#include "RbConstants.h"
#include "RbException.h"
#include "StringUtilities.h"
#include "TaxonReader.h"
#include <sstream>
#include <set>

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
    int column_taxon = -1;
    int column_age = -1;
    int column_species = -1;
    
    for (size_t i = 0 ; i < line.size() ; ++i)
    {
        std::string tmp = line[i];
        StringUtilities::toLower( tmp );
        if ( tmp == "taxon" )
        {
            column_taxon = int(i);
        }
        else if ( tmp == "age" )
        {
            column_age = int(i);
        }
        else if ( tmp == "species" )
        {
            column_species = int(i);
        }
        else
        {
            throw RbException("Wrong header in the taxa definition file. It can only contain 'taxon', 'species' and 'age' fields.");
        }
    }
    
    if (column_taxon == -1)
    {
        throw RbException("Missing header in the taxa definition file. It has to contain 'taxon' field.");
    }
    
    for (size_t i = 1; i < chars.size(); ++i) //going through all the lines
    {
        const std::vector<std::string>& line = chars[i];
        Taxon t = Taxon( line[ column_taxon ] );
        
        double age = 0.0;
        if ( column_age >= 0 )
        {
            std::stringstream ss;
            ss.str( line[ column_age ] );
            ss >> age;
        }
        t.setAge( age );
        
        if ( column_species >= 0 )
        {
            t.setSpeciesName( line[ column_species ] );
        }
        
        taxa.push_back( t );
    }

    
    std::set<std::string> found;
    for(size_t i = 0; i < taxa.size(); i++)
    {
        if(found.find(taxa[i].getName()) == found.end())
        {
            found.insert(taxa[i].getName());
        }
        else
        {
            std::stringstream ss;
            ss << "Duplicate taxon name '" << taxa[i].getName() << "' encountered when reading taxa";
            throw(RbException(ss.str()));
        }
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
