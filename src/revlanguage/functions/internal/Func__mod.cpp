#include "Func__mod.h"

#include "ArgumentRule.h"
#include "ModuloFunction.h"
#include "Natural.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"

using namespace RevLanguage;

Func__mod::Func__mod() : Function()
{
    
}

/* Clone object */
Func__mod* Func__mod::clone( void ) const
{
    
    return new Func__mod( *this );
}


/** Execute function: We rely on getValue or overloaded push_back to provide functionality */
RevPtr<Variable> Func__mod::execute( void ) {
    
    const RevBayesCore::TypedDagNode<int>* leftVal = static_cast<const Natural &>( args[0].getVariable()->getRevObject() ).getDagNode();
    const RevBayesCore::TypedDagNode<int>* rightVal = static_cast<const Natural &>( args[1].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::ModuloFunction *func = new RevBayesCore::ModuloFunction( leftVal, rightVal );
    
    DeterministicNode<int> *detNode = new DeterministicNode<int>("", func, this->clone());

    Natural *theBool = new Natural( detNode );
    
    return new Variable( theBool );
    
}


/** Get argument rules */
const ArgumentRules& Func__mod::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "x", true, Natural::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "y", true, Natural::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func__mod::getClassType(void)
{
    
    static std::string revType = "Func__mod";
    
	return revType;
}


/** Get class type spec describing type of object */
const TypeSpec& Func__mod::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/** Get type spec */
const TypeSpec& Func__mod::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func__mod::getReturnType( void ) const
{
    
    return Natural::getClassTypeSpec();
}

