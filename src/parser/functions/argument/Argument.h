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

        // Basic utility functions
        virtual Argument*                   clone(void) const = 0;                          //!< Clone object
        static const std::string&           getClassName(void);                                                         //!< Get class name
        static const TypeSpec&              getClassTypeSpec(void);                                                     //!< Get class type spec

        // Regular functions
        const std::string&                  getLabel(void) const;                                                       //!< Get label of argument


        // functions you need to overwrite (in ConstArgument and ReferenceArgument)
        virtual const RbPtr<const Variable>& getVariable(void) const = 0;

    protected:
        Argument(const std::string& argLabel = "");    //!< Constructor 

        std::string                         label;                                                                      //!< Label of argument
    
};

#endif

