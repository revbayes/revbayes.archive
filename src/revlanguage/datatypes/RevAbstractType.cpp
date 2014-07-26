#include "RevAbstractType.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;


/** Constructor from type specification */
RevAbstractType::RevAbstractType( const TypeSpec& t) :
    RevObject(),
    myTypeSpec( t )
{
}


/** Copy constructor (really not needed) */
RevAbstractType::RevAbstractType(const RevAbstractType& x) :
    RevObject( x ),
    myTypeSpec( x.myTypeSpec )
{
}


/** Generate type-safe clone */
RevAbstractType* RevAbstractType::clone(void) const
{
    return new RevAbstractType(*this);
}


/** Get type spec */
const TypeSpec& RevAbstractType::getTypeSpec( void ) const {
    
    return myTypeSpec;
}


/**
 * We represent an abstract type, so return true.
 */
bool RevAbstractType::isAbstract( void ) const
{
    return true;
}


/** Print the value (report an abstract type and then the type) */
void RevAbstractType::printValue(std::ostream &o) const {
    
    o << "<abstract object of type '" << myTypeSpec.getType() << "'";
}


