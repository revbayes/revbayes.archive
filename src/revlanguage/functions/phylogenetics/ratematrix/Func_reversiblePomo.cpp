#include "Func_reversiblePomo.h"
//
//  Func_reversiblePomo.cpp
//  RevBayesCore
//
//  Created by Bastien Boussau on 22/8/14.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "ReversiblePomoRateMatrixFunction.h"
#include "Func_reversiblePomo.h"
#include "Natural.h"
#include "RateMatrix_ReversiblePomo.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_reversiblePomo::Func_reversiblePomo( void ) : TypedFunction<RateMatrix>( ) {

}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_reversiblePomo* Func_reversiblePomo::clone( void ) const {

    return new Func_reversiblePomo( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::RateGenerator >* Func_reversiblePomo::createFunction( void ) const
{

  RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator >* q = static_cast<const RateMatrix &>( this->args[0].getVariable()->getRevObject() ).getDagNode();

  //  RevBayesCore::TypedDagNode<RevBayesCore::RateMatrix >* q = static_cast<const RateMatrix &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    //RevBayesCore::TypedDagNode< double >* root_pol = static_cast<const double &>( this->args[1].getVariable()->getRevObject() ).getDagNode();

    RevBayesCore::TypedDagNode< int >* n = static_cast<const Natural &>( this->args[1].getVariable()->getRevObject() ).getDagNode();

    RevBayesCore::ReversiblePomoRateMatrixFunction* f = new RevBayesCore::ReversiblePomoRateMatrixFunction( n, q );

    return f;
}


/* Get argument rules */
const ArgumentRules& Func_reversiblePomo::getArgumentRules( void ) const
{

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rules_set = false;

    if ( !rules_set )
    {

        argumentRules.push_back( new ArgumentRule( "mutationRates", RateGenerator::getClassTypeSpec()    , "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "virtualNe"    , Natural::getClassTypeSpec()          , "", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );

        rules_set = true;
    }

    return argumentRules;
}


const std::string& Func_reversiblePomo::getClassType(void)
{

    static std::string rev_type = "Func_reversiblePomo";

	return rev_type;
}


/* Get class type spec describing type of object */
const TypeSpec& Func_reversiblePomo::getClassTypeSpec(void)
{

    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );

	return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_reversiblePomo::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "fnReversiblePomo";

    return f_name;
}


const TypeSpec& Func_reversiblePomo::getTypeSpec( void ) const
{

    static TypeSpec type_spec = getClassTypeSpec();

    return type_spec;
}
