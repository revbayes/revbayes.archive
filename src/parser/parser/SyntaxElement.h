/**
 * @file
 * This file contains the declaration of SyntaxElement, which is
 * the base class used to hold elements (nodes) in the syntax tree.
 *
 * @brief Declaration of SyntaxElement
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @interface SyntaxElement
 * @since Version 1.0, 2009-09-02
 *
 * $Id$
 */

#ifndef SyntaxElement_H
#define SyntaxElement_H

#include "Variable.h"

#include <iostream>
#include <string>

/* Forward declarations */
class Environment;
class VectorString;

/**
 *  @brief Interface for syntax elements
 *
 *  SyntaxElement is the base class for nodes in the syntax tree. The nodes either
 *  have one or more operands, which are themselves syntax elements, or they have
 *  no operands and simply a predefined result of type (pointer to) DAGNode. In
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
 *  to DAG node expressions to allow repeated evaluation during an MCMC run. All syntax
 *  elements that can be part of expressions need to be able to convert themselves to
 *  appropriate DAG nodes by implementing the getDAGNodeExpr() function.
 *
 *  Syntax elements also need to implement a function getReturnType(), which returns
 *  the type specification of the semantic value without executing the syntax element
 *  to get its semantic value.
 *
 *  The destructor of syntax elements should delete the entire syntax tree rooted at the
 *  element. This is accomplish by simply cycling through all syntax elements referenced
 *  by the syntax element to be destroyed, calling the destructor of each element in turn.
 *  The base class does not reference any other syntax elements and therefore has an empty
 *  destructor.
 */
const std::string SyntaxElement_name = "Syntax element";

class SyntaxElement : public RbInternal {

    public:
        virtual                    ~SyntaxElement(void) {}                                      //!< Destructor; delete syntax subtree

        // Basic utility functions you have to override
        virtual std::string         briefInfo(void) const = 0;                                  //!< Brief info about object
        virtual SyntaxElement*      clone(void) const = 0;                                      //!< Clone object
        virtual const VectorString& getClass(void) const;                                       //!< Get class vector 
        virtual void                print(std::ostream& o) const = 0;                           //!< Print info about object

        // Regular functions
        virtual RbPtr<Variable>     evaluateContent(RbPtr<Environment> env) = 0;                //!< Get semantic value
        virtual bool                isConstExpression(void) const { return false; }             //!< Is subtree constant expr?

    protected:
                                    SyntaxElement(void) : RbInternal() {}                       //!< Protected constructor, just in case
};

#endif

