/*!
 * \file This file contains the declaration of Parser, which is
 * the parser class.
 *
 * \brief Declaration of Parser
 *
 * MrBayes version 4.0 beta
 *
 * (c) Copyright 2005-
 * \version 4.0 Beta
 * \date Last modified: $Date: 2009/02/03 16:26:33 $
 * \author John Huelsenbeck (1)
 * \author Bret Larget (2)
 * \author Paul van der Mark (3)
 * \author Fredrik Ronquist (4)
 * \author Donald Simon (5)
 * \author (authors listed in alphabetical order)
 * (1) Department of Integrative Biology, University of California, Berkeley
 * (2) Departments of Botany and of Statistics, University of Wisconsin - Madison
 * (3) Department of Scientific Computing, Florida State University
 * (4) Department of Entomology, Swedish Museum of Natural History, Stockholm
 * (5) Department of Mathematics/Computer Science, Duquesne University
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License (the file gpl.txt included with this
 * distribution or http://www.gnu.org/licenses/gpl.txt) for more
 * details.
 *
 * $Id: Parser.h,v 1.22 2009/02/03 16:26:33 ronquist Exp $
 */

#ifndef Parser_H
#define Parser_H

#include <assert.h>
#include <map>
#include <list>
#include <string>

#include "SyntaxElement.h"

using namespace std;


//! The Parser class handles the parsing and execution of the mb4 language. The insert functions
//  are called by the bison-generated parser code to add to the syntax tree. The different
//  overloaded variants are used for different types of syntax elements.
//
//  The execute function executes a syntax tree and the info function prints information about a
//  variable or function. The checkType and convert functions are services provided to functions
//  handled by the parser.
class Parser {
    public:
            Parser() {}    //!< default constructor; does nothing

        static bool      checkType(const string &objectType, const string &refType);     //!< check that object type is one of the permissible types
        static MbObject *convert(const MbObject *object, const string &refType);         //!< convert object to type, return NULL if failure

        int              execute(SyntaxElement *root) const;      //!< execute the syntax tree rooted at root
        int              info(SyntaxElement *element) const;      //!< provide info on a variable or function
        SyntaxElement   *insert(SyntaxElement::operatorT op, SyntaxElement *lhs, SyntaxElement *rhs) const;  //!< add to syntax tree (ASSIGN, DISTAS)
        SyntaxElement   *insert(SyntaxElement::operatorT op, MbObject *value) const;      //!< add to syntax tree (CONSTANT)
        SyntaxElement   *insert(SyntaxElement::operatorT op, list<SyntaxElement *> *operands) const; //!< add to syntax tree (FUNCTION_CALL, PARAMETER)
        SyntaxElement   *insert(SyntaxElement::operatorT op, MbObject *id, list<MbObject *> *attributeId, SyntaxElement *elementExpr) const;  //!< add to syntax tree (VARIABLE)

};

#endif
