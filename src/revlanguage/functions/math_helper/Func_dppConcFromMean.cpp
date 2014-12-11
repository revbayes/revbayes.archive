#include "Func_dppConcFromMean.h"
#include "RbStatisticsHelper.h"
#include "Natural.h"
#include "RealPos.h"

using namespace RevLanguage;

/** default constructor */
Func_dppConcFromMean::Func_dppConcFromMean( void ) : Procedure( ) {
    
}


/** Clone object */
Func_dppConcFromMean* Func_dppConcFromMean::clone( void ) const {
    
    return new Func_dppConcFromMean( *this );
}


RevPtr<RevVariable> Func_dppConcFromMean::execute() {

    double nc = static_cast<const RealPos &>( args[0].getVariable()->getRevObject() ).getValue();
    double ne = static_cast<const Natural &>( args[1].getVariable()->getRevObject() ).getValue();
	
	double meanCP = RevBayesCore::RbStatistics::Helper::dppConcParamFromNumTables(nc, ne);
	RealPos* value = new RealPos( meanCP );

	return new RevVariable( value );
}


/* Get argument rules */
const ArgumentRules& Func_dppConcFromMean::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "num_cats"    , RealPos::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        argumentRules.push_back( new ArgumentRule( "num_elements", Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_dppConcFromMean::getClassType(void) { 
    
    static std::string revType = "Func_dppConcFromMean";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_dppConcFromMean::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/* Get return type */
const TypeSpec& Func_dppConcFromMean::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RealPos::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_dppConcFromMean::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
