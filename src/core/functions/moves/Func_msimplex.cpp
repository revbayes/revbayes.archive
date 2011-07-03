/**
 * @file
 * This file contains the implementation of Func_msimplex, a class
 * used to construct a move on simplices.
 *
 * @brief Implementation of Func_msimplex
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id$
 */

#include "Func_msimplex.h"
#include "List.h"
#include "Move_msimplex.h"
#include "Natural.h"
#include "RateMatrix.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "RealPos.h"
#include "ReferenceRule.h"
#include "Simplex.h"
#include "StochasticNode.h"
#include "UserInterface.h"
#include "ValueContainer.h"
#include "ValueRule.h"
#include "VectorReal.h"
#include "VectorString.h"
#include <sstream>
#include <vector>



/** Clone object */
Func_msimplex* Func_msimplex::clone(void) const {
    
    return new Func_msimplex( *this );
}


/** Execute function */
DAGNode* Func_msimplex::execute(void) {

    // get the information from the arguments for reading the file
    StochasticNode* snde = dynamic_cast<StochasticNode*>( args[0].getReference() );
    const RealPos* v     = static_cast<const RealPos*>( args[1].getValue() );
    const Natural* nc    = static_cast<const Natural*>( args[2].getValue() );
    const RealPos* w     = static_cast<const RealPos*>( args[3].getValue() );
        
    // do some checking
    const Simplex* s = static_cast<const Simplex*>( snde->getValue() );
    if ( snde == NULL ) 
        throw ( RbException("Cannot attach a move to a DAG element that is not stochastic") );
    if ( v->getValue() < 0.00001 )
        throw ( RbException("Variance parameter of simplex move is too small") );
    if ( w->getValue() < 0.00001 )
        throw ( RbException("Weight parameter of simplex move is too small") );
    if ( nc->getValue() > int( s->getElementsSize() ) )
        throw ( RbException("You cannot update more simplex variables than the dimensions of the simplex") );
    if ( nc->getValue() <= 0 )
        throw ( RbException("You must update at least one simplex variable") );
    
    // construct the move
    Move_msimplex* mve = new Move_msimplex( snde, v->getValue(), nc->getValue(), w->getValue() );
    
    // wrap up the move object and send it on its way to parser-ville
    return mve->wrapIntoVariable();
}


/** Get argument rules */
const ArgumentRules& Func_msimplex::getArgumentRules(void) const {
    
    static ArgumentRules argumentRules;
    static bool          rulesSet = false;
    
    if (!rulesSet) 
        {
        argumentRules.push_back( new ReferenceRule( "simplex",  Simplex_name ) );
        argumentRules.push_back( new ValueRule(     "variance", RealPos_name ) );
        argumentRules.push_back( new ValueRule(     "ncat",     Natural_name ) );
        argumentRules.push_back( new ValueRule(     "weight",   RealPos_name ) );
        rulesSet = true;
        }
            
    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& Func_msimplex::getClass(void) const {
    
    static VectorString rbClass = VectorString( Func_msimplex_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec Func_msimplex::getReturnType(void) const {
    
    return TypeSpec( RbVoid_name );
}

