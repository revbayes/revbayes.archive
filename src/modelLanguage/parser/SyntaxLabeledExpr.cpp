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

#include "../main/RbString.h"
#include "../main/RbDataType.h"
#include "SyntaxLabeledExpr.h"
#include <sstream>

using namespace std;

SyntaxLabeledExpr::SyntaxLabeledExpr(const RbString* id, SyntaxElement* expr) :
    SyntaxElement(), name(id->copy()), expression(expr) {
}

RbDataType* SyntaxLabeledExpr::getValue(void) {

    if (!value) 
        value = expression->getValue();
    else if (touched) {
        changed = true;
        storedValue = value;
        value = expression->getValue();
    }
    return value;
}

void SyntaxLabeledExpr::print (ostream& c) const {

    if (!value)
        c << "SyntaxLabeledExpr: NULL" << endl;
    else {
        c << "SyntaxLabeledExpr: type = " << value->getType() << " --  value = ";
        value->print(c);
        c << endl;
    }
}

