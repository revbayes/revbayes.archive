#include "Func_probability.h"
#include "Probability.h"
#include "Real.h"
#include "ReferenceFunction.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_probability::Func_probability( void ) : TypedFunction<Probability>( )
{
    
}


/** Clone object */
Func_probability* Func_probability::clone( void ) const
{
    
    return new Func_probability( *this );
}


RevBayesCore::TypedFunction<double>* Func_probability::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<double>* x = static_cast<const Real&>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::ReferenceFunction<double>* f = new RevBayesCore::ReferenceFunction<double>( x );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_probability::getArgumentRules( void ) const
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


const std::string& Func_probability::getClassType(void)
{
    
    static std::string revType = "Func_probability";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_probability::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


const TypeSpec& Func_probability::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
