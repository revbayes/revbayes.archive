#include "SumFunction.h"
#include "Func_sum.h"
#include "ModelVector.h"
#include "Real.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_sum::Func_sum( void ) : TypedFunction<Real>( )
{
    
}


/** Clone object */
Func_sum* Func_sum::clone( void ) const
{
    
    return new Func_sum( *this );
}


RevBayesCore::TypedFunction<double>* Func_sum::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* arg = static_cast<const ModelVector<Real> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::SumFunction* f = new RevBayesCore::SumFunction( arg );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_sum::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "x", ModelVector<Real>::getClassTypeSpec(), "A vector of numbers.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_sum::getClassType(void)
{
    
    static std::string revType = "Func_sum";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_sum::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


const TypeSpec& Func_sum::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
