/**
 * @file
 * This file contains the declaration of Argument, which is
 * used to hold a potentially labeled argument passed to a
 * function. The Argument class just holds a DAG node pointer,
 * it does not manage the DAG node itself.
 *
 * @brief Declaration of Argument
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

#ifndef Argument_H
#define Argument_H

#include "DAGNode.h"
#include "RbInternal.h"
#include "RbObject.h"
#include "RbPtr.h"
#include "Variable.h"

#include <ostream>
#include <string>

class Argument : public RbInternal {

    public:

    Argument(const RbPtr<Variable> &arg, const std::string& argLabel = "", bool isConst = true);                    //!< Constructor 
    
    // Basic utility functions
    Argument*                           clone(void) const;                                                          //!< Clone object
    static const std::string&           getClassName(void);                                                         //!< Get class name
    static const TypeSpec&              getClassTypeSpec(void);                                                     //!< Get class type spec
    const TypeSpec&                     getTypeSpec(void) const;                                                    //!< Get language type of the object
    void                                printValue(std::ostream& o) const;                                          //!< Complete info about object
    
    // Regular functions
    const std::string&                  getLabel(void) const;
    RbPtr<const Variable>               getVariable(void) const;                                                    //!< Get the variable contained in this argument
    const RbPtr<Variable>&              getReferenceVariable(void) const;
    bool                                isConstant(void) const;
private:
    RbPtr<Variable>                     var;                                                                        //!< Pointer to the variable slot containing the variable (and value)
    bool                                isConst;                                                                    //!< Is this a constant variable?
    std::string                         label;                                                                      //!< Label of argument
    
};

#endif

