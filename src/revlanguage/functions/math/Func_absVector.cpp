#include "AbsoluteValueVectorFunction.h"
#include "Func_absVector.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_absVector::Func_absVector( void ) : TypedFunction< ModelVector<RealPos> >( )
{
    
}


/** Clone object */
Func_absVector* Func_absVector::clone( void ) const
{
    
    return new Func_absVector( *this );
}


RevBayesCore::TypedFunction<RevBayesCore::RbVector<double> >* Func_absVector::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* arg = static_cast<const ModelVector<Real> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::AbsoluteValueVectorFunction* f = new RevBayesCore::AbsoluteValueVectorFunction( arg );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_absVector::getArgumentRules( void ) const
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


const std::string& Func_absVector::getClassType(void)
{
    
    static std::string revType = "Func_absVector";
    
    return revType;
}


/* Get class type spec describing type of object */
const TypeSpec& Func_absVector::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( TypedFunction<ModelVector<RealPos> >::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_absVector::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "abs";
    
    return f_name;
}


const TypeSpec& Func_absVector::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
