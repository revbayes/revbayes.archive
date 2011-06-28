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
 * $Id:$
 */

#ifndef ArgumentRule_H
#define ArgumentRule_H

#include "RbInternal.h"
#include "TypeSpec.h"
#include "VariableSlot.h"

#include <string>

class DAGNode;
class RbObject;
class VectorString;

class ArgumentRule : public RbInternal {

    public:
        // Basic utility functions
        virtual ArgumentRule*       clone(void) const { return new ArgumentRule(*this); }                                               //!< Clone object
        virtual const VectorString& getClass(void) const;                                                                               //!< Get class vector
        void                        printValue(std::ostream& o) const;                                                                  //!< Print value for user
        std::string                 richInfo(void) const;                                                                               //!< General info on object

        // ArgumentRule functions
        DAGNode*                    convert(DAGNode* arg) const;                                                                        //!< Convert arg to conform to rule
        size_t                      getArgDim(void) const { return argSlot.getTypeSpec().getDim(); }                                    //!< Get argument dim
        std::string                 getArgLabel(void) const { return label; }                                                           //!< Get label of argument
        const std::string&          getArgType(void) const { return argSlot.getTypeSpec().getType(); }                                  //!< Get argument type
        const TypeSpec&             getArgTypeSpec(void) const { return argSlot.getTypeSpec(); }                                        //!< Get argument type spec
        RbObject*                   getDefaultValue(void) const;                                                                        //!< Get default val (copy)
        DAGNode*                    getDefaultVariable(void) const;                                                                     //!< Get default value wrapped as node (copy)
        DAGNode*                    getDefaultReference(void);                                                                          //!< Get default reference (ptr)
        bool                        hasDefault(void) const { return hasDefaultVal; }                                                    //!< Has default?
        virtual bool                isArgValid(DAGNode* var, bool& needsConversion, bool once) const;                                   //!< Is var valid argument?
        bool                        isReference(void) const { return argSlot.getTypeSpec().isReference(); }                             //!< Is reference ('&') argument?
        void                        setReference(bool flag) { argSlot.setReferenceFlag(flag); }                                         //!< Set reference flag

    protected:
                                    ArgumentRule(const std::string& argName, RbObject* defValue);                                       //!< Constructor of rule from default value
                                    ArgumentRule(const std::string& argName, const TypeSpec& argTypeSp);                                //!< Constructor of rule without default value
                                    ArgumentRule(const std::string& argName, const TypeSpec& argTypeSp, RbObject* defValue);            //!< Constructor of rule with default value
                                    ArgumentRule(const std::string& argName, const TypeSpec& argTypeSp, DAGNode* defVariable);          //!< Constructor of rule with default reference or default wrapped value

        std::string                 label;                                                                                              //!< Label of argument
        VariableSlot                argSlot;                                                                                            //!< Slot with typespec and possibly default value/ref
        bool                        hasDefaultVal;                                                                                      //!< Has default (which can be NULL) ?
};

#endif
