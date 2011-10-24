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

#include "ValueRule.h"
#include "DAGNode.h"
#include "Environment.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbObject.h"
#include "VectorString.h"
#include "SyntaxFunctionDef.h"
#include "UserFunction.h"
#include "Workspace.h"

#include <sstream>


// Definition of the static type spec member
const TypeSpec SyntaxFunctionDef::typeSpec(SyntaxFunctionDef_name);

/** Construct global function call from function name and arguments */
SyntaxFunctionDef::SyntaxFunctionDef(   RbPtr<RbString> type,
                                        RbPtr<RbString> name,
                                        RbPtr<std::list<RbPtr<SyntaxFormal> > > formals,
                                        RbPtr<std::list<RbPtr<SyntaxElement> > > stmts)
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
        returnType = RbPtr<TypeSpec>( new TypeSpec( tpName ) );
    }
}


/** Deep copy constructor */
SyntaxFunctionDef::SyntaxFunctionDef(const SyntaxFunctionDef& x) : SyntaxElement(x) {

    returnType   = RbPtr<TypeSpec>( new TypeSpec(*x.returnType) );
    functionName = RbPtr<RbString>( new RbString(*functionName) );
 
    for (std::list<RbPtr<SyntaxFormal> >::const_iterator i=x.formalArgs->begin(); i!=x.formalArgs->end(); i++)
        formalArgs->push_back(RbPtr<SyntaxFormal>((*i)->clone()));

    for (std::list<RbPtr<SyntaxElement> >::const_iterator i=x.code->begin(); i!=x.code->end(); i++)
        code->push_back(RbPtr<SyntaxElement>( (*i)->clone() ));
}


/** Destructor deletes members */
SyntaxFunctionDef::~SyntaxFunctionDef() {
    
}


/** Assignment operator */
SyntaxFunctionDef& SyntaxFunctionDef::operator=(const SyntaxFunctionDef& x) {

    if (&x != this) {

        SyntaxElement::operator=(x);

        returnType   = x.returnType;
        functionName = x.functionName;
        formalArgs   = x.formalArgs;
        code         = x.code;
        
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


/** Get semantic value: insert a user-defined function in the user workspace */
RbPtr<Variable> SyntaxFunctionDef::getContentAsVariable(RbPtr<Environment> env) const {

    // Get argument rules from the formals
    static ArgumentRules argRules;

    for (std::list<RbPtr<SyntaxFormal> >::const_iterator i=formalArgs->begin(); i!=formalArgs->end(); i++)
        argRules.push_back( (*i)->getArgumentRule(env) );

    // Create copy of the statements
    RbPtr<std::list<RbPtr<SyntaxElement> > > stmts( new std::list<RbPtr<SyntaxElement> >() );
    for(std::list<RbPtr<SyntaxElement> >::const_iterator i=code->begin(); i!=code->end(); i++)
        stmts->push_back((*i));

    // Create an environment with the parent environment being the one in which the function was defined
    RbPtr<Environment> defineEnvironment( new Environment(env) );

    // Create the function
    RbPtr<UserFunction> theFunction( new UserFunction(argRules, *returnType, stmts, defineEnvironment) );

    // Insert in the user workspace
    Workspace::userWorkspace()->addFunction(*functionName, theFunction);

    // No return value 
    return NULL;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& SyntaxFunctionDef::getTypeSpec(void) const {
    return typeSpec;
}


/** Print info about the syntax element */
void SyntaxFunctionDef::print(std::ostream& o) const {

    o << "SyntaxFunctionDef:" << std::endl;
    o << "returnType   = " << returnType->toString() << std::endl;
    o << "functionName = ";
    functionName->printValue(o);
    o << std::endl;
    o << "formalArgs   = <" << formalArgs->size() << " formals (argument rules)>" << std::endl;
    o << "code         = <" << code->size() << " code statements>" << std::endl;
}

