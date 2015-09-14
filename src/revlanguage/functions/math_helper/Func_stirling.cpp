#include "Func_stirling.h"
#include "RbMathCombinatorialFunctions.h"
#include "Natural.h"
#include "Integer.h"
#include "Real.h"
#include "OptionRule.h"

using namespace RevLanguage;

/** default constructor */
Func_stirling::Func_stirling( void ) : Procedure( ) {
    
}


/** Clone object */
Func_stirling* Func_stirling::clone( void ) const {
    
    return new Func_stirling( *this );
}


RevPtr<RevVariable> Func_stirling::execute() {

    std::string kind = static_cast<const RlString &>( args[0].getVariable()->getRevObject() ).getValue();
    int n = static_cast<const Natural &>( args[1].getVariable()->getRevObject() ).getValue();
    int k = static_cast<const Natural &>( args[2].getVariable()->getRevObject() ).getValue();
	
	if(kind == "lnFirst"){
		double sN = RevBayesCore::RbMath::lnStirlingFirst(n, k);
		Real* value = new Real( sN );
		return new RevVariable( value );
	}
	else if(kind == "first"){
		unsigned long sN = RevBayesCore::RbMath::stirlingFirst(n, k);
		Natural* value = new Natural( sN );
		return new RevVariable( value );
	}
	else if(kind == "second"){
		int sN = RevBayesCore::RbMath::stirlingSecond(n, k);
		Integer* value = new Integer( sN );
		return new RevVariable( value );
	}
	
	RlString* nv = new RlString("Incorrect arguments provided.");
	return new RevVariable( nv );
}


/* Get argument rules */
const ArgumentRules& Func_stirling::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        std::vector<std::string> optionsCondition;
        optionsCondition.push_back( "first" );
        optionsCondition.push_back( "lnFirst" );
        optionsCondition.push_back( "second" );
        argumentRules.push_back( new OptionRule( "kind"    , new RlString("first"), optionsCondition ) );
        argumentRules.push_back( new ArgumentRule( "n"    , Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        argumentRules.push_back( new ArgumentRule( "k", Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_stirling::getClassType(void) { 
    
    static std::string revType = "Func_stirling";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_stirling::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/* Get return type */
const TypeSpec& Func_stirling::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = Real::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_stirling::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
