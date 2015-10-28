#include "Func_InfiniteSitesRateMatrix.h"
#include "InfiniteSitesRateMatrixFunction.h"
#include "Natural.h"
#include "RateMatrix_InfiniteSites.h"
#include "RlRateMatrix.h"

using namespace RevLanguage;

/** default constructor */
Func_InfiniteSitesRateMatrix::Func_InfiniteSitesRateMatrix( void ) : TypedFunction<RateGenerator>( )
{
    
}


/** Clone object */
Func_InfiniteSitesRateMatrix* Func_InfiniteSitesRateMatrix::clone( void ) const
{
    
    return new Func_InfiniteSitesRateMatrix( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::RateGenerator >* Func_InfiniteSitesRateMatrix::createFunction( void ) const
{
    
    int ns = static_cast<const Natural &>( this->args[0].getVariable()->getRevObject() ).getValue();
    RevBayesCore::InfiniteSitesRateMatrixFunction* f = new RevBayesCore::InfiniteSitesRateMatrixFunction( size_t(ns) );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_InfiniteSitesRateMatrix::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "numStates", Natural::getClassTypeSpec(), "The number of states.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(2) ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_InfiniteSitesRateMatrix::getClassType(void)
{
    
    static std::string revType = "Func_InfiniteSitesRateMatrix";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_InfiniteSitesRateMatrix::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/**
 * Get the primary Rev name for this function.
 */
const std::string& Func_InfiniteSitesRateMatrix::getFunctionName( void ) const
{
    // create a static name variable that is the same for all instance of this class
    static std::string f_name = "fnInfiniteSites";
    
    return f_name;
}


const TypeSpec& Func_InfiniteSitesRateMatrix::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
