/**
 * @file
 * This file contains the implementation of Func_ln.
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

#include "Func_ln.h"
#include "RbValue.h"

#include <cassert>
#include <cmath>


Func_ln::Func_ln(void) : AbstractInferenceFunction() {
    
}


/** Clone object */
Func_ln* Func_ln::clone( void ) const {
    
    return new Func_ln( *this );
}


void Func_ln::executeSimple( std::vector<size_t> &offset ) {
    
    result.value[offset[1]] = log( x.value[offset[0]] );
    
}

/** We catch here the setting of the argument variables to store our parameters. */
void Func_ln::setInternalArguments(const std::vector<RbValue<void*> > &args) {
    
    x.value         = ( static_cast<double*>( args[0].value ) );
    x.lengths       = args[0].lengths;
    
    result.value    = static_cast<double*>( args[1].value );
    result.lengths  = args[1].lengths;
}

