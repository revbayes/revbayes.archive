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

#include "RbPtr.h"
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
        virtual ArgumentRule*       clone(void) const = 0;                                                                              //!< Clone object
        static const std::string&   getClassName(void);                                                                                 //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                                                             //!< Get class type spec
        virtual const TypeSpec&     getTypeSpec(void) const;                                                                            //!< Get language type of the object
        virtual void                printValue(std::ostream& o) const;                                                                  //!< Print value for user
        virtual std::string         debugInfo(void) const;                                                                              //!< General info on object

        // ArgumentRule functions
        const std::string&          getArgumentLabel(void) const;                                                                       //!< Get label of argument
        const TypeSpec&             getArgumentTypeSpec(void) const;                                                                    //!< Get argument type spec
        bool                        hasDefault(void) const;                                                                             //!< Has default?
        virtual bool                isArgumentValid(const RbPtr<const Variable> &var, bool convert = false) const;                      //!< Is var valid argument?

        // functions overwritten in derived class
        virtual const Variable&     getDefaultVariable(void) const;                                                                     //!< Get default argument

    protected:
                                    ArgumentRule(const std::string& argName, const TypeSpec& argTypeSp);                                //!< Constructor of rule without default value

        bool                        isArgumentValid(const RbObject& arg, bool& conversionNeeded, TypeSpec &conversionType) const;
            
        TypeSpec                    argTypeSpec;
        std::string                 label;                                                                                              //!< Label of argument
        bool                        hasDefaultVal;                                                                                      //!< Has default

};

#endif
