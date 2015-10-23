#include "Func_power.h"
#include "PowerFunction.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_power::Func_power( void ) : TypedFunction<RealPos>( ) {
    
}


/** Clone object */
Func_power* Func_power::clone( void ) const {
    
    return new Func_power( *this );
}


RevBayesCore::TypedFunction<double>* Func_power::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<double>* b = static_cast<const Real &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* e = static_cast<const Real &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::PowerFunction* f = new RevBayesCore::PowerFunction( b, e );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_power::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "base"    , Real::getClassTypeSpec(), "The base.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        argumentRules.push_back( new ArgumentRule( "exponent", Real::getClassTypeSpec(), "The exponent.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_power::getClassType(void) { 
    
    static std::string revType = "Func_power";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_power::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


const TypeSpec& Func_power::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
