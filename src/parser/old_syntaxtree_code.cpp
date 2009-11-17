/*!
 * \file This file contains the implementation of the SyntaxElement functions.
 *
 * \brief Definition of SyntaxElement
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
 * $Id: SyntaxElement.cpp,v 1.1 2009/02/03 16:26:33 ronquist Exp $
 */

#include <sstream>

#include "Distribution.h"
#include "MbAttribute.h"
#include "MbBasicTypes.h"
#include "MbException.h"
#include "MbObject.h"
#include "Parameter.h"
#include "SymbolTable.h"
#include "SyntaxElement.h"
#include "UserInterface.h"

extern SymbolTable    *symbolTable;

#define SYNTAX_ERROR MbException(MbException::SYNTAX_ERROR)

//! Trigger printing of PRINTF debug statements by defining DEBUG_PARSER
#ifndef NDEBUG
#ifdef DEBUG_PARSER
#define PRINTF printf
#else
#define PRINTF(...)
#endif
#endif


using namespace std;


//! Static strings matching the enum operatorT in SyntaxElement; for printing purposes.
const string SyntaxElement::opnames[] = {  string("ASSIGN"),
                                           string("CONSTANT"),
                                           string("DISTAS"),
                                           string("FUNCTION"),
                                           string("FUNCTIONCALL"),
                                           string("PARAMETER"),
                                           string("VARIABLE")
                                        };

//! Destructor, which deletes the operands and thus the entire syntax tree rooted
//  on the current element.
//  \todo We should also delete orphaned result objects. How??
SyntaxElement::~SyntaxElement() {

    for(list<SyntaxElement *>::iterator i=operands.begin();i!=operands.end();i++) 
        delete *i;
}


//! doAssign: operator is '<-'
//  Syntax element contains:
//
//  operator    = ASSIGN
//  operands[0] = lhs
//  operands[1] = rhs
void SyntaxElement::doAssign() {

    PRINTF("SyntaxElement::doAssign\n");
	ostringstream msg;

    //! Get the object (the result) from the rhs element
    //  This will trigger execution of syntax elements if necessary
    //  Don't store it as result quite yet in case we encounter syntax errors
    //  before completing execution of the assign statement
    MbObject *rhsResult = operands.back()->getResult();

    //! Get the lhs operands
    SyntaxElement *lhs = operands.front();
    list<SyntaxElement *> lhsOperands = lhs->getOperands();

    //! Get identifier of base variable
    string name = (lhsOperands.front()->getResult())->getString();
    
    //! Simply insert in symbol table if the name does not exist
    if (!symbolTable->exists(name)) {

        if (lhsOperands.size() > 1) {
            msg << "Unknown symbol '" << name << "'";
            ui->output(msg);
            throw(SYNTAX_ERROR);
        }

        symbolTable->insert(name,rhsResult);
        result = rhsResult;
        return;
    }
    
    //! Symbol exists
    
    //! Get previous object
    MbObject *entry = symbolTable->getEntry(name);

    //! Make sure it is not a function
    if (entry->isSubType(MbFunction::getType())) {
        msg << "'" << name << "' is a function and its value cannot be changed";
        ui->output(msg);
        throw(SYNTAX_ERROR);
    }

    //! Deal with assignments of the type 'a <- expression'
    if (lhsOperands.size()==1) {
        //! Remove old entry
        symbolTable->erase(name);

        //! Assign new value
        symbolTable->insert(name, rhsResult);

        //! Set result and return
        result = rhsResult;
        return;
    }

    //! We need to deal with an attribute and/or element specification

    //! Find attribute object
    list<SyntaxElement *>::iterator i = lhsOperands.begin();
    for (i++; (*i)!=lhsOperands.back(); i++) {
        string attributeName = ((*i)->getResult())->getString();
        if (!entry->attributeExists(attributeName)) {
            msg << "'" << attributeName << "' is not an attribute of '" << name << "'";
            ui->output(msg);
            throw(SYNTAX_ERROR);
        }
        name += "$" + attributeName;
        entry = entry->getAttributeValue(attributeName);
    }

    //! Deal with assignments of the type 'a$b <- expression'.
    //  Assignment is by reference or by copy depending on context. The
    //  behavior is determined by the lhs updateAttribute function.
    if ((*i)->getOperator() == SyntaxElement::CONSTANT) {
        string attributeName = ((*i)->getResult())->getString();
        if (!entry->updateAttribute(attributeName, rhsResult)) {
            msg << "Could not update attribute '" << attributeName << "' of variable '" << name << "'";
            ui->output(msg);
            throw(SYNTAX_ERROR);
        }
        //! Set result and return
        result = rhsResult;
        return;
    }

    //! Deal with assignments of the type 'a$b[1:3] <- expression' or 'a[1:3] <- expression'
    //  Assignment is always by copy.
    MbObject *elementIndex = (*i)->getResult();
    if (!elementIndex->isSubType(MbInt::getType())) {
        msg << "Index used for variable '" << name << "' is not an array of integers";
        ui->output(msg);
        throw(SYNTAX_ERROR);
    }
    vector<int> index = elementIndex->getIntVector();

    //! Check that the object has elements
    if (entry->getSize() == 0) {
        msg << "Variable '" << name << "' does not have elements";
        ui->output(msg);
        throw(SYNTAX_ERROR);
    }

    //! Check that all indices are within bounds
    for (int i=0; i<(int)index.size(); i++) {
        if (index[i] < 1) {
            msg << "Index array used for variable '" << name << "' contains a value (" << index[i] << ") smaller than 1";
            ui->output(msg);
            throw(SYNTAX_ERROR);
        }
        else if (index[i] > entry->getSize()) {
            msg << "Index array used for variable '" << name << "' contains a value (" << index[i]
                << ") larger than the number of elements (" << entry->getSize() << ")";
            ui->output(msg);
            throw(SYNTAX_ERROR);
        }
    }

    //! Check that result array is a multiple of element array; print warning if not
    if (rhsResult->getSize() != 0 && (index.size() % rhsResult->getSize() != 0)) {
        msg << "Number of items to replace is not a multiple of replacement length" << endl;
        ui->warn(msg);
    }

    //! Now replace values (always by copy). This code also works if the rhs does not have
    //  any elements because an MbObject without elements will return itself when an attempt
    //  is made to retrieve an element
    int j,k;
    for (j=k=0; j<(int)index.size(); j++, k++) {
        // Recycle if we run out of replacement elements
        if (k == rhsResult->getSize())
            k = 0;
        if (!entry->updateElement(index[j]-1, (rhsResult->getElement(k)))) {
            msg << "Could not update element " << index[j] << " of variable '" << name << "'." << endl;
            msg << "Element " << index[j] << " of variable '" << name << "' is of type '" << (entry ->getElement(index[j]-1))->getMyType() << "'." << endl;
            msg << "Replacement element " << k+1 << " is of type '" << (rhsResult->getElement(k))->getMyType() << "'";
            ui->output(msg);
            throw(SYNTAX_ERROR);
        }
    }

    //! Set result and return
    result = rhsResult;
    return;
}


