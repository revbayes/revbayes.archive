#include "DeterministicNode.h"
#include "Func_dayhoff.h"
#include "Natural.h"
#include "RateMatrix_Dayhoff.h"
#include "Real.h"
#include "RealPos.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_dayhoff::Func_dayhoff( void ) : Procedure( ) {
    
}


/** Clone object */
Func_dayhoff* Func_dayhoff::clone( void ) const {
    
    return new Func_dayhoff( *this );
}


RevPtr<Variable> Func_dayhoff::execute() {
    
    
    RevBayesCore::RateMatrix_Dayhoff *rmj = new RevBayesCore::RateMatrix_Dayhoff();
    RateMatrix* value = new RateMatrix( rmj );
    
    return new Variable( value );
}


/* Get argument rules */
const ArgumentRules& Func_dayhoff::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    
    return argumentRules;
}


const std::string& Func_dayhoff::getClassType(void) { 
    
    static std::string revType = "Func_dayhoff";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_dayhoff::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/* Get return type */
const TypeSpec& Func_dayhoff::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RateMatrix::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_dayhoff::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
