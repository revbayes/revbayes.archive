#include "LogisticFunction.h"
#include "Func_logistic.h"
#include "Probability.h"
#include "Real.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_logistic::Func_logistic( void ) : TypedFunction<Probability>( )
{
    
}


/** Clone object */
Func_logistic* Func_logistic::clone( void ) const
{
    
    return new Func_logistic( *this );
}


RevBayesCore::TypedFunction<double>* Func_logistic::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<double>* x = static_cast<const Real&>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::LogisticFunction* f = new RevBayesCore::LogisticFunction( x );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_logistic::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "x", Real::getClassTypeSpec(), "The value.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_logistic::getClassType(void)
{
    
    static std::string revType = "Func_logistic";
    
    return revType;
}


/* Get class type spec describing type of object */
const TypeSpec& Func_logistic::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/**
 * Get the primary Rev name for this function.
 */
const std::string& Func_logistic::getFunctionName( void ) const
{
    // create a static name variable that is the same for all instance of this class
    static std::string f_name = "logistic";
    
    return f_name;
}


const TypeSpec& Func_logistic::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
