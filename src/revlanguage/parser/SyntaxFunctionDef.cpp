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

#include "Environment.h"
//#include "GeneralUnivariateFunction.h"
#include "RbException.h"
#include "RbUtil.h"
#include "SyntaxFunctionDef.h"
#include "UserFunction.h"
#include "Workspace.h"

#include <sstream>

using namespace RevLanguage;

/** Construct global function call from function name and arguments */
SyntaxFunctionDef::SyntaxFunctionDef( const std::string &type, const std::string &name, std::list<SyntaxFormal*>* formals, std::list<SyntaxElement*>* stmts) : SyntaxElement(), functionName(name), formalArgs(formals), code(stmts), returnType( type ) {

    if ( type == "" ) {
        returnType = RevObject::getClassTypeSpec();
    }
    else {
        returnType  = TypeSpec( Workspace::userWorkspace().getClassTypeSpecOfType(type) );
    }
    int         nDim    = 0;
    bool        isRef  = false;
    std::string tpName  = std::string();
    for (std::string::const_iterator i=type.begin(); i!=type.end(); i++) {
        if ((*i) == '[')
            nDim++;
        else if ((*i) == '&')
            isRef = true;
        else if ((*i) != ']')
            tpName += (*i);
    }

}


/** Deep copy constructor */
SyntaxFunctionDef::SyntaxFunctionDef(const SyntaxFunctionDef& x) : SyntaxElement(x), returnType( x.returnType ) {

    functionName = functionName;
 
    for (std::list<SyntaxFormal*>::const_iterator i=x.formalArgs->begin(); i!=x.formalArgs->end(); i++)
        formalArgs->push_back((*i)->clone());

    for (std::list<SyntaxElement*>::const_iterator i=x.code->begin(); i!=x.code->end(); i++)
        code->push_back( (*i)->clone() );
}


/** Destructor deletes members */
SyntaxFunctionDef::~SyntaxFunctionDef() {
    
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

        returnType   = x.returnType;
        functionName = x.functionName;
        
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



/** Get semantic value: insert a user-defined function in the user workspace */
RbPtr<Variable> SyntaxFunctionDef::evaluateContent(Environment& env) {

    // Get argument rules from the formals
    ArgumentRules* argRules = new ArgumentRules();

    for (std::list<SyntaxFormal*>::iterator i=formalArgs->begin(); i!=formalArgs->end(); i++)
        argRules->push_back( (*i)->getArgumentRule()->clone() );

    // Create copy of the statements
    std::list<SyntaxElement*>* stmts = new std::list<SyntaxElement*>();
    for(std::list<SyntaxElement*>::const_iterator i=code->begin(); i!=code->end(); i++)
        stmts->push_back((*i)->clone());

    // Create an environment with the parent environment being the one in which the function was defined
    Environment* defineEnvironment = new Environment(&env);
    
//    bool univariate = false;
//    if ( argRules->size() == 1 && (*argRules)[0].getArgumentTypeSpec() == Real::getClassTypeSpec() ) {
//        if (returnType.isDerivedOf( Real::getClassTypeSpec() ) ) {
//            univariate = true;
//        }
//    }
//    
//    if ( univariate ) {    
//        // Create the function
////        Function* theFunction = new GeneralUnivariateUserFunction(argRules, returnType, stmts, defineEnvironment);
////        
////        // insert in the (user) workspace
////        env.addFunction(functionName, theFunction);
//
//    } else {   
        // Create the function
        Function* theFunction = new UserFunction(argRules, returnType, stmts, defineEnvironment);
        
        // insert in the (user) workspace
        env.addFunction(functionName, theFunction);

//    }

    // No return value 
    return NULL;
}


/** Print info about the syntax element */
void SyntaxFunctionDef::printValue(std::ostream& o) const {

    o << "SyntaxFunctionDef:" << std::endl;
    o << "returnType   = " << returnType.toString() << std::endl;
    o << "functionName = " << functionName;
    o << std::endl;
    o << "formalArgs   = <" << formalArgs->size() << " formals (argument rules)>" << std::endl;
    o << "code         = <" << code->size() << " code statements>" << std::endl;
}


/**
 * Replace the syntax variable with name by the constant value. Loops have to do that for their index variables.
 * We just delegate that to the arguments.
 * 
 * \TODO: Currently we do not replace the variable inside the function
 */
void SyntaxFunctionDef::replaceVariableWithConstant(const std::string& name, const RevObject& c) {
    
    for (std::list<SyntaxFormal*>::iterator i = formalArgs->begin(); i != formalArgs->end(); i++) {
        (*i)->replaceVariableWithConstant(name, c);
    }
}