//! doDistAs: operator is '~'
//  Syntax element contains:
//
//  operator    = DISTAS
//  operands[0] = lhs
//  operands[1] = rhs
void SyntaxElement::doDistAs() {

    PRINTF("SyntaxElement::doDistAs\n");
    ostringstream msg;

    //! Check that rhs evaluates to one or more distributions
    MbObject *distribution = operands.back()->getResult();
    if (distribution->getSize()==0 && !distribution->isSubType(Distribution::getType())) {
        msg << "Right-hand side is not a distribution";
        ui->output(msg);
        throw(SYNTAX_ERROR);
    }
    else if (distribution->getSize()>0 && !isSubType(distribution->getElementType(),Distribution::getType())) {
        msg << "At least one element on the right-hand side is not a distribution";
        ui->output(msg);
        throw(SYNTAX_ERROR);
    }

    //! Check if lhs exists already in the symbol table
    MbObject *parameter;
    string varName = ((operands.front()->getOperands().front())->getResult())->getString();
    if (!symbolTable->exists(varName) && operands.front()->getOperands().size()==1) {
        if (distribution->getSize()==0) {
            parameter = ((Distribution *)distribution)->getDefaultParameter();
        }
        else {
            parameter = new MbObjectVector();
            for (int i=0; i<distribution->getSize(); i++) {
                parameter->addElement(((Distribution *)(distribution->getElement(i)))->getDefaultParameter());
            }
        }
        symbolTable->insert(varName,parameter);
    }
    else {
        //! Check that lhs evalutes to one or more parameters
        parameter = operands.front()->getResult();
        if (parameter->getSize()==0 && !parameter->isSubType(Parameter::getType())) {
            msg << "Right-hand side is not a parameter";
            ui->output(msg);
            throw(SYNTAX_ERROR);
        }
        else if (parameter->getSize()>0 && !isSubType(parameter->getElementType(),Parameter::getType())) {
            msg << "At least one element on the right-hand side is not a parameter";
            ui->output(msg);
            throw(SYNTAX_ERROR);
        }
    }

    //! Check that # elements in lhs is even multiple of # elements in rhs
    int lhsSize = parameter->getSize();
    int rhsSize = distribution->getSize();
    if (lhsSize == 0) lhsSize = 1;  //!< We have one object without elements on lhs
    if (rhsSize == 0) rhsSize = 1;  //!< We have one object without elements on rhs
    if (rhsSize % lhsSize != 0) {
        msg << "Number of distributions is not an even multiple of the number of parameters" << endl;
        ui->warn(msg);
    }

    //! Set distribution attribute of lhs parameter(s)
    int i,j;
    for (i=j=0; i<parameter->getSize(); i++, j++) {
        // Recycle if we run out of distribution elements
        if (j == distribution->getSize())
            j = 0;
        
        Distribution *theDistribution;
        if (distribution->getSize()==0)
            theDistribution = (Distribution *)distribution;
        else
            theDistribution = (Distribution *)distribution->getElement(j);

        Parameter *theParameter;
        if (parameter->getSize()==0)
            theParameter = (Parameter *)parameter;
        else
            theParameter = (Parameter *)parameter->getElement(i);

        if (!(theParameter->updateAttribute("distribution",theDistribution))) {
            msg << "Could not set 'distribution' attribute of parameter '" << theParameter->getName() << "'";
            ui->output(msg);
            throw(SYNTAX_ERROR);
        }
    }
}


