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

#include "SyntaxHelpExpr.h"
#include <ostream>

using namespace std;

SyntaxHelpExpr::SyntaxHelpExpr(const string &topic) :
    SyntaxElement(), helpTopic(topic) {

    //! \todo Get help info from the xml help files
}

/** Print some info about the element */
void SyntaxHelpExpr::print (ostream& c) const {

    c << "SyntaxHelpExpr: topic=" << helpTopic << endl;
}

/** Print content to console */
void SyntaxHelpExpr::printConsole (ostream& c) const {

    //! \todo Print help info from the xml help files instead
    c << "You requested help for topic '" << helpTopic << "'." << endl;
    c << "REvBayes doesn't know anything about that topic. Sorry!" << endl;
}
