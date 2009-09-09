/**
 * @file
 * This file contains the declaration of functions in the RbTypeConverter
 * namespace, which contains utilities for type conversions between data
 * types.
 *
 * @brief Declaration of functions in the namespace RbTypeConverter
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2009-08-23 15:04:35 +0200 (Sön, 23 Aug 2009) $
 * @author The REvBayes development core team
 * @license GPL version 3
 * @namespace RbTypeConverter
 * @package datatypes
 * @since Version 1.0, 2009-09-09
 *
 * $Id: RbDataType.h 9 2009-08-23 13:04:35Z ronquist $
 */

#ifndef RbTypeConverter_H
#define RbTypeConverter_H

#include "RbAbstractDataType.h"
#include "RbDataType.h"

namespace RbTypeConverter {

    RbDataType*     makeCommonType(std::vector<SyntaxElement*> expressions) { return NULL; }
    const string&   getCommonType(std::vector<SyntaxElement*> expressions) { return RbAbstractDataType::dataType; }
}

#endif
