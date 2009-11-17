/**
 * @file
 * This file contains the declaration of SyntaxElement, which is
 * the base class used to hold elements (nodes) in the syntax tree.
 *
 * @brief Declaration of SyntaxElement
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author REvBayes core team
 * @license GPL version 3.0
 * @interface SyntaxElement
 * @since Version 1.0, 2009-09-02
 *
 * $Id$
 */

#ifndef SyntaxElement_H
#define SyntaxElement_H

#include "../../main/RbObject.h"
#include "../../dag/DAGNode.h"
#include <iostream>
#include <set>

/* Forward declarations */
class SymbolTable;

/**
 *  \brief Interface for syntax elements
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
 *  Syntax elements implement a dynamic copy method that makes a copy of part of the
 *  syntax tree for repeated fast evaluation of an expression. The variables in the
 *  expression are kept in a local symbol table if one is provided; otherwise they are
 *  in the global namespace. This functionality is used both in the creation of
 *  stochastic and deterministic DAG nodes, in the former case for distribution functions
 *  and in the latter for deterministic expressions.
 *
 *  Syntax elements also need to implement a function getDataType(), which returns the
 *  a string giving the type of the semantic value without executing the syntax element
 *  to get its semantic value.
 *
 *  The destructor of syntax elements should delete the entire syntax tree rooted at the
 *  element. We accomplish this by calling delete on all the parent nodes in the destructor
 *  of the base class. Derived syntax elements simply need to store their parents in the
 *  vector of parent nodes.
 */
class SyntaxElement : public RbObject {

    public:
            SyntaxElement() : RbObject() {}     //!< Default constructor calls base class
            virtual ~SyntaxElement();           //!< Destructor; delete syntax subtree

        virtual SyntaxElement*  copy(SymbolTable* symbols=NULL) const = 0; //!< Get fast copy of syntax subtree
        virtual DAGNode*        getDAGNode() const = 0;         //!< Convert the element to a DAG node
        virtual const string&   getValueClass(void) const = 0;  //!< Get class (data type) of semantic value
        virtual RbObject*       getValue() = 0;                 //!< Get semantic value
        virtual bool            isConstExpr(void) const = 0;    //!< Is syntax subtree a constant expression?
        virtual void            print(std::ostream& c) const = 0;       //!< Print some info
        virtual void            printConsole(std::ostream& c) const {}  //!< Print content to console

    protected:
};

#endif
