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
#include "SyntaxFunctionDef.h"
#include "UserFunction.h"
#include "Workspace.h"

#include <sstream>


/** Construct global function call from function name and arguments */
SyntaxFunctionDef::SyntaxFunctionDef( RbString* type,
                                      RbString* name,
                                      std::list<SyntaxFormal*>* formals,
                                      std::list<SyntaxElement*>* stmts)
                    : SyntaxElement(), functionName(name), formalArgs(formals), code(stmts) {

                        
    if (type != NULL) {
        const std::string   typeString  = *type;
                            returnType  = new TypeSpec( Workspace::userWorkspace().getClassTypeSpecOfType(typeString) );
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

    }
    else {
        // Create the type specification
        returnType = new TypeSpec( RbLanguageObject::getClassTypeSpec() );
    }
}


/** Deep copy constructor */
SyntaxFunctionDef::SyntaxFunctionDef(const SyntaxFunctionDef& x) : SyntaxElement(x) {

    returnType   = new TypeSpec(*x.returnType);
    functionName = new RbString(*functionName);
 
    for (std::list<SyntaxFormal*>::const_iterator i=x.formalArgs->begin(); i!=x.formalArgs->end(); i++)
        formalArgs->push_back((*i)->clone());

    for (std::list<SyntaxElement*>::const_iterator i=x.code->begin(); i!=x.code->end(); i++)
        code->push_back( (*i)->clone() );
}


/** Destructor deletes members */
SyntaxFunctionDef::~SyntaxFunctionDef() {
    
    delete returnType;
    delete functionName;
    
    for (std::list<SyntaxFormal*>::iterator i=formalArgs->begin(); i!=formalArgs->end(); i++) {
        delete *i;
    }
    delete formalArgs;
    
    for (std::list<SyntaxElement*>::iterator i=code->begin(); i!=code->end(); i++) {
        delete *i;
    }
    delete code;
}


/** Assignment operator */
SyntaxFunctionDef& SyntaxFunctionDef::operator=(const SyntaxFunctionDef& x) {

    if (&x != this) {

        SyntaxElement::operator=(x);

        returnType   = new TypeSpec(*x.returnType);
        functionName = x.functionName->clone();
        
        formalArgs->clear();
        for (std::list<SyntaxFormal*>::const_iterator i=x.formalArgs->begin(); i!=x.formalArgs->end(); i++)
            formalArgs->push_back((*i)->clone());
        
        code->clear();
        for (std::list<SyntaxElement*>::const_iterator i=x.code->begin(); i!=x.code->end(); i++)
            code->push_back( (*i)->clone() );
        
    }

    return (*this);
}


/** Clone syntax element */
SyntaxFunctionDef* SyntaxFunctionDef::clone () const {

    return new SyntaxFunctionDef(*this);
}


/** Get class name of object */
const std::string& SyntaxFunctionDef::getClassName(void) { 
    
    static std::string rbClassName = "Function definition";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& SyntaxFunctionDef::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( SyntaxElement::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& SyntaxFunctionDef::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Get semantic value: insert a user-defined function in the user workspace */
RbVariablePtr SyntaxFunctionDef::evaluateContent(Environment& env) {

    // Get argument rules from the formals
    ArgumentRules* argRules = new ArgumentRules();

    for (std::list<SyntaxFormal*>::iterator i=formalArgs->begin(); i!=formalArgs->end(); i++)
        argRules->push_back( (*i)->getArgumentRule()->clone() );

    // Create copy of the statements
    std::list<SyntaxElement*>* stmts = new std::list<SyntaxElement*>();
    for(std::list<SyntaxElement*>::const_iterator i=code->begin(); i!=code->end(); i++)
        stmts->push_back((*i)->clone());

    // Create an environment with the parent environment being the one in which the function was defined
    Environment* defineEnvironment = new Environment(env);

    // Create the function
    RbFunction* theFunction = new UserFunction(argRules, *returnType, stmts, defineEnvironment);

    // insert in the user workspace
    Workspace::userWorkspace().addFunction(*functionName, theFunction);

    // No return value 
    return RbVariablePtr( NULL );
}


/** Print info about the syntax element */
void SyntaxFunctionDef::printValue(std::ostream& o) const {

    o << "SyntaxFunctionDef:" << std::endl;
    o << "returnType   = " << returnType->toString() << std::endl;
    o << "functionName = ";
    functionName->printValue(o);
    o << std::endl;
    o << "formalArgs   = <" << formalArgs->size() << " formals (argument rules)>" << std::endl;
    o << "code         = <" << code->size() << " code statements>" << std::endl;
}

