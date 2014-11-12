#include "Func_printSeed.h"
#include "ArgumentRule.h"
#include "DeterministicNode.h"
#include "Ellipsis.h"
#include "Natural.h"
#include "RandomNumberGenerator.h"
#include "RandomNumberFactory.h"
#include "RbUtil.h"
#include "RevNullObject.h"
#include "RealPos.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include <ostream>

using namespace RevLanguage;

Func_printSeed::Func_printSeed() : Procedure() {
    
}

/* Clone object */
Func_printSeed* Func_printSeed::clone( void ) const {
    
    return new Func_printSeed( *this );
}


/** Execute function: We rely on getValue and overloaded push_back to provide functionality */
RevPtr<RevVariable> Func_printSeed::execute( void ) {
    
    
    RevBayesCore::RandomNumberGenerator *rng = RevBayesCore::GLOBAL_RNG;
	std::vector<unsigned int> s = rng->getSeed();

	std::cout << "Current RNG Seed = (" << s[0] << ", " << s[1] << ")" << std::endl;
    
    return NULL;
}


/** Get argument rules */
const ArgumentRules& Func_printSeed::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new Ellipsis( RevObject::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_printSeed::getClassType(void) { 
    
    static std::string revType = "Func_printSeed";
    
	return revType; 
}


/** Get class type spec describing type of object */
const TypeSpec& Func_printSeed::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/** Get type spec */
const TypeSpec& Func_printSeed::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_printSeed::getReturnType( void ) const {
    
    return RevNullObject::getClassTypeSpec();
}
