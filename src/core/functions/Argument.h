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
#include "Variable.h"

#include <ostream>
#include <string>

const std::string Argument_name = "argument";

class Argument : public RbInternal {

    public:
                                            Argument(RbPtr<Variable> arg);                                      //!< Constructor 
                                            Argument(const std::string& argLabel, RbPtr<Variable> arg);         //!< Constructor 
                                            Argument(const Argument &x);                                        //!< Copy constructor 
    virtual                                ~Argument(void);                                                     //!< Destructor

        // Basic utility functions
        Argument*                           clone(void) const { return new Argument (*this); }                  //!< Clone object
        const VectorString&                 getClass(void) const;                                               //!< Get class vector
        const TypeSpec&                     getTypeSpec(void) const;                                            //!< Get language type of the object
        std::string                         richInfo(void) const;                                               //!< Complete info about object

        // Regular functions
        RbPtr<Variable>                     getVariable(void) { return var; }                                   //!< Get the variable contained in this argument
        const std::string&                  getLabel(void) const { return label; }                              //!< Get label of argument
        RbPtr<DAGNode>                      getDagNode(void) const { return var->getDagNodePtr(); }             //!< Get argument variable
        void                                setDagNode(DAGNode *newNode);                                       //!< set the DAG node of the argument; replaces the DAG node in the variable
        void                                setVariable(RbPtr<Variable> newVar);                                //!< set the variable of the argument

    protected:
        std::string                         label;                                                              //!< Label of argument
        RbPtr<Variable>                     var;                                                                //!< Pointer to the variable slot containing the variable (and value)
    
    private:
        static const TypeSpec   typeSpec;
};

#endif

