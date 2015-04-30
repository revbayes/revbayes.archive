#include "SegregatingSitesFunction.h"
#include "Func_SegregatingSites.h"
#include "RlAbstractDiscreteCharacterData.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_SegregatingSites::Func_SegregatingSites( void ) : TypedFunction<Natural>( )
{
    
}


/** Clone object */
Func_SegregatingSites* Func_SegregatingSites::clone( void ) const
{
    
    return new Func_SegregatingSites( *this );
}


RevBayesCore::TypedFunction< int >* Func_SegregatingSites::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<RevBayesCore::AbstractDiscreteCharacterData >* d = static_cast<const AbstractDiscreteCharacterData &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::SegregatingSitesFunction* f = new RevBayesCore::SegregatingSitesFunction( d );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_SegregatingSites::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "data", AbstractDiscreteCharacterData::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_SegregatingSites::getClassType(void)
{
    
    static std::string revType = "Func_SegregatingSites";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_SegregatingSites::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( TypedFunction<Natural>::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


const TypeSpec& Func_SegregatingSites::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