//! doFunction: Get the function object
//  Syntax element contains:
//
//  operator    = FUNCTION
//  operands[0] = CONSTANT (function name)
void SyntaxElement::doFunction() {

    PRINTF("SyntaxElement::doFunction\n");
    ostringstream msg;
    
    //! Get name
    string name = (operands.front()->getResult())->getString();
    
    //! Check if it exists in symbol table
    if (!symbolTable->exists(name)) {
        msg << "Unknown function '" << name << "'";
        ui->output(msg);
        throw(SYNTAX_ERROR);
    }
    
    //! Get entry
    MbObject *entry = symbolTable->getEntry(name);
    
    //! Check that entry is function
    if (!entry->isSubType(MbFunction::getType())) {
        msg << "'" << name << "' is not a function";
        ui->output(msg);
        throw(SYNTAX_ERROR);
    }

    //! Set result to function object
    result = entry;
}


//! doFunctionCall: Execute function
//  Syntax element contains:
//
//  operator    = FUNCTIONCALL
//  operands[0] = FUNCTION
//  operands[1] = PARAMETER 1
//  operands[2] = PARAMETER 2
//  ...
void SyntaxElement::doFunctionCall() {

    PRINTF("SyntaxElement::doFunctionCall\n");

    //! Get the function object
    MbFunction *theFunction = (MbFunction *) operands.front()->getResult();
    
    //! Generate argument list
    ArgumentList argList;
    list<SyntaxElement *>::iterator i = operands.begin();
    for (i++; i!=operands.end(); i++) {
        string argLabel = (((*i)->getOperands().front())->getResult())->getString();
        MbObject *argValue = (*i)->getResult();
        argList.push_back(pair<string,MbObject *>(argLabel,argValue));
    }
    
    //! Check argument types
    if (!theFunction->doTypeCheck(argList))
        throw (SYNTAX_ERROR);

    //! Call function
    result = theFunction->execute(argList);
}


//! doParameter: Used for labeled arguments (the b=1 part of f(b=1)) and for
//  unlabeled arguments.
//  Syntax element contains:
//
//  operator   = op (PARAMETER)
//  operand[0] = label (CONSTANT)
//  operand[1] = expression
void SyntaxElement::doParameter() {

    PRINTF("SyntaxElement::doParameter\n");

    //! Simply get the result from evaluating the expression
    result = operands.back()->getResult();
}


