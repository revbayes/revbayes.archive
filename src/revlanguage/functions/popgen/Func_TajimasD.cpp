#include "TajimasDFunction.h"
#include "Func_TajimasD.h"
#include "RlAbstractHomologousDiscreteCharacterData.h"
#include "RlBoolean.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_TajimasD::Func_TajimasD( void ) : TypedFunction<Real>( )
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_TajimasD* Func_TajimasD::clone( void ) const
{
    
    return new Func_TajimasD( *this );
}


RevBayesCore::TypedFunction< double >* Func_TajimasD::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<RevBayesCore::AbstractHomologousDiscreteCharacterData >* d = static_cast<const AbstractHomologousDiscreteCharacterData &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    bool excl = static_cast<const RlBoolean &>( this->args[1].getVariable()->getRevObject() ).getValue();
    RevBayesCore::TajimasDFunction* f = new RevBayesCore::TajimasDFunction( d, excl );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_TajimasD::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "data", AbstractHomologousDiscreteCharacterData::getClassTypeSpec(), "The character data matrix.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "excludeAmbiguous", RlBoolean::getClassTypeSpec(), "Should we exclude ambiguous or amissing characters?", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RlBoolean(false) ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_TajimasD::getClassType(void)
{
    
    static std::string revType = "Func_TajimasD";
    
    return revType;
}


/* Get class type spec describing type of object */
const TypeSpec& Func_TajimasD::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( TypedFunction<Real>::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_TajimasD::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "fnTajimasD";
    
    return f_name;
}


const TypeSpec& Func_TajimasD::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
