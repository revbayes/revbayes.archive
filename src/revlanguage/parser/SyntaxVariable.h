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
     * We store the identifier and the base variable
     * here so that we can wrap these things into a DAG node expression
     * if needed.
     *
     * The variable class uses three different functions to evaluate its content.
     * If the variable is part of a left-hand side expression, it is evaluated
     * using evaluateLHSContent(). If it is part of a dynamic rhs expression,
     * it is evaluated using evaluateContent(dynamic=true), and if it is in a static
     * rhs expression, it is evaluated using evaluateContent().
     */
    class SyntaxVariable : public SyntaxElement {
        
    public:
        SyntaxVariable(const std::string &n);                                                                                       //!< Global variable
        SyntaxVariable(SyntaxElement* baseVar, const std::string &n);                                                               //!< Member variable
        SyntaxVariable(const SyntaxVariable& x);                                                                                    //!< Copy constructor
        
        virtual                            ~SyntaxVariable(void);                                                                   //!< Destructor deletes variable, identifier and index
        
        // Assignment operator
        SyntaxVariable&                     operator=(const SyntaxVariable& x);                                                     //!< Assignment operator
        
        // Basic utility functions
        SyntaxVariable*                     clone(void) const;                                                                      //!< Clone object
        void                                printValue(std::ostream& o) const;                                                      //!< Print info about object
        
        // Regular functions
        RevPtr<Variable>                    evaluateContent(Environment& env, bool dynamic=false);                                  //!< Get semantic rhs value
        RevPtr<Variable>                    evaluateLHSContent(Environment& env, const std::string& varType);                       //!< Get semantic lhs value
        const std::string&                  getIdentifier(void) { return identifier; }                                              //!< Get identifier
        std::string                         getFullName(Environment& env) const;                                                    //!< Get full name, with indices and base obj
        bool                                isFunctionSafe(const Environment&       env,
                                                           std::set<std::string>&   localVars) const;                               //!< Is this element safe in a function?
        bool                                isMemberVariable(void) const { return baseVariable != NULL; }                           //!< Is the variable a member variable?
        bool                                retrievesExternVar(const Environment&       env,
                                                               std::set<std::string>&   localVars,
                                                               bool                     inLHS) const;                               //!< Does this element retrieve an external variable?
        
    protected:

        std::string                         identifier;                                                                             //!< The name of the variable, if identified by name
        SyntaxElement*                      baseVariable;                                                                           //!< Base variable (pointing to a Rev member object)
    
    };
    
}

#endif


