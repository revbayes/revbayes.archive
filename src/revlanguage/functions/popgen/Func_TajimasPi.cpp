#include "TajimasPiFunction.h"
#include "Func_TajimasPi.h"
#include "RlAbstractHomologousDiscreteCharacterData.h"
#include "RlBoolean.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_TajimasPi::Func_TajimasPi( void ) : TypedFunction<RealPos>( )
{
    
}


/** Clone object */
Func_TajimasPi* Func_TajimasPi::clone( void ) const
{
    
    return new Func_TajimasPi( *this );
}


RevBayesCore::TypedFunction< double >* Func_TajimasPi::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<RevBayesCore::AbstractHomologousDiscreteCharacterData >* d = static_cast<const AbstractHomologousDiscreteCharacterData &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::Boolean >* ps = static_cast<const RlBoolean &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TajimasPiFunction* f = new RevBayesCore::TajimasPiFunction( d, ps->getValue() );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_TajimasPi::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "data",    AbstractHomologousDiscreteCharacterData::getClassTypeSpec(), "The character data matrix for which to compute the summary.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "perSite", RlBoolean::getClassTypeSpec(), "Is the statistic normalized per site?", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RlBoolean(true) ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_TajimasPi::getClassType(void)
{
    
    static std::string revType = "Func_TajimasPi";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_TajimasPi::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( TypedFunction<RealPos>::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


const TypeSpec& Func_TajimasPi::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
