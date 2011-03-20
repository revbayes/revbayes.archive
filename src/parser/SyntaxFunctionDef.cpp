/**
 * @file
 * This file contains the implementation of SyntaxFunctionDef, which is
 * used to hold definitions functions (user-defined functions).
 *
 * @brief Implementation of SyntaxFunctionDef
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "DAGNode.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbObject.h"
#include "VectorString.h"
#include "SyntaxFunctionDef.h"
#include "UserFunction.h"
#include "Workspace.h"

#include <sstream>


/** Construct global function call from function name and arguments */
SyntaxFunctionDef::SyntaxFunctionDef(   RbString* type,
                                        RbString* name,
                                        std::list<SyntaxFormal*>* formals,
                                        std::list<SyntaxElement*>* stmts)
    : SyntaxElement(), returnType(new TypeSpec(RbObject_name)), functionName(name), formalArgs(formals), code(stmts) {

    if (type != NULL) {
        const std::string   typeString  = *type;
        int                 nDim        = 0;
        bool                isRef       = false;
        std::string         tpName      = std::string();
        for (std::string::const_iterator i=typeString.begin(); i!=typeString.end(); i++) {
            if ((*i) == '[')
                nDim++;
            else if ((*i) == '&')
                isRef = true;
            else if ((*i) != ']')
                tpName += (*i);
        }

        // Create the type specification
        delete returnType;
        returnType = new TypeSpec(Workspace::userWorkspace().getTypeNameRef(tpName), nDim, isRef);
    }
}


/** Deep copy constructor */
SyntaxFunctionDef::SyntaxFunctionDef(const SyntaxFunctionDef& x)
    : SyntaxElement(x) {

    returnType   = new TypeSpec(x.returnType->getType(), x.returnType->getDim(), x.returnType->isReference());
    functionName = new RbString(*functionName);
 
    for (std::list<SyntaxFormal*>::const_iterator i=x.formalArgs->begin(); i!=x.formalArgs->end(); i++)
        formalArgs->push_back((SyntaxFormal*)((*i)->clone()));

    for (std::list<SyntaxElement*>::const_iterator i=x.code->begin(); i!=x.code->end(); i++)
        code->push_back((*i)->clone());
}


/** Destructor deletes members */
SyntaxFunctionDef::~SyntaxFunctionDef() {
    
    delete returnType;
    delete functionName;

    for (std::list<SyntaxFormal*>::iterator i=formalArgs->begin(); i!=formalArgs->end(); i++)
        delete (*i);
    delete formalArgs;

    for (std::list<SyntaxElement*>::iterator i=code->begin(); i!=code->end(); i++)
        delete (*i);
    delete code;
}


/** Assignment operator */
SyntaxFunctionDef& SyntaxFunctionDef::operator=(const SyntaxFunctionDef& x) {

    if (&x != this) {

        delete returnType;
        delete functionName;

        for (std::list<SyntaxFormal*>::iterator i=formalArgs->begin(); i!=formalArgs->end(); i++)
            delete (*i);
        delete formalArgs;

        for (std::list<SyntaxElement*>::iterator i=code->begin(); i!=code->end(); i++)
            delete (*i);
        delete code;

        SyntaxElement::operator=(x);

        returnType   = new TypeSpec(x.returnType->getType(), x.returnType->getDim(), x.returnType->isReference());
        functionName = new RbString(*functionName);
     
        for (std::list<SyntaxFormal*>::const_iterator i=x.formalArgs->begin(); i!=x.formalArgs->end(); i++)
            formalArgs->push_back((SyntaxFormal*)((*i)->clone()));

        for (std::list<SyntaxElement*>::const_iterator i=x.code->begin(); i!=x.code->end(); i++)
            code->push_back((*i)->clone());
    }

    return (*this);
}


/** Return brief info about object */
std::string SyntaxFunctionDef::briefInfo () const {

    std::ostringstream   o;
    o << "SyntaxFunctionDef:  definition of a funtion called  " << std::string(*functionName);

    return o.str();
}


/** Clone syntax element */
SyntaxFunctionDef* SyntaxFunctionDef::clone () const {

    return new SyntaxFunctionDef(*this);
}


/** Get class vector describing type of object */
const VectorString& SyntaxFunctionDef::getClass(void) const { 

    static VectorString rbClass = VectorString(SyntaxFunctionDef_name) + SyntaxElement::getClass();
	return rbClass; 
}


/** Convert element to DAG node; return NULL since it is not applicable */
DAGNode* SyntaxFunctionDef::getDAGNodeExpr(Frame* formal) const {

    return NULL;
}


/** Get semantic value: insert a user-defined function in the user workspace */
DAGNode* SyntaxFunctionDef::getValue(Frame* frame) const {

    // Get argument rules from the formals
    static ArgumentRules argRules;

    for (std::list<SyntaxFormal*>::iterator i=formalArgs->begin(); i!=formalArgs->end(); i++)
        argRules.push_back( (*i)->getArgumentRule(frame) );

    // Create copy of the statements
    std::list<SyntaxElement*>* stmts = new std::list<SyntaxElement*>();
    for(std::list<SyntaxElement*>::iterator i=code->begin(); i!=code->end(); i++)
        stmts->push_back((*i)->clone());

    // Create copy of the environment in which the function was defined
    Frame* defineEnvironment = frame->cloneEnvironment();

    // Create the function
    UserFunction* theFunction = new UserFunction(argRules, *returnType, stmts, defineEnvironment);

    // Insert in the user workspace
    Workspace::userWorkspace().addFunction(*functionName, theFunction);

    // No return value 
    return NULL;
}


/** Print info about the syntax element */
void SyntaxFunctionDef::print(std::ostream& o) const {

    o << "SyntaxFunctionDef:" << std::endl;
    o << "returnType   = " << returnType->richInfo() << std::endl;
    o << "functionName = ";
    functionName->printValue(o);
    o << std::endl;
    o << "formalArgs   = <" << formalArgs->size() << " formals (argument rules)>" << std::endl;
    o << "code         = <" << code->size() << " code statements>" << std::endl;
}

