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

#include "SyntaxElement.h"
#include <string>
#include <iostream>

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
            SyntaxLabeledExpr(const string &id, SyntaxElement *expr);   //!< Constructor
	        virtual ~SyntaxLabeledExpr();          //!< Destructor; delete syntax tree

        const string&           getLabel() const { return label; }   //!< Get label
        SyntaxElement*          getExpression() const { return expression; }  //!< Get expression
        void                    print(ostream& c) const;    //!< Print info about syntax element
    
    protected:
        SyntaxElement* expression;  //!< The expression (argument value or default value)
        const string&  label;        //!< The label of the expression
};

#endif
