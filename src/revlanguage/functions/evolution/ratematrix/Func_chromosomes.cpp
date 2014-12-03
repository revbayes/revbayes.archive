/**
 * @file
 * This file contains the implementation of the RevLanguage chromosomes function, which
 * is used to created a deterministic variable associated with the chromosomes function.
 *
 * @brief Implementation of Func_chromosomes
 *
 * (c) Copyright 2014- under GPL version 3
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 */

#include "Func_chromosomes.h"
#include "ChromosomesRateMatrixFunction.h"
#include "Func_chromosomes.h"
#include "RateMatrix_Chromosomes.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_chromosomes::Func_chromosomes( void ) : Function( ) {
    
}


/** Clone object */
Func_chromosomes* Func_chromosomes::clone( void ) const {
    
    return new Func_chromosomes( *this );
}


RevPtr<Variable> Func_chromosomes::execute() {
    
    
    RevBayesCore::TypedDagNode< int >* n = static_cast<const Natural &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< double >* la = static_cast<const RealPos &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< double >* de = static_cast<const RealPos &>( this->args[2].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< double >* rh = static_cast<const RealPos &>( this->args[3].getVariable()->getRevObject() ).getDagNode();

    RevBayesCore::ChromosomesRateMatrixFunction* f = new RevBayesCore::ChromosomesRateMatrixFunction( n, la, de, rh );
    
    DeterministicNode<RevBayesCore::RateMatrix> *detNode = new DeterministicNode<RevBayesCore::RateMatrix>("", f, this->clone());
    
    RateMatrix* value = new RateMatrix( detNode );
    
    return new Variable( value );
}


/* Get argument rules */
const ArgumentRules& Func_chromosomes::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
      
        argumentRules.push_back( new ArgumentRule( "maxChromosomes"    , Natural::getClassTypeSpec()          , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "lambda"          , RealPos::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "delta"          , RealPos::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "rho"          , RealPos::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_chromosomes::getClassType(void) {
    
    static std::string revType = "Func_chromosomes";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_chromosomes::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/* Get return type */
const TypeSpec& Func_chromosomes::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RateMatrix::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_chromosomes::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
