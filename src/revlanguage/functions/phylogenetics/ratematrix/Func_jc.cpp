#include "Func_jc.h"
#include "JcRateMatrixFunction.h"
#include "Natural.h"
#include "RateMatrix_JC.h"
#include "RlRateMatrix.h"

using namespace RevLanguage;

/** default constructor */
Func_jc::Func_jc( void ) : TypedFunction<RateGenerator>( )
{
    
}


/** Clone object */
Func_jc* Func_jc::clone( void ) const
{
    
    return new Func_jc( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::RateGenerator >* Func_jc::createFunction( void ) const
{
    
    int ns = static_cast<const Natural &>( this->args[0].getVariable()->getRevObject() ).getValue();
    RevBayesCore::JcRateMatrixFunction* f = new RevBayesCore::JcRateMatrixFunction( size_t(ns) );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_jc::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "numStates", Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_jc::getClassType(void)
{
    
    static std::string revType = "Func_jc";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_jc::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


const TypeSpec& Func_jc::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
