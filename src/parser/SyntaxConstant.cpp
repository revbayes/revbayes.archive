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

/** Construct from bool */
SyntaxConstant::SyntaxConstant(const bool v) :
    SyntaxElement() {

    value = new RbBool(v);    
}

/** Construct from int */
SyntaxConstant::SyntaxConstant(const int v) :
    SyntaxElement() {

    value = new RbInt(v);    
}

/** Construct from double */
SyntaxConstant::SyntaxConstant(const double v) :
    SyntaxElement() {

    value = new Scalar(v);    
}

/** Print info about the syntax element */
void SyntaxConstant::print (ostream &c) const {

    c << "SyntaxConstant: type=" << value->getType() << " value=" << value << endl;
}

/** Print content to console */
void SyntaxConstant::printConsole (ostream &c) const {

    c << value << endl;
}
