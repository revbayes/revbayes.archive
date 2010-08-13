/**
 * @file
 * This file contains the implementation of SyntaxStatement, which is
 * used to hold control and loop statements in the syntax tree.
 *
 * @brief Implementation of SyntaxStatement
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 *
 * $Id$
 */

#include <iostream>
#include <list>
#include <sstream>

#include "DAGNode.h"
#include "DeterministicNode.h"
#include "RbException.h"
#include "StringVector.h"
#include "SyntaxStatement.h"
#include "Workspace.h"


/** Static vector of strings giving names of statement types */
std::string SyntaxStatement::stmtName[] = { "IF", "IF_ELSE", "FOR", "WHILE", "NEXT", "BREAK", "RETURN" }; 


/** Construct from statement type */
SyntaxStatement::SyntaxStatement(statementT type)
    : SyntaxElement(), statementType(type), expression(NULL), statements1(NULL), statements2(NULL) {
}


/** Construct from statement type and expression (RETURN expression) */
SyntaxStatement::SyntaxStatement(statementT type, SyntaxElement* expr)
    : SyntaxElement(), statementType(type), expression(expr), statements1(NULL), statements2(NULL) {
}


/** Construct from statement type, condition and statement list */
SyntaxStatement::SyntaxStatement(statementT type, SyntaxElement* cond, std::list<SyntaxElement*>* stmts)
    : SyntaxElement(), statementType(type), expression(cond), statements1(stmts), statements2(NULL) {
}


/** Construct from statement type, condition and itwo statement lists */
SyntaxStatement::SyntaxStatement(statementT                 type,
                                 SyntaxElement*             cond,
                                 std::list<SyntaxElement*>* stmts1,
                                 std::list<SyntaxElement*>* stmts2)
    : SyntaxElement(), statementType(type), expression(cond), statements1(stmts1), statements2(stmts2) {
}


/** Deep copy constructor */
SyntaxStatement::SyntaxStatement(const SyntaxStatement& x)
    : SyntaxElement(x) {

    statementType   = x.statementType;
    expression      = x.expression->clone();

    for (std::list<SyntaxElement*>::iterator i=x.statements1->begin(); i!=x.statements1->end(); i++)
        statements1->push_back((*i)->clone());

    for (std::list<SyntaxElement*>::iterator i=x.statements2->begin(); i!=x.statements2->end(); i++)
        statements2->push_back((*i)->clone());
}


/** Destructor deletes expression and statements */
SyntaxStatement::~SyntaxStatement() {
    
    delete expression;

    if (statements1 != NULL) {
        for (std::list<SyntaxElement*>::iterator i=statements1->begin(); i!=statements1->end(); i++)
            delete (*i);
        delete statements1;
    }

    if (statements2 != NULL) {
        for (std::list<SyntaxElement*>::iterator i=statements2->begin(); i!=statements2->end(); i++)
            delete (*i);
        delete statements2;
    }
}


/** Return brief info about object */
std::string SyntaxStatement::briefInfo () const {

    std::ostringstream   o;
    o << "SyntaxStatement: type = " << stmtName[statementType];

    return o.str();
}


/** Clone syntax element */
SyntaxElement* SyntaxStatement::clone () const {

    return (SyntaxElement*)(new SyntaxStatement(*this));
}


/** Equals comparison */
bool SyntaxStatement::equals(const SyntaxElement* x) const {

	const SyntaxStatement* p = dynamic_cast<const SyntaxStatement*>(x);
    if (p == NULL)
        return false;

    bool result = true;
    result = result && expression->equals(p->expression);
 
    if (statements1 != NULL) {
        std::list<SyntaxElement*>::const_iterator i, j;
        for (i=statements1->begin(), j=p->statements1->begin(); i!=statements1->end(); ++i, ++j)
            result = result && ((*i)->equals(*j));
    }

    if (statements2 != NULL) {
        std::list<SyntaxElement*>::const_iterator i, j;
        for (i=statements2->begin(), j=p->statements2->begin(); i!=statements2->end(); ++i, ++j)
            result = result && ((*i)->equals(*j));
    }

    return result;
}


/** Convert element to DAG node; inapplicable, so return NULL */
DAGNode* SyntaxStatement::getDAGNode(Frame* frame) const {

    return NULL;
}


/** Get semantic value: it is here that we execute the statement */
RbObject* SyntaxStatement::getValue(Frame* frame) const {

    if (statementType == For) {

        // Execute expression and hope to find one or more values in a vector

        // If the variable symbol is not in the environment, create a new variable in a new frame

        // Cycle through the statements, assigning each one of the values in turn to the variable
    }

    return NULL;
}


/** Print info about the syntax element */
void SyntaxStatement::print(std::ostream& o) const {

    o << "SyntaxStatement:" << std::endl;
    o << "statementType = " << stmtName[statementType] << std::endl;
    if (expression == NULL)
        o << "expression    = NULL" << std::endl;
    else
        o << "expression    = " << expression->briefInfo() << std::endl;
    if (statements1 == NULL)
        o << "statements1   = NULL" << std::endl;
    else
        o << "statements1   = <" << statements1->size() << " statements" << std::endl;
    if (statements2 == NULL)
        o << "statements2   = NULL" << std::endl;
    else
        o << "statements2   = <" << statements2->size() << " statements" << std::endl;
}


