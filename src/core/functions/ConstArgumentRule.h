/**
 * @file
 * This file contains the declaration of ConstArgumentRule, which is
 * the base class for objects used to describe rules for
 * ConstArguments passed to functions.
 *
 * @brief Declaration of ConstArgumentRule
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-04-16 17:27:08 -0700 (Mon, 16 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 *
 * $Id: ConstArgumentRule.h 1378 2012-04-17 00:27:08Z hoehna $
 */

#ifndef ConstArgumentRule_H
#define ConstArgumentRule_H

#include "RbPtr.h"
#include "RbInternal.h"
#include "TypeSpec.h"
#include "Environment.h"
#include "VariableSlot.h"

#include <string>

class DAGNode;
class RbObject;

class ConstArgumentRule : public ArgumentRule {
    
public:
    
    // Basic utility functions
    virtual ConstArgumentRule*  clone(void) const = 0;                                                                              //!< Clone object
    static const std::string&   getClassName(void);                                                                                 //!< Get class name
    static const TypeSpec&      getClassTypeSpec(void);                                                                             //!< Get class type spec
    virtual const TypeSpec&     getTypeSpec(void) const;                                                                            //!< Get language type of the object
    void                        printValue(std::ostream& o) const;                                                                  //!< Print value for user
    std::string                 debugInfo(void) const;                                                                              //!< General info on object
    
    // ConstArgumentRule functions
    const std::string&          getConstArgumentLabel(void) const;                                                                       //!< Get label of ConstArgument
    const TypeSpec&             getConstArgumentTypeSpec(void) const;                                                                    //!< Get ConstArgument type spec
    bool                        hasDefault(void) const;                                                                             //!< Has default?
    virtual bool                isConstArgumentValid(const Variable* var, bool convert = false) const;                                   //!< Is var valid ConstArgument?
    bool                        isOptional(void) const;                                                                             //!< Is this rule optional? If so, we do not use a default parameter when no ConstArgument is given.
    
    // functions overwritten in derived class
    virtual const Variable&     getDefaultVariable(void) const = 0;                                                                     //!< Get default ConstArgument
    
protected:
    ConstArgumentRule(const std::string& argName);                                                           //!< Constructor of rule from default value
    ConstArgumentRule(const std::string& argName, const TypeSpec& argTypeSp, bool optional=false);           //!< Constructor of rule without default value
    
    bool                        isConstArgumentValid(const RbObject& arg, bool& conversionNeeded, TypeSpec &conversionType) const;
    
    TypeSpec                    argTypeSpec;
    std::string                 label;                                                                                              //!< Label of ConstArgument
    bool                        hasDefaultVal;                                                                                      //!< Has default
    bool                        optional;                                                                                           //!< Is optional?
    
};

#endif
