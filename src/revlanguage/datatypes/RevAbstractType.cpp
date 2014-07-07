#include "RevAbstractType.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

RevAbstractType::RevAbstractType( const TypeSpec& t) : RevObject(), myTypeSpec( t )
{
}


RevAbstractType::RevAbstractType(const RevAbstractType& x) : RevObject( x ), myTypeSpec( x.myTypeSpec )
{
}


RevAbstractType* RevAbstractType::clone(void) const
{
    return new RevAbstractType(*this);
}


/** Get type spec */
const TypeSpec& RevAbstractType::getTypeSpec( void ) const {
    
    return myTypeSpec;
}



/** Print the value (report an abstract type and then the type) */
void RevAbstractType::printValue(std::ostream &o) const {
    
    o << "<abstract object of type: " << myTypeSpec.getType();
}


