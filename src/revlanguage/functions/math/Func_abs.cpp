#include "AbsoluteValueFunction.h"
#include "Func_abs.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_abs::Func_abs( void ) : TypedFunction<RealPos>( )
{
    
}


/** Clone object */
Func_abs* Func_abs::clone( void ) const
{
    
    return new Func_abs( *this );
}


RevBayesCore::TypedFunction<double>* Func_abs::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<double>* arg = static_cast<const Real &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::AbsoluteValueFunction* f = new RevBayesCore::AbsoluteValueFunction( arg );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_abs::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "x", Real::getClassTypeSpec(), "A (possibly negative) number.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_abs::getClassType(void) { 
    
    static std::string revType = "Func_abs";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_abs::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


const TypeSpec& Func_abs::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
