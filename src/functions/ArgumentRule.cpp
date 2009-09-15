/*!
 * \file
 * This file contains the implementation of ArgumentRule, which is
 * used to describe rules for arguments passed to functions (and
 * distributions).
 *
 * \brief Implementation of ArgumentRule
 *
 * (c) Copyright 2009-
 * \date Last modified: $Date$
 * \author The REvBayes core team
 * \license GPL version 3.0
 *
 * $Id$
 */

#include "ArgumentRule.h"

/** Constructor for keyword = value arguments. Keyword can be set to "", signalling no label */
ArgumentRule::ArgumentRule (const string& key, const RbDataType& type, const RbDataType* defVal)
    : dataType(type.getType()), label(key), defaultValue(defVal) {
}

/** Constructor for keyword = value arguments. Keyword can be set to "", signalling no label */
ArgumentRule::ArgumentRule (const string& key, const string& type, const RbDataType* defVal)
    : dataType(type), label(key), defaultValue(defVal) {
}

/** Overload operator!= for ArgumentRule to allow static C-style arrays terminated by ArgumentRule() */
bool operator!=(const ArgumentRule& A, const ArgumentRule& B) {

    if (A.getDataType() != B.getDataType())
        return false;
    if (A.getLabel() != B.getLabel())
        return false;

    /** @todo Implement operator!= for datatypes
    if (A.defaultValue() != B.defaultValue())
        return false;
    */

    return true;
}
