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
     * The addition assignment operator adds the value of the right-hand side variable
     * to the value of the left-hand side variable.
     *
     * Note that the left-hand side variable must be either a constant variable or a 
     * control variable, and that the addition assignment is interpreted as a constant
     * assignment or a control variable assignment depending on the original type of the
     * left-hand side variable. Thus, if the left-hand side variable is a constant, the
     * addition assignment statement 'a += b' is equivalent to the statement:
     *
     *    a <- a + b
     *
     * while if the left-hand side variable is a control variable, the addition assignment
     * is equivalent to
     *
     *    a <<- a + b
     *
     * Note that an addition assignment could not be a deterministic assignment, because
     * then it would be rejected as creating loops in the DAG, i.e., 'a' would be a parent
     * of itself.
     */
    class SyntaxAdditionAssignment : public SyntaxElement {
        
    public:
        SyntaxAdditionAssignment(SyntaxElement* lhsExpr, SyntaxElement* rhsExpr);                           //!< Standard constructor
        SyntaxAdditionAssignment(const SyntaxAdditionAssignment& x);                                        //!< Copy constructor
	    virtual                             ~SyntaxAdditionAssignment();                                    //!< Destructor
        
        // Assignment operator
        SyntaxAdditionAssignment&           operator=(const SyntaxAdditionAssignment& x);                   //!< Assignment operator
        
        // Basic utility functions
        SyntaxAdditionAssignment*           clone() const;                                                  //!< Clone object
        bool                                isAssignment(void) const;                                       //!< Is this syntax element an assignment?
        void                                printValue(std::ostream& o) const;                              //!< Print info about object
        
        // Regular functions
        RevPtr<Variable>                    evaluateContent(Environment& env);                              //!< Get semantic value
        bool                                isFunctionSafe(const Environment&       env,
                                                           std::set<std::string>&   localVars) const;       //!< Is this element safe in a function?
        
    protected:
        SyntaxElement*                      lhsExpression;                                                  //!< The lhs expression
        SyntaxElement*                      rhsExpression;                                                  //!< The rhs expression
        
    };
    
}

#endif

