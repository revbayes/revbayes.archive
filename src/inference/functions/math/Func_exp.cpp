/**
 * @file
 * This file contains the implementation of Func_exp.
 *
 * @brief Implementation of Func_sqrt
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

#include "Func_exp.h"
#include "RbValue.h"

#include <cassert>
#include <cmath>


Func_exp::Func_exp(void) : InferenceFunction() {
    
}


/** Clone object */
Func_exp* Func_exp::clone( void ) const {
    
    return new Func_exp( *this );
}


/** Execute function */
void Func_exp::execute( void ) {
    
    *result.value = exp( *lambda.value );
}


/** We catch here the setting of the argument variables to store our parameters. */
void Func_exp::setArguments(const std::vector<RbValue<void*> > &args) {
    
    lambda.value    = ( static_cast<double*>( args[0].value ) );
    lambda.lengths  = args[0].lengths;
    
    result.value    = static_cast<double*>( args[1].value );
    result.lengths  = args[1].lengths;
}

