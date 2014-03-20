#include "AbstractCharacterData.h"



std::ostream& RevBayesCore::operator<<(std::ostream& o, const AbstractCharacterData& x) {
    
    o << "Origination:          " << x.getFileName() << std::endl;
    o << "Number of taxa:       " << x.getNumberOfTaxa() << std::endl;
    o << "Number of characters: " << x.getNumberOfCharacters() << std::endl;
    o << "Datatype:             " << x.getDatatype() << std::endl;
    
    return o;
}
