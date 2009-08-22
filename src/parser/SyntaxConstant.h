/*!
 * \file
 * This file contains the declaration of SyntaxConstant, which is
 * used to hold constants in the syntax tree.
 *
 * \brief Declaration of SyntaxConstant
 *
 * (c) Copyright 2009- under GPL version 3
 * \date Last modified: $Date$
 * \author Fredrik Ronquist and the REvBayes core team
 * \license GPL version 3
 *
 * $Id$
 */

#ifndef SyntaxConstant_H
#define SyntaxConstant_H

#include "RbObject.h"

using namespace std;

/*! This is the class used to hold constants in the syntax tree.
 *
 *  The result is never going to change so we can set the result
 *  already in the constructor.
 */
class SyntaxConstant : public SyntaxElement {

    public:
            SyntaxElement();         //!< Default constructor
	        ~SyntaxElement();        //!< Destructor; delete operands and result

        virtual bool        check() const = 0;              //!< Check syntax
        virtual RbObject*   getResult() = 0;                //!< Return result
        virtual void        print(ostream &c) const = 0;    //!< Print content
        virtual void        restore() { swap(); }           //!< Restore stored value (children not called in default implementation)

    protected:
        
        RbObject*           result;         //!< The result of executing the element; preset for terminal elements
        RbObject*           storedResult;   //!< Stored result from previous execution of the element
};

#endif
