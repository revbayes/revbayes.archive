#ifndef SyntaxDivisionAssignment_H
#define SyntaxDivisionAssignment_H

#include "SyntaxElement.h"
#include "SyntaxVariable.h"

#include <iostream>
#include <list>
#include <string>

namespace RevLanguage {
    
    /**
     * @brief Division assignment operator ('a /= b')
     *
     * The division assignment operator divides the value the left-hand side variable
     * with the right-hand side variable.
     *
     * Note that the left-hand side variable must be either a constant variable or a
     * control variable, and that the division assignment is interpreted as a constant
     * assignment or a control variable assignment depending on the original type of the
     * left-hand side variable. Thus, if the left-hand side variable is a constant, the
     * division assignment statement 'a /= b' is equivalent to the statement:
     *
     *    a <- a / b
     *
     * while if the left-hand side variable is a control variable, the addition assignment
     * is equivalent to
     *
     *    a <<- a / b
     *
     * Note that a division assignment could not be a deterministic assignment, because
     * then it would be rejected as creating loops in the DAG, i.e., 'a' would be a parent
     * of itself.
     */
    class SyntaxDivisionAssignment : public SyntaxElement {
        
    public:
        SyntaxDivisionAssignment(SyntaxElement* lhsExpr, SyntaxElement* rhsExpr);                           //!< Standard constructor
        SyntaxDivisionAssignment(const SyntaxDivisionAssignment& x);                                        //!< Copy constructor
	    virtual                             ~SyntaxDivisionAssignment();                                    //!< Destructor
        
        // Assignment operator
        SyntaxDivisionAssignment&           operator=(const SyntaxDivisionAssignment& x);                   //!< Assignment operator
        
        // Basic utility functions
        SyntaxDivisionAssignment*           clone() const;                                                  //!< Clone object
        bool                                isAssignment(void) const;                                       //!< Is this syntax element an assignment?
        void                                printValue(std::ostream& o) const;                              //!< Print info about object
        
        // Regular functions
        RevPtr<Variable>                    evaluateContent(Environment& env);                              //!< Get semantic value
        
    protected:
        SyntaxElement*                      lhsExpression;                                                  //!< The lhs expression
        SyntaxElement*                      rhsExpression;                                                  //!< The rhs expression
        
    };
    
}

#endif

