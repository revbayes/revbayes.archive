#include "ArgumentRule.h"
#include "Ellipsis.h"
#include "AbstractMemberFunction.h"
#include "RbException.h"
#include "RbUtil.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Constructor */
AbstractMemberFunction::AbstractMemberFunction(const TypeSpec retType, ArgumentRules* argRules) : Function(), 
    argumentRules(argRules), 
    object(NULL), 
    returnType(retType) 
{
    
}


/** Get class name of object */
const std::string& AbstractMemberFunction::getClassType(void) { 
    
    static std::string revClassType = "AbstractMemberFunction";
    
	return revClassType; 
}

/** Get class type spec describing type of object */
const TypeSpec& AbstractMemberFunction::getClassTypeSpec(void) { 
    
    static TypeSpec revClassTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revClassTypeSpec; 
}

/** Get type spec */
const TypeSpec& AbstractMemberFunction::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get argument rules */
const ArgumentRules& AbstractMemberFunction::getArgumentRules(void) const {
    
    return *argumentRules;
}


/** Get return type */
const TypeSpec& AbstractMemberFunction::getReturnType(void) const {
    
    return returnType;
}


void AbstractMemberFunction::setMemberObject( const RevPtr<Variable> &obj) {
    
    // we do not own the object itself because one object can have multiple member functions
    object = obj;
}



void AbstractMemberFunction::setMethodName(std::string const &name) {
 
    funcName = name;
}

