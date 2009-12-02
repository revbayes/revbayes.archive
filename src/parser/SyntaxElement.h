/**
 * @file
 * This file contains the declaration of SyntaxElement, which is
 * the base class used to hold elements (nodes) in the syntax tree.
 *
 * @brief Declaration of SyntaxElement
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @interface SyntaxElement
 * @since Version 1.0, 2009-09-02
 *
 * $Id$
 */

#ifndef SyntaxElement_H
#define SyntaxElement_H

#include "DAGNode.h"
#include "Environment.h"

#include <iostream>
#include <string>

/* Forward declarations */
class Environment;
class StringVector;

/**
 *  @brief Interface for syntax elements
 *
 *  SyntaxElement is the base class for nodes in the syntax tree. The nodes either
 *  have one or more operands, which are themselves syntax elements, or they have
 *  no operands and simply a predefined result of type (pointer to) RbObject. In
 *  the former case, the elements correspond to interior nodes in the syntax tree
 *  and in the latter case, they correspond to terminal nodes.
 *
 *  If you call getValue on a syntax element, the syntax element will calculate its
 *  semantic value, causing recursive execution of the syntax subtree rooted on that
 *  element. Unless part of a user-defined function, a syntax tree should be evaluated
 *  only once.
 *
 *  If you call getValue on a terminal element, the predefined result is simply returned.
 *  Some syntax elements operate only through their side effects, for instance labeled
 *  expressions, and their semantic value is NULL.
 *
 *  If syntax elements are part of equation expressions or expressions that are passed
 *  as arguments to a distribution function in a tilde statement, they are converted
 *  to DAG nodes to allow repeated evaluation during an MCMC run. All syntax elements
 *  that can be part of expressions need to be able to convert themselves to appropriate
 *  DAG nodes by implementing the getDAGNode() function.
 *
 *  Syntax elements also need to implement a function getReturnType(), which returns
 *  a string giving the type of the semantic value without executing the syntax element
 *  to get its semantic value.
 *
 *  The destructor of syntax elements should delete the entire syntax tree rooted at the
 *  element. We accomplish this by calling delete on all the parent nodes in the destructor
 *  of the base class. Derived syntax elements simply need to store their parents in the
 *  vector of parent nodes.
 */
class SyntaxElement {

    public:
            virtual ~SyntaxElement() {}         //!< Destructor; delete syntax subtree

        // Basic utility functions
        virtual std::string         briefInfo() const = 0;                      //!< Brief info about object
        virtual SyntaxElement*      clone() const = 0;                          //!< Clone object
        virtual bool                equals(const SyntaxElement* elem) const = 0;//!< Equals comparison
        virtual void                print(std::ostream& o) const = 0;           //!< Print info about object

        // Regular functions
        virtual DAGNode*            getDAGNode(Environment* env=NULL) const = 0;//!< Convert to DAG node
        virtual RbObject*           getValue(Environment* env=NULL) = 0;        //!< Get semantic value
        virtual bool                isConstExpr() const { return false; }       //!< Is subtree constant expr?

    protected:
            SyntaxElement() {}      //!< Protected constructor, no elements of this class

};

#endif

