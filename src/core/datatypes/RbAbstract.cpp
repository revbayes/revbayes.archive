/**
 * @file
 * This file contains the implementation of RbAbstract, which is
 * used to describe abstract types in the type table.
 *
 * @brief Implementation of RbAbstract
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */


#include "RbBoolean.h"
#include "RbAbstract.h"
#include "RealPos.h"
#include "Real.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbString.h"
#include "TypeSpec.h"
#include "VectorInteger.h"
#include "VectorString.h"

#include <sstream>

/** Constructor */
RbAbstract::RbAbstract( const VectorString classVec, TypeSpec* elemType ) : RbLanguageObject(), classVector( classVec ), elementType( elemType ), typeSpec( classVec[0], elemType) {
}


/** Clone object */
RbAbstract* RbAbstract::clone( void ) const {

	return  new RbAbstract( *this );
}


/** Get language type specification of abstract class */
const TypeSpec& RbAbstract::getTypeSpec( void ) const {
    return typeSpec;
}


/** Print value for user */
void RbAbstract::printValue(std::ostream &o) const {

    o << classVector;
}


