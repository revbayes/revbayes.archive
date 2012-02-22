/**
 * @file
 * This file contains the declaration of ArgumentRule, which is
 * the base class for objects used to describe rules for
 * arguments passed to functions.
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

#ifndef ArgumentRule_H
#define ArgumentRule_H

#include "RbInternal.h"
#include "TypeSpec.h"
#include "Environment.h"
#include "VariableSlot.h"

#include <string>

class DAGNode;
class RbObject;

class ArgumentRule : public RbInternal {

    public:
        // Basic utility functions
        virtual ArgumentRule*       clone(void) const { return new ArgumentRule(*this); }                                               //!< Clone object
        static const std::string&   getClassName(void);                                                                                 //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                                                             //!< Get class type spec
        virtual const TypeSpec&     getTypeSpec(void) const;                                                                            //!< Get language type of the object
        void                        printValue(std::ostream& o) const;                                                                  //!< Print value for user
        std::string                 debugInfo(void) const;                                                                              //!< General info on object

        // ArgumentRule functions
        const std::string&          getArgumentLabel(void) const;                                                                       //!< Get label of argument
        const std::string&          getArgumentType(void) const;                                                                        //!< Get argument type
        const TypeSpec&             getArgumentTypeSpec(void) const;                                                                    //!< Get argument type spec
        const Variable&             getDefaultVariable(void) const;                                                                     //!< Get default argument
        Variable&                   getDefaultVariable(void);                                                                           //!< Get default argument (non-const to return non-const variable)
        bool                        hasDefault(void) const;                                                                             //!< Has default?
        virtual bool                isArgumentValid(const RbVariablePtr& var, bool convert = false) const;                              //!< Is var valid argument?

    protected:
                                    ArgumentRule(const std::string& argName, RbLanguageObject *defValue);                               //!< Constructor of rule from default value
                                    ArgumentRule(const std::string& argName, const TypeSpec& argTypeSp);                                //!< Constructor of rule without default value
                                    ArgumentRule(const std::string& argName, const TypeSpec& argTypeSp, RbLanguageObject *defValue);    //!< Constructor of rule with default value
                                    ArgumentRule(const std::string& argName, const TypeSpec& argTypeSp, DAGNode *defVariable);          //!< Constructor of rule with default reference or default wrapped value

        std::string                 label;                                                                                              //!< Label of argument
        VariableSlot                argSlot;                                                                                            //!< Slot with typespec and possibly default value
        bool                        hasDefaultVal;                                                                                      //!< Has default (which can be NULL) ?

};

#endif
