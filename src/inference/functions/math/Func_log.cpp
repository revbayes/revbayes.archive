/**
 * @file
 * This file contains the implementation of Func_log.
 *
 * @brief Implementation of Func_log
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

#include "ConstantNode.h"
#include "DAGNode.h"
#include "DeterministicNode.h"
#include "Func_log.h"
#include "RealPos.h"
#include "Real.h"
#include "RbException.h"
#include "RbUtil.h"
#include "StochasticNode.h"
#include "TypeSpec.h"
#include "ValueRule.h"

#include <cassert>
#include <cmath>


/** Clone object */
Func_log* Func_log::clone( void ) const {
    
    return new Func_log( *this );
}


/** Execute function */
void Func_log::executeSimple( std::vector<size_t> &offset ) {
    
    result.value[offset[2]] = log10( x.value[offset[0]] ) / log10( base.value[offset[1]] );	
}


/** We catch here the setting of the argument variables to store our parameters. */
void Func_log::setInternalArguments(const std::vector<RbValue<void*> > &args) {
    
    x.value         = static_cast<double*>( args[0].value );
    x.lengths       = args[0].lengths;
    
	base.value		= static_cast<double*>( args[1].value );
	base.lengths	= args[1].lengths;
	
    result.value    = static_cast<double*>( args[2].value );
    result.lengths  = args[2].lengths;
}

