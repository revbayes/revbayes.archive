/*!
 * \file
 * This file contains the implementation of SyntaxConstant, which is
 * used to hold constants in the syntax tree.
 *
 * \brief Implementation of SyntaxConstant
 *
 * (c) Copyright 2009- under GPL version 3
 * \date Last modified: $Date$
 * \author Fredrik Ronquist and the REvBayes core team
 * \license GPL version 3
 *
 * $Id$
 */

#include "RbBool.h"
#include "RbInt.h"
#include "Scalar.h"
#include "SyntaxConstant.h"
#include <iostream>

using namespace std;

SyntaxConstant::SyntaxConstant(const bool v) :
    SyntaxElement(), op(SyntaxConstant::BOOL) {

    value = new RbBool(v);    
}

SyntaxConstant::SyntaxConstant(const int v) :
    SyntaxElement(), op(SyntaxConstant::INT) {

    value = new RbInt(v);    
}

SyntaxConstant::SyntaxConstant(const double v) :
    SyntaxElement(), op(SyntaxConstant::REAL) {

    value = new Scalar(v);    
}

void SyntaxConstant::print (ostream &c) const {

    c << "SyntaxConstant: type=" << value->getType() << " value=" << value << endl;
}

