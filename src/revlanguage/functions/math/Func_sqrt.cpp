#include "Func_sqrt.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "SqrtFunction.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_sqrt::Func_sqrt( void ) : TypedFunction<RealPos>( )
{
    
}


/** Clone object */
Func_sqrt* Func_sqrt::clone( void ) const {
    
    return new Func_sqrt( *this );
}


RevBayesCore::TypedFunction<double>* Func_sqrt::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<double>* arg = static_cast<const Real &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::SqrtFunction* f = new RevBayesCore::SqrtFunction( arg );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_sqrt::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "x", RealPos::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_sqrt::getClassType(void) { 
    
    static std::string revType = "Func_sqrt";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_sqrt::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


const TypeSpec& Func_sqrt::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
