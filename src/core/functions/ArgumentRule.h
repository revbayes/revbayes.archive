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
class VectorString;

const std::string ArgumentRule_name = "argument rule";

class ArgumentRule : public RbInternal {

    public:
        // Basic utility functions
        virtual ArgumentRule*       clone(void) const { return new ArgumentRule(*this); }                                               //!< Clone object
        virtual const VectorString& getClass(void) const;                                                                               //!< Get class vector
        virtual const TypeSpec&     getTypeSpec(void) const;                                                                            //!< Get language type of the object
        void                        printValue(std::ostream& o) const;                                                                  //!< Print value for user
        std::string                 richInfo(void) const;                                                                               //!< General info on object

        // ArgumentRule functions
        std::string                 getArgumentLabel(void) const { return label; }                                                      //!< Get label of argument
        const std::string&          getArgumentType(void) const { return argSlot.getSlotType(); }                                       //!< Get argument type
        const TypeSpec&             getArgumentTypeSpec(void) const { return argSlot.getSlotTypeSpec(); }                               //!< Get argument type spec
        Variable*                   getDefaultVariable(void) { return argSlot.getVariable(); }                                          //!< Get default argument
        bool                        hasDefault(void) const { return hasDefaultVal; }                                                    //!< Has default?
        virtual bool                isArgumentValid(DAGNode* var, bool& needsConversion) const;                                         //!< Is var valid argument?

    protected:
                                    ArgumentRule(const std::string& argName, RbLanguageObject* defValue);                               //!< Constructor of rule from default value
                                    ArgumentRule(const std::string& argName, const TypeSpec& argTypeSp);                                //!< Constructor of rule without default value
                                    ArgumentRule(const std::string& argName, const TypeSpec& argTypeSp, RbLanguageObject* defValue);    //!< Constructor of rule with default value
                                    ArgumentRule(const std::string& argName, const TypeSpec& argTypeSp, DAGNode* defVariable);          //!< Constructor of rule with default reference or default wrapped value

        std::string                 label;                                                                                              //!< Label of argument
        VariableSlot                argSlot;                                                                                            //!< Slot with typespec and possibly default value
        bool                        hasDefaultVal;                                                                                      //!< Has default (which can be NULL) ?
    
    private:
        static const TypeSpec       typeSpec;
};

#endif
