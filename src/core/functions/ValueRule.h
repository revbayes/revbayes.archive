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


class ValueRule : public ArgumentRule {

    public:
                                    ValueRule(const std::string& argName, RbLanguageObject *defValue);                                      //!< Constructor of rule from default value
                                    ValueRule(const std::string& argName, const TypeSpec& argTypeSp, bool optional=false);                  //!< Constructor of rule without default value
                                    ValueRule(const std::string& argName, const TypeSpec& argTypeSp, RbLanguageObject *defValue);           //!< Constructor of rule with default value
                                    ValueRule(const std::string& argName, const TypeSpec& argTypeSp, DAGNode *defVariable);                 //!< Constructor of rule with default reference or default wrapped value

        // Basic utility functions
        virtual ValueRule*          clone(void) const { return new ValueRule(*this); }                                                      //!< Clone object
        virtual std::string         debugInfo(void) const;
        static const std::string&   getClassName(void);                                                                 //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                                             //!< Get class type spec
        virtual const TypeSpec&     getTypeSpec(void) const;                                                                                //!< Get language type of the object
   
};

#endif
