#ifndef SyntaxMultiplicationAssignment_H
#define SyntaxMultiplicationAssignment_H

#include "SyntaxElement.h"
#include "SyntaxVariable.h"

#include <iostream>
#include <list>
#include <string>

namespace RevLanguage {
    
    /**
     * @brief Multiplication assignment operator ('a *= b')
     *
     * The multiplication assignment operator multiplies the value of the left-hand side
     * variable with the value of the right-hand side variable.
     *
     * Note that the left-hand side variable must be either a constant variable or a
     * control variable, and that the multiplication assignment is interpreted as a constant
     * assignment or a control variable assignment depending on the original type of the
     * left-hand side variable. Thus, if the left-hand side variable is a constant, the
     * multiplication assignment statement 'a *= b' is equivalent to the statement:
     *
     *    a <- a * b
     *
     * while if the left-hand side variable is a control variable, the multiplication assignment
     * is equivalent to
     *
     *    a <<- a * b
     *
     * Note that a multiplication assignment could not be a deterministic assignment, because
     * then it would be rejected as creating loops in the DAG, i.e., 'a' would be a parent
     * of itself.
     */
    class SyntaxMultiplicationAssignment : public SyntaxElement {
        
    public:
        SyntaxMultiplicationAssignment(SyntaxElement* lhsExpr, SyntaxElement* rhsExpr);                     //!< Standard constructor
        SyntaxMultiplicationAssignment(const SyntaxMultiplicationAssignment& x);                            //!< Copy constructor

	    virtual                            ~SyntaxMultiplicationAssignment();                               //!< Destructor
        
        // Assignment operator
        SyntaxMultiplicationAssignment&     operator=(const SyntaxMultiplicationAssignment& x);             //!< Assignment operator
        
        // Basic utility functions
        SyntaxMultiplicationAssignment*     clone() const;                                                  //!< Clone object
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

