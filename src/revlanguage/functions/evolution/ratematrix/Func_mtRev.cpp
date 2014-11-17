#include "DeterministicNode.h"
#include "Func_mtRev.h"
#include "Natural.h"
#include "RateMatrix_MtRev24.h"
#include "Real.h"
#include "RealPos.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_mtRev::Func_mtRev( void ) : Procedure( ) {
    
}


/** Clone object */
Func_mtRev* Func_mtRev::clone( void ) const {
    
    return new Func_mtRev( *this );
}


RevPtr<RevVariable> Func_mtRev::execute() {
    
    
    RevBayesCore::RateMatrix_MtRev24 *rmj = new RevBayesCore::RateMatrix_MtRev24();
    RateMatrix* value = new RateMatrix( rmj );
    
    return new RevVariable( value );
}


/* Get argument rules */
const ArgumentRules& Func_mtRev::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    
    return argumentRules;
}


const std::string& Func_mtRev::getClassType(void) { 
    
    static std::string revType = "Func_mtRev";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_mtRev::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/* Get return type */
const TypeSpec& Func_mtRev::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RateMatrix::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_mtRev::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
