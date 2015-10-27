#include "VarianceFunction.h"
#include "Func_variance.h"
#include "ModelVector.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_variance::Func_variance( void ) : TypedFunction<Real>( )
{
    
}


/** Clone object */
Func_variance* Func_variance::clone( void ) const {
    
    return new Func_variance( *this );
}


RevBayesCore::TypedFunction<double>* Func_variance::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* arg = static_cast<const ModelVector<Real> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::VarianceFunction* f = new RevBayesCore::VarianceFunction( arg );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_variance::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "x", ModelVector<Real>::getClassTypeSpec(), "A vector of values.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_variance::getClassType(void) {
    
    static std::string revType = "Func_variance";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_variance::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


const TypeSpec& Func_variance::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
