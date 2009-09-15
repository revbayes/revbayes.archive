/**
 * @file
 * This file contains the default argument rules for accessor
 * functions in REvBayes. It assumes a single unlabeled argument
 * of type RbDataType.
 *
 * @brief Default argument rules for accessor functions
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

#include "ArgumentRule.h"
#include "RbAccessorFxn.h"
#include "RbAbstractDataType.h"


/** Set default argument rules for accessor functions */
const ArgumentRule RbAccessorFxn::argRules[] = {
    ArgumentRule("", RbAbstractDataType::dataType),
    ArgumentRule()
};

/** Copy constructor */
RbAccessorFxn::RbAccessorFxn(const RbAccessorFxn& s)
    : RbStandardFxn(s) {
}
