#include "MeanFunction.h"
#include "Func_mean.h"
#include "ModelVector.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_mean::Func_mean( void ) : TypedFunction<Real>( )
{
    
}


/** Clone object */
Func_mean* Func_mean::clone( void ) const
{
    
    return new Func_mean( *this );
}


RevBayesCore::TypedFunction<double>* Func_mean::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* arg = static_cast<const ModelVector<Real> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::MeanFunction* f = new RevBayesCore::MeanFunction( arg );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_mean::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "x", ModelVector<Real>::getClassTypeSpec(), "A vector of numbers.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_mean::getClassType(void)
{
    
    static std::string revType = "Func_mean";
    
	return revType; 
}


/* Get class type spec describing type of object */
const TypeSpec& Func_mean::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_mean::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "mean";
    
    return f_name;
}


const TypeSpec& Func_mean::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
