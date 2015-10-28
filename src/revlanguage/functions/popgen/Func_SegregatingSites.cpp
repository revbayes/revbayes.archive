#include "SegregatingSitesFunction.h"
#include "Func_SegregatingSites.h"
#include "RlAbstractHomologousDiscreteCharacterData.h"
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
    
    RevBayesCore::TypedDagNode<RevBayesCore::AbstractHomologousDiscreteCharacterData >* d = static_cast<const AbstractHomologousDiscreteCharacterData &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
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
        
        argumentRules.push_back( new ArgumentRule( "data", AbstractHomologousDiscreteCharacterData::getClassTypeSpec(), "The alignment for which to compute the number of segregating sites.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_SegregatingSites::getClassType(void)
{
    
    static std::string revType = "Func_SegregatingSites";
    
    return revType;
}


/**
 * Get the primary Rev name for this function.
 */
const std::string& Func_SegregatingSites::getFunctionName( void ) const
{
    // create a static name variable that is the same for all instance of this class
    static std::string f_name = "fnSegregatingSites";
    
    return f_name;
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
