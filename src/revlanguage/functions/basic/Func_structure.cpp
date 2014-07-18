#include "Argument.h"
#include "ArgumentRule.h"
#include "DagNode.h"
#include "Func_structure.h"
#include "RbException.h"
#include "RlUserInterface.h"
#include "RlUtils.h"
#include "TypeSpec.h"

using namespace RevLanguage;

/** Default constructor */
Func_structure::Func_structure( void ) : Function() {
    
}


/** Clone object */
Func_structure* Func_structure::clone( void ) const {
    
    return new Func_structure( *this );
}


/** Execute function */
RevPtr<Variable> Func_structure::execute( void ) {
    
    std::ostringstream o;

    o << std::endl;
    o << "_variable     = " << args[0].getVariable()->getName() << " <" << args[0].getVariable() << ">" << std::endl;

    args[0].getVariable()->getRevObject().printStructure( o );
    o << std::endl;

    RBOUT( o.str() );

    // no return value
    return NULL;
}


/** Get argument rules */
const ArgumentRules& Func_structure::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "x", true, RevObject::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_structure::getClassType(void) { 
    
    static std::string rbClassName = "Func_type";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_structure::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_structure::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_structure::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RlUtils::Void;
    
    return returnTypeSpec;
}

