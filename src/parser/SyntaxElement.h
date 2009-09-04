/*!
 * \file
 * This file contains the declaration of SyntaxElement, which is
 * the base class used to hold elements (nodes) in the syntax tree.
 *
 * \brief Declaration of SyntaxElement
 *
 * (c) Copyright 2009-
 * \date Last modified: $Date$
 * \author Fredrik Ronquist and the REvBayes core team
 * \license GPL version 3.0
 *
 * $Id$
 */

#ifndef SyntaxElement_H
#define SyntaxElement_H

#include "../main/DAGNode.h"
#include "../main/RbDataType.h"

using namespace std;

/*! This is the abstract base class for nodes in the syntax tree. It is derived
 *  from DAGNode, which contains essential functionality like a value member
 *  and a getValue method.
 *
 *  The syntax tree is built up by syntax element nodes. The syntax elements either
 *  have one or more operands, which are themselves syntax elements, or they have
 *  no operands and simply a predefined result of type (pointer to) RbDataType. In
 *  the former case, the elements correspond to interior nodes in the syntax tree
 *  and in the latter case, they correspond to terminal nodes.
 *
 *  If you call getValue on an interior node and the result has not been filled in,
 *  or the node is marked for recalculation, the syntax element should evaluate its
 *  semantic value, causing recursive execution of the syntax subtree rooted on that
 *  element, after which the semantic value is returned.
 *
 *  If you call getValue on a terminal element, the predefined result is simply returned.
 *
 *  A syntax element has the ability to restore itself to a previous state, to speed
 *  up accept and reject steps for deterministic nodes in a model DAG. A touched flag is
 *  used to mark an element for later recalculation. The new value can be kept with keep
 *  or discarded with restore. This functionality is implemented in the DAGNode base
 *  class.
 *
 *  Each syntax element needs to implement the function isSyntaxCorrect, which checks that
 *  the syntax tree rooted on the element is correct and returns true or false depending
 *  on the outcome.
 */
class SyntaxElement : public DAGNode {

    public:
            //SyntaxElement() : DAGNode(), result(NULL) {}      //!< Default constructor calls base class
            SyntaxElement() : DAGNode() {}      //!< Default constructor calls base class
            virtual ~SyntaxElement() {}         //!< Destructor; base class deletes values

        virtual RbDataType  getDataType(void) = 0;      //!< Return the data type of the semantic value
        virtual bool        isConstExpr(void) = 0;      //!< Is the syntax tree rooted here a constant expression?
};

#endif

