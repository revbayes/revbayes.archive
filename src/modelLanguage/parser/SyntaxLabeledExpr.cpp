/*!
 * \file
 * This file contains the implementation of SyntaxLabeledExpr, which is
 * used to hold labeled expressions in the syntax tree.
 *
 * \brief Implementation of SyntaxLabeledExpr
 *
 * (c) Copyright 2009- under GPL version 3
 * \date Last modified: $Date$
 * \author Fredrik Ronquist and the REvBayes core team
 * \license GPL version 3
 *
 * $Id$
 */

#include "RbString.h"
#include "SyntaxLabeledExpr.h"
#include <sstream>

using namespace std;

SyntaxLabeledExpr::SyntaxLabeledExpr(const string& id, SyntaxElement* expr) :
    SyntaxElement(), label(id), expression(expr) {

    parents.insert(expression);
}

/** Print some info about the labeled expression */
void SyntaxLabeledExpr::print (ostream& c) const {

    c << "SyntaxLabeledExpr: label = " << label << " --  value = ";
    expression->print(c);
    c << endl;
}
