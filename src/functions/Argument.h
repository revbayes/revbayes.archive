/**
 * @file
 * This file contains the declaration of Argument, which is
 * used to hold a potentially labeled argument passed to a
 * function.
 *
 * @brief Declaration of Argument
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 *
 * $Id$
 */

#ifndef Argument_H
#define Argument_H

#include <string>
#include <ostream>

#include "DAGNode.h"
#include "RbObject.h"

class Argument : public RbObject {

    public:
                    Argument(const std::string& lbl, DAGNode* n);       //!< Constructor 
                    Argument(const Argument& a);                        //!< Copy constructor 
            virtual ~Argument() { delete node; }                        //!< Destructor 

        static const StringVector   rbClass;            //!< Static class attribute

        // Basic utility functions
        virtual std::string         briefInfo() const;                          //!< Brief info about object
                RbObject*           clone() const;                              //!< Clone object
        virtual bool                equals(const RbObject* obj) const;          //!< Equals comparison
        virtual const StringVector& getClass() const { return rbClass; }        //!< Get class
        virtual void                print(std::ostream& o) const;               //!< Print complete object info
        virtual void                printValue(std::ostream& o) const;          //!< Print value (for user)

        // Regular functions
        std::string         getLabel() const { return label; }          //!< Get label of argument
        DAGNode*            getDAGNode() const { return node; }         //!< Get DAG node

    protected:
        const std::string   label;          //!< Label of argument
        DAGNode*            node;           //!< The node passed in as the argument
};

#endif