//! doVariable: Used for variable identifiers
//  Syntax element contains:
//
//  operator    = op (VARIABLE)
//  operands[0] = variable name (CONSTANT) (for look-up of the base variable in the symbol table)
//  operands[1] = 1st-level attribute (CONSTANT)
//  operands[2] = 2nd-level attribute (CONSTANT)
//  ... etc
//  operands[k] = element identifier, an expression (optional)
void SyntaxElement::doVariable() {

    PRINTF("SyntaxElement::doVariable\n");
	ostringstream msg;

    //! Get identifier of base variable
    string name = (operands.front()->getResult())->getString();
    
    //! Check if the base variable exists
    if (!symbolTable->exists(name)) {
        msg << "Unknown symbol '" << name << "'";
        ui->output(msg);
        throw(SYNTAX_ERROR);
    }

    //! Get base object
    MbObject *entry = symbolTable->getEntry(name);

    //! Make sure it is not a function
    if (entry->isSubType(MbFunction::getType())) {
        msg << "'" << name << "' is a function name and does not have a value";
        ui->output(msg);
        throw(SYNTAX_ERROR);
    }

    //! Return if simple variable
    if (operands.size()==1) {
        result = entry;
        return;
    }

    //! Complex variable identifier

    //! Find attribute object
    list<SyntaxElement *>::iterator i = operands.begin();
    for (i++; (*i)!=operands.back(); i++) {
        string attributeName = ((*i)->getResult())->getString();
        if (!entry->attributeExists(attributeName)) {
            msg << "'" << attributeName << "' is not an attribute of '" << name << "'";
            ui->output(msg);
            throw(SYNTAX_ERROR);
        }
        name += "$" + attributeName;
        entry = entry->getAttributeValue(attributeName);
    }

    //! Catch case where last operand is an attribute name
    if ((*i)->getOperator() == SyntaxElement::CONSTANT) {
        //! An attribute
        string attributeName = ((*i)->getResult())->getString();
        if (!entry->attributeExists(attributeName)) {
            msg << "'" << attributeName << "' is not an attribute of '" << name << "'";
            ui->output(msg);
            throw(SYNTAX_ERROR);
        }
        //! Set result
        result = entry->getAttributeValue(attributeName);
        return;
    }

    //! We have an element index in the last operand
    MbObject *elementIndex = (*i)->getResult();
    if (!elementIndex->isSubType(MbInt::getType())) {
        msg << "Index used for variable '" << name << "' is not an array of integers";
        ui->output(msg);
        throw(SYNTAX_ERROR);
    }
    vector<int> index = elementIndex->getIntVector();

    //! Check that the object has elements
    if (entry->getSize() == 0) {
        msg << "Variable '" << name << "' does not have elements";
        ui->output(msg);
        throw(SYNTAX_ERROR);
    }

    //! Check that all indices are within bounds
    for (int i=0; i<(int)index.size(); i++) {
        if (index[i] < 1) {
            msg << "Index array used for variable '" << name << "' contains a value (" << index[i] << ") smaller than 1";
            ui->output(msg);
            throw(SYNTAX_ERROR);
        }
    else if (index[i] > entry->getSize()) {
            msg << "Index array used for variable '" << name << "' contains a value (" << index[i]
                << ") larger than the number of elements (" << entry->getSize() << ")";
            ui->output(msg);
            throw(SYNTAX_ERROR);
        }
    }

    //! Now extract values one by one
    MbObject *newEntry = entry->clone();
    newEntry->clearElements();
    for (int i=0; i<(int)index.size(); i++)
        newEntry->addElement(entry->getElement(i));
    
    result = newEntry;
}


//! Return result if predefined or calculated, otherwise execute element
MbObject *SyntaxElement::getResult() {

    //! This test should catch both constant (terminal) elements with predefined
    //  value(s) and elements that have already been executed, saving some time.
    if (result != NULL)
        return result;

    //! Fill result and assign value to lhs
    switch (op) {
        case SyntaxElement::ASSIGN :
            doAssign();
            break;

        case SyntaxElement::DISTAS :
            doDistAs();
            break;

        case SyntaxElement::FUNCTION :
            doFunction();
            break;

        case SyntaxElement::FUNCTION_CALL :
            doFunctionCall();
            break;

        case SyntaxElement::PARAMETER :
            doParameter();
            break;

        case SyntaxElement::VARIABLE :
            doVariable();
            break;
        case SyntaxElement::CONSTANT :
            //doConstant(); // NOTE: This should be implemented, it causes a warning in gcc if it is not dealt with in the switch
            break;
    }

    return result;
}


//! Print function with possibility to prefix print with user-defined string
void SyntaxElement::print(std::ostream &c, string pre) const {

    int count=1;
    c << pre << SyntaxElement::opnames[op] << " with "
      << (int)operands.size() << " operands." << endl;

    string pre2;
    for (int i=0; i<(int)pre.size(); i++)
        pre2 += " ";

    for(list<SyntaxElement *>::const_iterator i=operands.begin();i!=operands.end();i++,count++) {
        ostringstream nPre;
        nPre << pre2 << "op " << count << ": ";
        (*i)->print(c,nPre.str());
    }

    if(result != NULL) {
        c << pre2 << "result: ";
        result->print(c);
    }
}

