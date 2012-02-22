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
#include "RbVariablePtr.h"
#include "Variable.h"

#include <ostream>
#include <string>

class Argument : public RbInternal {

    public:
                                            Argument(const RbVariablePtr& arg);                                            //!< Constructor 
                                            Argument(const std::string& argLabel, const RbVariablePtr& arg);               //!< Constructor 
                                            Argument(const Argument &x);                                        //!< Copy constructor 
    virtual                                ~Argument(void);                                                     //!< Destructor
   
        // Assignment operator
        Argument&                           operator=(const Argument& x);                                       //!< Assignment


        // Basic utility functions
        Argument*                           clone(void) const { return new Argument (*this); }                  //!< Clone object
        static const std::string&           getClassName(void);                                                 //!< Get class name
        static const TypeSpec&              getClassTypeSpec(void);                                             //!< Get class type spec
        const TypeSpec&                     getTypeSpec(void) const;                                            //!< Get language type of the object
        void                                printValue(std::ostream& o) const;                                  //!< Complete info about object

        // Regular functions
        const std::string&                  getLabel(void) const;                                               //!< Get label of argument
        const Variable&                     getVariable(void) const;                                            //!< Get the variable contained in this argument
        Variable&                           getVariable(void);                                                  //!< Get the variable contained in this argument (non-const to return non-const variable)
        const RbVariablePtr&                getVariablePtr(void) const;                                         //!< Get the variable contained in this argument (non-const to return non-const variable)
        void                                setVariable(const RbVariablePtr& newVar);                           //!< set the variable of the argument

    protected:
        std::string                         label;                                                              //!< Label of argument
        RbVariablePtr                       var;                                                                //!< Pointer to the variable slot containing the variable (and value)
    
};

#endif

