/**
 * @file
 * This file contains the declaration of SyntaxVariable, which is
 * used to hold variable references in the syntax tree.
 *
 * @brief Declaration of SyntaxVariable
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef SyntaxVariable_H
#define SyntaxVariable_H

#include "ModelVector.h"
#include "Natural.h"
#include "SyntaxElement.h"

#include <iostream>
#include <list>

namespace RevLanguage {

class SyntaxFunctionCall;


    /**
     * This is the class used to hold variables in the syntax tree.
     *
     * We store the identifier, the index vector and the base variable
     * here so that we can wrap these things into a DAG node expression
     * if needed.
     *
     * The variable class uses three different functions to evaluate its content.
     * If the variable is part of a left-hand side expression, it is evaluated
     * using evaluateLHSContent(). If it is part of a dynamic rhs expression,
     * it is evaluated using evaluateDynamicContent(), and if it is in a static
     * rhs expression, it is evaluated using evaluateContent().
     */
    class SyntaxVariable : public SyntaxElement {

    public:
        SyntaxVariable(const std::string &n, std::list<SyntaxElement*>* indx);                                                      //!< Global variable
        SyntaxVariable(SyntaxFunctionCall* fxnCall, std::list<SyntaxElement*>* indx);                                               //!< Global function call variable expression
        SyntaxVariable(SyntaxElement* expr, std::list<SyntaxElement*>* indx);                                                       //!< Global (...)[..]... variable expression
        SyntaxVariable(SyntaxVariable* baseVar, const std::string &n, std::list<SyntaxElement*>* indx);                             //!< Member variable
        SyntaxVariable(SyntaxVariable* baseVar, SyntaxFunctionCall* fxnCall, std::list<SyntaxElement*>* indx);                      //!< Member function call variable expression
        SyntaxVariable(const SyntaxVariable& x);                                                                                    //!< Copy constructor
	   
        virtual                            ~SyntaxVariable(void);                                                                   //!< Destructor deletes variable, identifier and index

        // Assignment operator
        SyntaxVariable&                     operator=(const SyntaxVariable& x);                                                     //!< Assignment operator

        // Basic utility functions
        SyntaxVariable*                     clone(void) const;                                                                      //!< Clone object
        void                                printValue(std::ostream& o) const;                                                      //!< Print info about object

        // Regular functions
        std::vector<RevPtr<Variable> >      computeDynamicIndex(Environment& env);                                                  //!< Evaluate index variables (dynamic case)
        std::vector<size_t>                 computeIndex(Environment& env);                                                         //!< Evaluate index (constant case)
        RevPtr<Variable>                    evaluateContent(Environment& env);                                                      //!< Get semantic rhs value (constant case)
        RevPtr<Variable>                    evaluateDynamicContent(Environment& env);                                               //!< Get semantic rhs value (dynamic case)
        RevPtr<Variable>                    evaluateLHSContent(Environment& env, const std::string& varType);                       //!< Get semantic lhs value
        const std::string&                  getIdentifier(void) { return identifier; }                                              //!< Get identifier
        std::string                         getFullName(Environment& env) const;                                                    //!< Get full name, with indices and base obj
        bool                                hasExpression(void) const { return expression != NULL; }                                //!< Does the variable have an expression (e.g. (a+b)[1] )
        bool                                hasFunctionCall(void) const { return functionCall != NULL; }                            //!< Does the variable have a function call (e.g. foo()[1] )
        bool                                isMemberVariable(void) const { return baseVariable != NULL; }                           //!< Is the variable a member variable?

    protected:
        std::string                         identifier;                                                                             //!< The name of the variable, if identified by name
        SyntaxFunctionCall*                 functionCall;                                                                           //!< Function call part of variable expression
        SyntaxElement*                      expression;                                                                             //!< Expression part of variable expression
        std::list<SyntaxElement*>*          index;                                                                                  //!< Vector of Natural indices to variable element
        SyntaxVariable*                     baseVariable;                                                                           //!< Base variable (pointing to a Rev member object)
    };

}

#endif

