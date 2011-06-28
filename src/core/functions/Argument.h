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
 * $Id:$
 */

#ifndef Argument_H
#define Argument_H

#include "DAGNode.h"
#include "RbInternal.h"
#include "RbObject.h"

#include <ostream>
#include <string>

class Argument : public RbInternal {

    public:
                                Argument(const std::string& argLabel, DAGNode* arg);                //!< Constructor 

        // Basic utility functions
        Argument*               clone(void) const { return new Argument (*this); }                  //!< Clone object
        const VectorString&     getClass(void) const;                                               //!< Get class vector
        std::string             richInfo(void) const;                                               //!< Complete info about object

        // Regular functions
        std::string             getLabel(void) const { return label; }                              //!< Get label of argument
        DAGNode*                getVariable(void) const { return dagNodePtr; }                      //!< Get argument variable

    protected:
        std::string             label;                                                              //!< Label of argument
        DAGNode*                dagNodePtr;                                                         //!< Pointer to the DAG node wrapper of the argument value
};

#endif

