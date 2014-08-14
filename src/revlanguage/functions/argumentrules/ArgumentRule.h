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

#include "Environment.h"
#include "RevPtr.h"
#include "TypeSpec.h"

#include <string>

namespace RevLanguage {

class ArgumentRule {

    public: 
        ArgumentRule(const std::string& argName, bool c, const TypeSpec& argTypeSp);                                             //!< Constructor of rule without default value
        ArgumentRule(const std::string& argName, bool c, const TypeSpec& argTypeSp, RevObject *defVal);             //!< Constructor of rule without default value
        ArgumentRule(const std::string& argName, bool c, const std::vector<TypeSpec>& argTypeSp);                                             //!< Constructor of rule without default value
        ArgumentRule(const std::string& argName, bool c, const std::vector<TypeSpec>& argTypeSp, RevObject *defVal);             //!< Constructor of rule without default value
        virtual                            ~ArgumentRule(void) {}
        
        // Basic utility functions
        virtual ArgumentRule*               clone(void) const;                                                                                  //!< Clone object
        virtual void                        printValue(std::ostream& o) const;                                                                  //!< Print value for user

        // ArgumentRule functions
        const std::string&                  getArgumentLabel(void) const;                                                                       //!< Get label of argument
        const std::vector<TypeSpec>&        getArgumentTypeSpec(void) const;                                                                    //!< Get argument type spec
        virtual const Variable&             getDefaultVariable(void) const;                                                                     //!< Get default argument
        bool                                hasDefault(void) const;                                                                             //!< Has default?
        virtual bool                        isArgumentValid(const RevPtr<const Variable> &var, bool once) const;                                //!< Is var a valid argument?
        bool                                isConstant(void) const;
        virtual bool                        isEllipsis(void) const;
    

    protected:

//        bool                                isArgumentValid(const RevObject& arg, bool& conversionNeeded, TypeSpec &conversionType) const;
            
        std::vector<TypeSpec>               argTypeSpecs;
        Variable*                           defaultVar;
        bool                                isConst;
        std::string                         label;                                                                                              //!< Label of argument
        bool                                hasDefaultVal;                                                                                      //!< Has default

};
    
}

#endif
