/**
 * @file
 * This file contains the declaration of WrapperRule, which is
 * used to describe rules for arguments passed by reference
 * and not by value.
 *
 * @brief Declaration of ArgumentRule
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

#ifndef WrapperRule_H
#define WrapperRule_H

#include "ArgumentRule.h"

#include <string>

class DAGNode;
class VectorString;

class WrapperRule : public ArgumentRule {

    public:
                                    WrapperRule(const std::string& argName, const std::string& valType, int varDim=0);    //!< Wrapper rule without default
                                    WrapperRule(const std::string& argName, DAGNode* defVar);                             //!< Wrapper rule with default

        // Basic utility functions
        WrapperRule*                clone(void) const { return new WrapperRule(*this); }    //!< Clone object
        virtual const VectorString& getClass(void) const;                                   //!< Get class vector
};

#endif
