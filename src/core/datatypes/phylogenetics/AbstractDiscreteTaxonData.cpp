#include "AbstractDiscreteTaxonData.h"


std::ostream& RevBayesCore::operator<<(std::ostream& o, const AbstractDiscreteTaxonData& x)
{
    
    o << x.getTaxonName() << ":" << std::endl;
    for (size_t i = 0; i < x.getNumberOfCharacters(); ++i)
    {
        o << x[i];
    }
    o << std::endl;
    
    return o;
}