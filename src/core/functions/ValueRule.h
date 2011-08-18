/**
 * @file
 * This file contains the declaration of ValueRule, which is
 * used for argument rules pertaining to value variables.
 *
 * @brief Declaration of ValueRule
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

#ifndef ValueRule_H
#define ValueRule_H

#include "ArgumentRule.h"
#include "TypeSpec.h"

#include <string>

class DAGNode;
class RbObject;
class VectorString;



const std::string ValueRule_name = "ValueRule";

class ValueRule : public ArgumentRule {

    public:
                                    ValueRule(const std::string& argName, RbLanguageObject* defValue);                               //!< Constructor of rule from default value
                                    ValueRule(const std::string& argName, const TypeSpec& argTypeSp);                                //!< Constructor of rule without default value
                                    ValueRule(const std::string& argName, const TypeSpec& argTypeSp, RbLanguageObject* defValue);    //!< Constructor of rule with default value
                                    ValueRule(const std::string& argName, const TypeSpec& argTypeSp, DAGNode* defVariable);          //!< Constructor of rule with default reference or default wrapped value

        // Basic utility functions
        virtual ValueRule*          clone(void) const { return new ValueRule(*this); }                                               //!< Clone object
        virtual const VectorString& getClass(void) const;                                                                            //!< Get class vector
        std::string                 richInfo(void) const;                                                                            //!< General info on object

};

#endif
