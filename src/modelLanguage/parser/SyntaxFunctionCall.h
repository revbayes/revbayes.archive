/*!
 * \file
 * This file contains the declaration of SytnaxFunctionCall, which is
 * used to hold function calls in the syntax tree.
 *
 * \brief Declaration of SyntaxFunctionCall
 *
 * (c) Copyright 2009- under GPL version 3
 * \date Last modified: $Date$
 * \author Fredrik Ronquist and the REvBayes core team
 * \license GPL version 3
 *
 * $Id$
 */

#ifndef SyntaxFunctionCall_H
#define SyntaxFunctionCall_H

#include "RbString.h"
#include "SyntaxElement.h"

using namespace std;

//! This is the class used to hold function calls in the syntax tree.
class SyntaxFunctionCall : public SyntaxElement {

    public:
        enum operatorT = { ACCESSOR, DISTRIBUTION, FUNCTION };

            SyntaxFunctionCall(const string functionName, list<SyntaxElement*> arguments);   //!< Constructor
	        virtual ~SytnaxFunctionCall();          //!< Destructor; delete local copy of function

        virtual RbDataType* getValue();                 //!< Get semantic value
        virtual void        print(ostream &c) const;    //!< Print content
    
    protected:
        RbFunction *function;    //!< Local copy of the function
        operatorT   operator;    //!< Type of function call
};

#endif
