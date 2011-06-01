/**
 * @file
 * This file contains the declaration of ReferenceRule, which is
 * used to describe rules for arguments passed by reference
 * and not by value.
 *
 * @brief Declaration of ReferenceRule
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 *
 * $Id$
 */

#ifndef ReferenceRule_H
#define ReferenceRule_H

#include "ArgumentRule.h"

#include <string>

class DAGNode;
class VectorString;

class ReferenceRule : public ArgumentRule {

    public:
                                        ReferenceRule(const std::string& argName, const std::string& valType, int valDim=0);    //!< Reference rule without default
                                        ReferenceRule(const std::string& argName, const std::string& valType, DAGNode* defVar); //!< Reference rule with default

        // Basic utility functions
        ReferenceRule*                  clone(void) const { return new ReferenceRule(*this); }                                  //!< Clone object
        virtual const VectorString&     getClass(void) const;                                                                   //!< Get class vector
};

#endif
