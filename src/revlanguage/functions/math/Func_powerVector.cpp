#include "Func_powerVector.h"
#include "ModelVector.h"
#include "PowerVectorFunction.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_powerVector::Func_powerVector( void ) : TypedFunction< ModelVector< RealPos > >( ) {
    
}


/** Clone object */
Func_powerVector* Func_powerVector::clone( void ) const {
    
    return new Func_powerVector( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::RbVector< double > >* Func_powerVector::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* b = static_cast<const ModelVector<Real> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* e = static_cast<const Real &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::PowerVectorFunction* f = new RevBayesCore::PowerVectorFunction( b, e );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_powerVector::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "base"    , ModelVector<Real>::getClassTypeSpec(), "The base.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "exponent", Real::getClassTypeSpec()             , "The exponent.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_powerVector::getClassType(void)
{
    
    static std::string revType = "Func_powerVector";
    
	return revType;
}


/* Get class type spec describing type of object */
const TypeSpec& Func_powerVector::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_powerVector::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "power";
    
    return f_name;
}


const TypeSpec& Func_powerVector::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
