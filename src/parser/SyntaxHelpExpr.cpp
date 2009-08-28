/*!
 * \file
 * This file contains the implementation of SyntaxHelpExpr, which is
 * used to hold help expressions in the syntax tree.
 *
 * \brief Implementation of SyntaxHelpExpr
 *
 * (c) Copyright 2009- under GPL version 3
 * \date Last modified: $Date$
 * \author Fredrik Ronquist and the REvBayes core team
 * \license GPL version 3
 *
 * $Id$
 */

#include "RbString.h"
#include "SyntaxHelpExpr.h"
#include <sstream>

using namespace std;

SyntaxHelpExpr::SyntaxHelpExpr(const RbString* id) :
    SyntaxElement() {

    //! \todo Get help info from the xml help files instead
    ostringstream   msg;
    msg << "You requested help for topic '" << id << "'." << endl;
    msg << "REvBayes doesn't know anything about that topic. Sorry!" << endl;
    value = new RbString(msg.str());
}

void SyntaxHelpExpr::print (ostream& c) const {

    c << "SyntaxHelpExpr: type=" << value->getType() << " value:" << endl;
    value->print(c);
}

