/*!
 * \file
 * This file contains the implementation of Parser, which deals with the
 * parsing of user input (or input from a source file).
 *
 * \brief Implementation of Parser
 *
 * MrBayes version 4.0 beta
 *
 * (c) Copyright 2005-
 * \version 4.0 Beta
 * \date Last modified: $Date: 2009/02/03 16:26:33 $
 * \author John Huelsenbeck (1)
 * \author Bret Larget (2)
 * \author Paul van der Mark (3)
 * \author Fredrik Ronquist (3,4)
 * \author Donald Simon (5)
 * \author Marc Suchard (6)
 * \author (authors listed in alphabetical order)
 * (1) Department of Integrative Biology, University of California, Berkeley
 * (2) Departments of Botany and of Statistics, University of Wisconsin - Madison
 * (3) Department of Scientific Computing, Florida State University
 * (4) Department of Entomology, Swedish Museum of Natural History, Stockholm, Sweden
 * (5) Department of Mathematics/Computer Science, Duquesne University
 * (6) Departments of Biomathematics, of Biostatistics and of Human Genetics, University of California, Los Angeles
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License (the file gpl.txt included with this
 * distribution or http://www.gnu.org/licenses/gpl.txt) for more
 * details.
 *
 * $Id: Parser.cpp,v 1.66 2009/02/03 16:26:33 ronquist Exp $
 */

//! Trigger printing of PRINTF debug statements by defining DEBUG_PARSER
#ifndef NDEBUG
#ifdef DEBUG_PARSER
#define PRINTF printf
#else
#define PRINTF(...)
#endif
#endif


#include <cmath>
#include <list>

#include "MbAttribute.h"
#include "MbBasicTypes.h"
#include "MbException.h"
#include "Parser.h"
#include "ParameterPosReal.h"

using namespace std;


//! Parser::checkType: Check that objectType is of refType
bool Parser::checkType(const string &objectType, const string &refType) {

    //! Return false if the type of the new value is wrong. This is done
    //  by searching for an isSubType match with any of the allowed types of the attribute
    size_t pos;
    string theType;
    for(pos=0 ; pos<refType.size(); pos+=(theType.size()+1)) {
        theType = refType.substr(pos,refType.find_first_of("|",pos));
        if (isSubType(objectType,theType))
            break;
    }
    if (pos<refType.size())
        return true;

    return true;
}


//! This function does type conversion
MbObject *Parser::convert(const MbObject *object, const string &refType) {

    string objType = object->getMyType();

    if (objType==MbInt::getType()) {
        if(checkType("ParameterPosReal",refType) && object->getSize()==1)
            return new ParameterPosReal((HPFLOAT)(object->getInt()));
    }

    return NULL;        //!< default case
}


//! This function recursively executes a syntax tree
int Parser::execute(SyntaxElement *root) const {

#ifdef DEBUG_PARSER
    //! Print syntax tree
    cerr << endl;
    cerr << "Syntax tree before execution:\n";
    root->print(cerr);
    cerr << endl;
#endif

    //! Declare a variable for the result
    const MbObject *result;
    
    //! Execute syntax tree
    try {
        result = root->getResult();
    }
    catch(MbException &mbException) {
        //! Catch a quit request
        if (mbException.getMbExceptionType() == MbException::QUIT)
            exit(0);

        //! All other exceptions
        ostringstream msg;
        mbException.print(msg);
        msg << endl;
        ui->output(msg);
        return 1;
    }

    //! Print result if not assign expression
    if (result != NULL && root->getOperator() != SyntaxElement::ASSIGN) {
        ostringstream msg;
        msg << *result << endl;
        ui->output(msg);
    }

    //! Delete syntax tree
    delete root;

    //! Return success
    return 0;
}


