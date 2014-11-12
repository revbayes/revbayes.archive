/* 
 * File:   Func_t92.cpp
 * Author: nl
 * 
 * Created on 25 juillet 2014, 20:12
 */

#include "Func_t92.h"

#include "HkyRateMatrixFunction.h"
#include "RateMatrix_HKY.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_t92::Func_t92( void ) : TypedFunction<RateMatrix>( ) {
    
}


/** Clone object */
Func_t92* Func_t92::clone( void ) const {
    
    return new Func_t92( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::RateMatrix >* Func_t92::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode< double >* ka = static_cast<const RealPos &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* bf = static_cast<const Simplex &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::HkyRateMatrixFunction* f = new RevBayesCore::HkyRateMatrixFunction( ka, bf );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_t92::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "kappa"          , RealPos::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "baseFrequencies", Simplex::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_t92::getClassType(void) { 
    
    static std::string revType = "Func_t92";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_t92::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


const TypeSpec& Func_t92::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
