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
     * The variable expression can be one of three types: a named variable
     * a function-call variable expression, or a generic variable expression.
     * In the first case, the variable is retrieved from the environment usign
     * an identifier. These are expressions of the type:
     *
     *    a[1]
     *
     * A function-call variable expression is an expression of the type:
     *
     *    foo(a)[1]
     *
     * A function-call variable expression must have at least one element
     * specification, otherwise it is parsed as a regular function call.
     *
     * Finally, a generic variable expression is an expression of the type:
     *
     *    (a+b)[1]
     *
     * Note that the parentheses are required in this case. Like function-call
     * variable expressions, generic variable expressions must include at least
     * one element specificaton. Otherwise, they are parsed as regular expressions.
     *
     * A variable expression may be combined with a base variable expression
     * so that it retrieves a member variable instead of an environment variable.
     * These are expressions of the type:
     * 
     *    a.b
     *    foo(a).b
     *    (a+b).c
     *
     * These types of statements can of course be chained into longer
     * variable expressions containing a series of consecutive base variable
     * expressions terminated by a dot. The chain itself must be terminated by
     * a simple variable expression. Special rules apply for these chains. The
     * first base variable element can either be a named variable, a function call
     * or a generic expression enclosed in parentheses. In contrast to the case of
     * simple variable expressions, function-call and generic base variable
     * expressions need not be followed by any element specifications.
     * 
     * Any subsequent base variable expressions must either be a function-call
     * expression or a named variable expression. Generic variable expressions are
     * not allowed. Each base variable expression is terminated by a dot.
     * 
     * The terminating simple variable expression must either be a named variable
     * expression or a function-call variable expression. If it is a function-call
     * variable expression, it must be associated with at least one element
     * specification, or it is parsed as a member function call.
     *
     * An element specification is an expression surrounded by square brackets. The
     * expression may be omitted, in which case a slice in that dimension is indicated.
     * For instance, 'a[]' would retrieve the entire vector if 'a' were a vector, and 
     * 'b[][4]' would retrieve the fourth column vector if 'b' were a matrix.
     */
    class SyntaxVariable : public SyntaxElement {

    public:
        SyntaxVariable(const std::string &n, std::list<SyntaxElement*>* indx);                                                      //!< Named variable: 'a[1]'
        SyntaxVariable(SyntaxFunctionCall* fxnCall, std::list<SyntaxElement*>* indx);                                               //!< Function-call variable expression: 'f(a)[1]'
        SyntaxVariable(SyntaxElement* expr, std::list<SyntaxElement*>* indx);                                                       //!< Generic variable expression: '(a+b)[1]'
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
        bool                                isFunctionSafe(const Environment& env) const;                                           //!< Is this element safe in a function?
        bool                                isMemberVariable(void) const { return baseVariable != NULL; }                           //!< Is the variable a member variable?
        bool                                retrievesExternVar(const Environment& env) const;                                       //!< Does this element retrieve an external variable?
        void                                setBaseVariable( SyntaxVariable* var);                                                  //!< Set the base variable
        
    protected:
        std::string                         identifier;                                                                             //!< The name of the variable, if identified by name
        SyntaxFunctionCall*                 functionCall;                                                                           //!< Function call part of variable expression
        SyntaxElement*                      expression;                                                                             //!< Expression part of variable expression
        std::list<SyntaxElement*>*          index;                                                                                  //!< Vector of Natural indices to variable element
        SyntaxVariable*                     baseVariable;                                                                           //!< Base variable (pointing to a Rev member object)
    };

}

#endif

