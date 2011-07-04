/**
 * @file
 * This file contains the declaration of ConstantValueRule,
 * which is used to describe value rules that require the
 * variable to be a constant.
 *
 * @brief Declaration of ConstantValueRule
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef ConstantValueRule_H
#define ConstantValueRule_H

#include "ValueRule.h"

#include <string>

class DAGNode;
class RbObject;
class VectorString;

class ConstantValueRule : public ValueRule {

    public:
                                    ConstantValueRule(const std::string& argName, TypeSpec typeSp);                         //!< Constructor from name and typespec
                                    ConstantValueRule(const std::string& argName, TypeSpec typeSp, RbObject* defVal);       //!< Constructor with default value

        // Basic utility functions
        ConstantValueRule*          clone(void) const;                                                                      //!< Clone object
        const VectorString&         getClass(void) const;                                                                   //!< Get class vector
        void                        printValue(std::ostream& o) const;                                                      //!< Print value for user
        std::string                 richInfo(void) const;                                                                   //!< General info on object

        // ConstantValueRule functions
        virtual bool                isArgValid(DAGNode* var, bool& needsConversion, bool once) const;                       //!< Is var valid argument?
};

#endif

