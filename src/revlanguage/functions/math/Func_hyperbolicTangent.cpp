#include "HyperbolicTangentFunction.h"
#include "Func_hyperbolicTangent.h"
#include "Probability.h"
#include "Real.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_hyperbolicTangent::Func_hyperbolicTangent( void ) : TypedFunction<Real>( )
{
    
}


/** Clone object */
Func_hyperbolicTangent* Func_hyperbolicTangent::clone( void ) const
{
    
    return new Func_hyperbolicTangent( *this );
}


RevBayesCore::TypedFunction<double>* Func_hyperbolicTangent::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<double>* x = static_cast<const Real&>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::HyperbolicTangentFunction* f = new RevBayesCore::HyperbolicTangentFunction( x );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_hyperbolicTangent::getArgumentRules( void ) const
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


const std::string& Func_hyperbolicTangent::getClassType(void)
{
    
    static std::string revType = "Func_hyperbolicTangent";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_hyperbolicTangent::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


const TypeSpec& Func_hyperbolicTangent::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
