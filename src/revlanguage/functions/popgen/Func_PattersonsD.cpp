#include "PattersonsDFunction.h"
#include "Func_PattersonsD.h"
#include "RlAbstractHomologousDiscreteCharacterData.h"
#include "RlBoolean.h"
#include "RlDeterministicNode.h"
#include "RlString.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_PattersonsD::Func_PattersonsD( void ) : TypedFunction<Real>( )
{
    
}


/** Clone object */
Func_PattersonsD* Func_PattersonsD::clone( void ) const
{
    
    return new Func_PattersonsD( *this );
}


RevBayesCore::TypedFunction< double >* Func_PattersonsD::createFunction( void ) const
{
    std::string p1 = static_cast<const RlString &>( this->args[0].getVariable()->getRevObject() ).getValue();
    std::string p2 = static_cast<const RlString &>( this->args[1].getVariable()->getRevObject() ).getValue();
    std::string p3 = static_cast<const RlString &>( this->args[2].getVariable()->getRevObject() ).getValue();
    std::string og = static_cast<const RlString &>( this->args[3].getVariable()->getRevObject() ).getValue();
    
    RevBayesCore::TypedDagNode<RevBayesCore::AbstractHomologousDiscreteCharacterData >* d = static_cast<const AbstractHomologousDiscreteCharacterData &>( this->args[4].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::PattersonsDFunction* f = new RevBayesCore::PattersonsDFunction( p1, p2, p3, og, d );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_PattersonsD::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        argumentRules.push_back( new ArgumentRule( "p1"      , RlString::getClassTypeSpec(), "",                                ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "p2"      , RlString::getClassTypeSpec(), "",                                ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "p3"      , RlString::getClassTypeSpec(), "",                                ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "outgroup", RlString::getClassTypeSpec(), "",                                ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "data"    , AbstractHomologousDiscreteCharacterData::getClassTypeSpec(), "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_PattersonsD::getClassType(void)
{
    
    static std::string revType = "Func_PattersonsD";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_PattersonsD::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( TypedFunction<Real>::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/**
 * Get the primary Rev name for this function.
 */
const std::string& Func_PattersonsD::getFunctionName( void ) const
{
    // create a static name variable that is the same for all instance of this class
    static std::string f_name = "fnPattersonsD";
    
    return f_name;
}


const TypeSpec& Func_PattersonsD::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}