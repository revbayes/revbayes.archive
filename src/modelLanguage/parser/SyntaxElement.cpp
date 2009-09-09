/*!
 * \file
 * This file contains the implementation of some functions in SyntaxElement,
 * which is the base class used to hold elements (nodes) in the syntax tree.
 *
 * \brief Implementation of SyntaxElement (some functions)
 *
 * (c) Copyright 2009-
 * \date Last modified: $Date$
 * \author Fredrik Ronquist and the REvBayes core team
 * \license GPL version 3.0
 *
 * $Id$
 */

#include "SyntaxElement.h"
#include <string>

using namespace std;

/** Destructor deletes entire syntax tree */
SyntaxElement:: ~SyntaxElement() {

    for (set<DAGNode*>::iterator i=parents.begin(); i!=parents.end(); i++)
        delete *i;
}
