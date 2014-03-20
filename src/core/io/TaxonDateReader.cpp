#include "TaxonDateReader.h"


using namespace RevBayesCore;


/**
 * Constructor. Here we read in immidiately the file and the we parse through each line 
 * and extract the taxon information.
 *
 * \param[in]     fn       The name of the file where the data is stored.
 * \param[in]     delim    The delimiter between the columns.
 */
TaxonDateReader::TaxonDateReader(const std::string &fn, char delim) : DelimitedDataReader( fn, delim )
{
    
    for (size_t i = 0; i < chars.size(); ++i) 
    {
        const std::vector<std::string>& line = chars[i];
        Taxon t = Taxon(line[0]);
        
        TimeAndDate d = TimeAndDate();
        t.setDate( d );
        
        taxa.push_back( t );
    }
    
}


/**
 * Get the taxon information read from the file.
 *
 * \return The vector of taxa.
 */
const std::vector<Taxon>& TaxonDateReader::getTaxa( void ) const
{
    
    return taxa;
}
