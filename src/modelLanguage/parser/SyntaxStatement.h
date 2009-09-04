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

#include "MbObject.h"

using namespace std;

/*! This is the abstract base class for nodes in the syntax tree.
 *
 *  The syntax tree is built up by syntax elements. The syntax elements either
 *  have one or more operands, which are themselves syntax elements, or they
 *  have no operands and simply a predefined result vector of type RbObject. In
 *  the former case, the elements correspond to interior nodes in the syntax tree
 *  and in the latter case, they correspond to terminal nodes.
 *
 *  If you call getResult on an interior element and the result has not been filled in,
 *  the syntax element will be executed (causing recursive execution of the subtree
 *  rooted on that element) before the result is returned.
 *
 *  If you call getResult on a terminal element, the predefined result is simply returned.
 *  A syntax element also has the ability to restore itself to a previous state, to speed
 *  up accept and reject steps for deterministic nodes in a model DAG.
 */
class SyntaxElement {

    public:
            SyntaxElement();         //!< Default constructor
	        ~SyntaxElement();        //!< Destructor; delete operands and result

        virtual bool        check() const = 0;              //!< Check syntax
        virtual RbObject*   getResult() = 0;                //!< Return result
        virtual void        print(ostream &c) const = 0;    //!< Print content
        virtual void        restore() { swap(); }           //!< Restore stored value (children not called in default implementation)

    protected:
        RbObject           *result;         //!< The result of executing the element; preset for terminal elements
        RbObject           *storedResult;   //!< Stored result from previous execution of the element
        void                swap() { RbObject *temp = result; result = storedResult; storedResult = temp; }  //!< Restore stored value
};

#endif
