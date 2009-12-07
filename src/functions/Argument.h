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

#include "RbObject.h"

class DAGNode;

class Argument : public RbObject {

    public:
                    Argument(const std::string& lbl, DAGNode* n);       //!< Constructor 
                    Argument(const Argument& a);                        //!< Copy constructor 
            virtual ~Argument();                        //!< Destructor 

        // Basic utility functions
         std::string         briefInfo() const;                          //!< Brief info about object
         RbObject*           clone() const;                              //!< Clone object
         bool                equals(const RbObject* obj) const;          //!< Equals comparison
         const StringVector& getClass() const;                           //!< Get class vector
         void                print(std::ostream& o) const;               //!< Print complete object info
         void                printValue(std::ostream& o) const;          //!< Print value (for user)
         std::string         toString(void) const;                                 //!< General info on object

        // Regular functions
        std::string         getLabel() const { return label; }          //!< Get label of argument
        DAGNode*            getDAGNode() const { return node; }         //!< Get DAG node


        // overloaded operators
        RbObject&           operator=(const RbObject& o);
        Argument&           operator=(const Argument& o);

    protected:
        const std::string   label;          //!< Label of argument
        DAGNode*            node;           //!< The node passed in as the argument
};

#endif

