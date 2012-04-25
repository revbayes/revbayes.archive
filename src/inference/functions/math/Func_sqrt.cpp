/**
 * @file
 * This file contains the implementation of Func_sqrt.
 *
 * @brief Implementation of Func_sqrt
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-19 11:54:31 -0700 (Thu, 19 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id: Func_sqrt.cpp 1399 2012-04-19 18:54:31Z hoehna $
 */

#include "Func_sqrt.h"
#include "RbValue.h"

#include <cassert>
#include <cmath>


Func_sqrt::Func_sqrt(void) : AbstractInferenceFunction() {
    
}


/** Clone object */
Func_sqrt* Func_sqrt::clone( void ) const {
    
    return new Func_sqrt( *this );
}


void Func_sqrt::executeSimple( std::vector<size_t> &offset ) {
    
    result.value[offset[1]] = sqrt( d.value[offset[0]] );
    
}

/** We catch here the setting of the argument variables to store our parameters. */
void Func_sqrt::setInternalArguments(const std::vector<RbValue<void*> > &args) {
    
    d.value    = ( static_cast<double*>( args[0].value ) );
    d.lengths  = args[0].lengths;
    
    result.value    = static_cast<double*>( args[1].value );
    result.lengths  = args[1].lengths;
}

