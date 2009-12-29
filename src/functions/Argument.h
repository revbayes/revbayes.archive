/**
 * @file
 * This file contains the declaration of Argument, which is
 * used to hold a potentially labeled argument passed to a
 * function. The Argument class manages the argument wrapper.
 *
 * @brief Declaration of Argument
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 *
 * $Id$
 */

#ifndef Argument_H
#define Argument_H

#include "RbInternal.h"

#include <ostream>
#include <string>

class DAGNode;


class Argument : public RbInternal {

    public:
                    Argument(const std::string& argLabel, DAGNode* arg);            //!< Constructor 
                    Argument(const Argument& a);                                    //!< Copy constructor 
            virtual ~Argument();                                                    //!< Destructor 

        // Assignment operator
        Argument&                   operator=(const Argument& x);                       //!< Assignment operator

        // Basic utility functions
        Argument*                   clone(void) const { return new Argument (*this); }  //!< Clone object
        virtual const StringVector& getClass(void) const;                               //!< Get class vector
        std::string                 toString(void) const;                               //!< Complete info about object

        // Regular functions
        std::string                 getLabel(void) const { return label; }              //!< Get label of argument
        const DAGNode*              getDAGNode(void) const { return dagNode; }          //!< Get DAG node wrapping the argument

    protected:
        std::string                 label;          //!< Label of argument
        const DAGNode*              dagNode;        //!< The DAG node wrapper of the argument value
};

#endif

