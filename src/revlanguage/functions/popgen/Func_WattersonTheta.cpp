#include "WattersonThetaFunction.h"
#include "Func_WattersonTheta.h"
#include "RlAbstractDiscreteCharacterData.h"
#include "RlBoolean.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_WattersonTheta::Func_WattersonTheta( void ) : TypedFunction<RealPos>( )
{
    
}


/** Clone object */
Func_WattersonTheta* Func_WattersonTheta::clone( void ) const
{
    
    return new Func_WattersonTheta( *this );
}


RevBayesCore::TypedFunction< double >* Func_WattersonTheta::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<RevBayesCore::AbstractDiscreteCharacterData >* d = static_cast<const AbstractDiscreteCharacterData &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::Boolean >* ps = static_cast<const RlBoolean &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::WattersonThetaFunction* f = new RevBayesCore::WattersonThetaFunction( d, ps->getValue() );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_WattersonTheta::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "data",    AbstractDiscreteCharacterData::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "perSite", RlBoolean::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RlBoolean(true) ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_WattersonTheta::getClassType(void)
{
    
    static std::string revType = "Func_WattersonTheta";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_WattersonTheta::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( TypedFunction<RealPos>::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


const TypeSpec& Func_WattersonTheta::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
