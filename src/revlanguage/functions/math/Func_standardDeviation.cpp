#include "StandardDeviationFunction.h"
#include "Func_standardDeviation.h"
#include "ModelVector.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_standardDeviation::Func_standardDeviation( void ) : TypedFunction<Real>( )
{
    
}


/** Clone object */
Func_standardDeviation* Func_standardDeviation::clone( void ) const {
    
    return new Func_standardDeviation( *this );
}


RevBayesCore::TypedFunction<double>* Func_standardDeviation::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* arg = static_cast<const ModelVector<Real> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::StandardDeviationFunction* f = new RevBayesCore::StandardDeviationFunction( arg );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_standardDeviation::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "x", ModelVector<Real>::getClassTypeSpec(), "The vector of samples.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_standardDeviation::getClassType(void) {
    
    static std::string revType = "Func_standardDeviation";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_standardDeviation::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


const TypeSpec& Func_standardDeviation::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
