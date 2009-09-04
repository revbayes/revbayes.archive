/*!
 * \file
 * This file contains the declaration of SyntaxLabeledExpr, which is
 * used to hold labeled expressions in the syntax tree. Labeled
 * expressions are used in function calls for arguments and in
 * function definitions for argument labels and default values.
 *
 * \brief Declaration of SyntaxLabeledExpr
 *
 * (c) Copyright 2009- under GPL version 3
 * \date Last modified: $Date$
 * \author Fredrik Ronquist and the REvBayes core team
 * \license GPL version 3
 *
 * $Id$
 */

#ifndef SyntaxLabeledExpr_H
#define SyntaxLabeledExpr_H

#include "../main/RbString.h"
#include "SyntaxElement.h"

using namespace std;

/*! This is the class used to hold labeled expressions in the syntax
 *  tree. These are used as arguments to functions and as templates
 *  for arguments in function definitions.
 *
 *  The result is never going to change so we can set the value
 *  already in the constructor.
 */
class SyntaxLabeledExpr : public SyntaxElement {

    public:
            SyntaxLabeledExpr(const RbString *id, SyntaxElement *expr);   //!< Constructor
	        virtual ~SyntaxLabeledExpr();          //!< Destructor; delete syntax tree

        virtual RbDataType* getValue();                 //!< Get semantic value
        virtual bool        isSyntaxCorrect() const { return true; }    //!< Syntax errors not possible
        virtual void        print(ostream &c) const;    //!< Print content
    
    protected:
        SyntaxElement*  expression;     //!< The expression (argument value or default value)
        RbDataType*     name;           //!< The label of the expression
};

#endif