//! Print information about an object or set of objects pointed to by
//  a variable name (or function name)
int Parser::info(SyntaxElement *element) const {
    PRINTF("Parser::info\n");
    ostringstream msg;

#if defined DEBUG_PARSER
    //! Print syntax tree
    cerr << endl;
    cerr << "Syntax tree before info:\n";
    element->print(cerr);
    cerr << endl;
#endif

    const MbObject *object = element->getResult();

    //! \todo Check if function; if so, use command help instead (or modify print function of function objects)
    object->print(msg);
    ui->output(msg);

    //! Delete syntax tree
    delete element;

    //! Return success
    return 0;
}


//! The insert functions build up a syntax tree. The error checking is left
//  until the syntax tree is evaluated, which is done with the function execute.
//  It is simply very difficult to do error checking before evaluating syntax
//  elements.
//
//  This variant of the insert function is used for assign expressions and tilde
//  expressions. The structure of the new syntax element will be
//
//  operator    = op (ASSIGN or DISTAS)
//  operands[0] = lhs (left-hand side)
//  operands[1] = rhs (right-hand side)
//  result      = empty initially
SyntaxElement *Parser::insert(SyntaxElement::operatorT op, SyntaxElement *lhs, SyntaxElement *rhs) const {

    assert(op==SyntaxElement::ASSIGN || op==SyntaxElement::DISTAS);
    
    list<SyntaxElement *> operands;
    operands.push_back(lhs);
    operands.push_back(rhs);

    return new SyntaxElement(op, operands);
}


//! This variant of the insert function is used for constant expressions.
//  The structure of the new syntax element will be
//
//  operator    = op (CONSTANT)
//  operands    = empty
//  result      = the constant MbObject
SyntaxElement *Parser::insert(SyntaxElement::operatorT op, MbObject *value) const {

    assert(op==SyntaxElement::CONSTANT);

    return new SyntaxElement(value);
}


//! This variant of the insert function is used for function calls and parameters.
//
//  In case of a function call, the structure of the new syntax element will be
//
//  operator    = op (FUNCTION_CALL)
//  operands[0] = identifier (FUNCTION) (for look-up of the function in the symbol table)
//  operands[1] = parameter 1, a syntax element of type PARAMETER
//  operands[2] = parameter 2, a syntax element of type PARAMETER
//  ... etc
//  result      = initially empty
//
//  In case of a parameter, the structure of the new syntax element will be
//
//  operator    = op (PARAMETER)
//  operands[0] = label, a syntax element of type CONSTANT
//  operands[1] = expression, a syntax element of type FUNCTION_CALL or VARIABLE or ...
//  result      = initially empty
SyntaxElement *Parser::insert(SyntaxElement::operatorT op, list<SyntaxElement *> *operands) const {

    assert(op==SyntaxElement::FUNCTION_CALL || op==SyntaxElement::PARAMETER);

    return new SyntaxElement(op, *operands);
}


//! This variant of the insert function is used for identifiers of variables or elements of array variables.
//  The structure of the new syntax element will be
//                                                       Example for a$b$c[1:3]
//  operator    = op (VARIABLE)                          ----------------------
//  operands[0] = base variable name (CONSTANT)                    a
//  operands[1] = first-level attribute (CONSTANT)                 b
//  operands[2] = second-level attribute (CONSTANT)                c
//  ... etc
//  operands[k] = element identifier, an expression (optional)    1:3
//  result      = initially empty
SyntaxElement *Parser::insert(SyntaxElement::operatorT op, MbObject *id, list<MbObject *> *attributeId, SyntaxElement *elementExpr) const {

    assert(op==SyntaxElement::VARIABLE);

    list<SyntaxElement *> operands;

    operands.push_back(new SyntaxElement(id));
    for (list<MbObject *>::const_iterator i=attributeId->begin(); i!=attributeId->end(); i++)
        operands.push_back(new SyntaxElement(*i));
    if (elementExpr != NULL)
        operands.push_back(elementExpr);

    return new SyntaxElement(SyntaxElement::VARIABLE, operands);
}

