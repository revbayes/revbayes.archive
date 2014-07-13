#ifndef SyntaxAdditionAssignment_H
#define SyntaxAdditionAssignment_H

#include "SyntaxElement.h"
#include "SyntaxVariable.h"

#include <iostream>
#include <list>
#include <string>

namespace RevLanguage {
    
    /**
     * @brief Addition assignment operator ('a += b')
     *
     * The addition assignment operator adds the value of the right hand side variable
     * to the value of the left hand side variable.
     *
     * Note that the left hand side variable must be constant and that this function
     * is a constant expression and thus evaluated immidiately. The reason is that otherwise
     * we would obtain a loop in the DAG, namely that 'a' is a parent of itself.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-02-18, version 1.0
     *
     */
    class SyntaxAdditionAssignment : public SyntaxElement {
        
    public:
        SyntaxAdditionAssignment(SyntaxVariable* var, SyntaxElement* expr);                                                     //!< Constructor with lhs = variable
        SyntaxAdditionAssignment(SyntaxFunctionCall* fxnCall, SyntaxElement* expr);                                             //!< Constructor with lhs = function call
        SyntaxAdditionAssignment(const SyntaxAdditionAssignment& x);                                                            //!< Copy constructor
	    virtual                             ~SyntaxAdditionAssignment();                                                        //!< Destructor
        
        // Assignment operator
        SyntaxAdditionAssignment&           operator=(const SyntaxAdditionAssignment& x);                                       //!< Assignment operator
        
        // Basic utility functions
        SyntaxAdditionAssignment*           clone() const;                                                                      //!< Clone object
        bool                                isAssignment(void) const;                                                           //!< Is this syntax element an assignment?
        void                                printValue(std::ostream& o) const;                                                  //!< Print info about object
        
        // Regular functions
        RevPtr<Variable>                    evaluateContent(Environment& env);                                                  //!< Get semantic value
        
    protected:
        SyntaxVariable*                     variable;                                                                           //!< A lhs variable
        SyntaxFunctionCall*                 functionCall;                                                                       //!< A lhs function call
        SyntaxElement*                      expression;                                                                         //!< The rhs expression
        
    };
    
}

#endif

