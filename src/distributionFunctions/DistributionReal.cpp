/**
 * @file
 * This file contains the implementation of some functions in
 * DistributionReal, which is used to hold distribution functions
 * on real-valued parameters in the symbol table. It is also used
 * for the distri bution attribute of stochastic DAG nodes of
 * real-valued type.
 *
 * @brief Implementation of DistributionReal (some functions)
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id$
 */

#include "DistributionReal.h"
#include "RbDataType.h"
#include "RbVector.h"

/** Call appropriate function depending on function type */
RbDataType* DistributionReal::execute() {

    if (functionType == d) {
        if ( !useLogs )
            return pdf();
        else
            return lnPdf();
    }
    else if ( functionType == p ) {
        if ( !useLogs )
            return cdf();
        else
            return lnCdf();
    }
    else if ( functionType == q ) {
        return quantile();
        if ( !useLogs )
            return quantile();
        else
            return quantileFromLog();
    }
    else if ( functionType == r )
        return rv();

    return NULL;
}

/** Set convenient pointer to result container */
void DistributionReal::setResultContainers() {
    
    result = vec = new RbVector();
}
