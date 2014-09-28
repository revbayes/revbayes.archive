#include "Func_powerVector.h"
#include "ModelVector.h"
#include "PowerVectorFunction.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_powerVector::Func_powerVector( void ) : Function( ) {
    
}


/** Clone object */
Func_powerVector* Func_powerVector::clone( void ) const {
    
    return new Func_powerVector( *this );
}


RevPtr<Variable> Func_powerVector::execute() {
    
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* b = static_cast<const ModelVector<Real> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* e = static_cast<const Real &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::PowerVectorFunction* f = new RevBayesCore::PowerVectorFunction( b, e );
    
    DeterministicNode<RevBayesCore::RbVector<double> > *detNode = new DeterministicNode<RevBayesCore::RbVector<double> >("", f, this->clone());
    
    ModelVector<RealPos>* value = new ModelVector<RealPos>( detNode );
    
    return new Variable( value );
}


/* Get argument rules */
const ArgumentRules& Func_powerVector::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "base"    , ModelVector<Real>::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "exponent", Real::getClassTypeSpec()             , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_powerVector::getClassType(void) {
    
    static std::string revType = "Func_powerVector";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_powerVector::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/* Get return type */
const TypeSpec& Func_powerVector::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = ModelVector<RealPos>::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_powerVector::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
