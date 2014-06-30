#include "RlDistribution.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;


/**
 * Default constructor.
 * 
 * The default constructor does nothing except allocating the object.
 */
Distribution::Distribution() : RevObject() 
{
    
}


/**
 * Default destructor.
 * 
 * The default destructor does nothing except deleting the object.
 */
Distribution::~Distribution() 
{
    
}


/**
 * Get class name of object 
 *
 * \return The class' name.
 */
const std::string& Distribution::getClassName(void) 
{ 
    
    static std::string rbClassName = "Distribution";
    
	return rbClassName; 
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& Distribution::getClassTypeSpec(void) 
{ 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return rbClass; 
}


/**
 * Print the value of this object for the user.
 *
 * There is not much of a value to print for a distribution. 
 * Thus, we simply print the name of the distribution.
 *
 * \param[in]    the stream to which to print.
 */
void Distribution::printValue(std::ostream &o) const 
{
    o << getClassName() << "(...)" << std::endl;
}
