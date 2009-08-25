/*!
 * \file
 * This file contains the declaration of FunctionCall, which is
 * used to hold function calls in the syntax tree.
 *
 * \brief Declaration of FunctionCall
 *
 * (c) Copyright 2009- under GPL version 3
 * \date Last modified: $Date$
 * \author Fredrik Ronquist and the REvBayes core team
 * \license GPL version 3
 *
 * $Id$
 */

#ifndef FunctionCall_H
#define FunctionCall_H

#include "RbString.h"
#include "SyntaxElement.h"

using namespace std;

/*! This is the class used to hold function calls in the syntax tree.
 */
class FunctionCall : public SyntaxElement {

    public:
            FunctionCall(const string id, list<SyntaxElement*> elemList);   //!< Constructor
	        virtual ~FunctionCall();          //!< Destructor; delete syntax tree

        virtual RbDataType* getValue();                 //!< Get semantic value
        virtual void        print(ostream &c) const;    //!< Print content
    
    protected:
        list<SyntaxElement*>  arguments;    //!< The arguments of the function
};

#endif
