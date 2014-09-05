#ifndef SyntaxSubtractionAssignment_H
#define SyntaxSubtractionAssignment_H

#include "SyntaxElement.h"
#include "SyntaxVariable.h"

#include <iostream>
#include <list>
#include <string>

namespace RevLanguage {
    
    /**
     * @brief Subtraction assignment operator ('a -= b')
     *
     * The subtraction assignment operator subtracts the value of the right-hand side variable
     * from the value of the left-hand side variable.
     *
     * Note that the left-hand side variable must be either a constant variable or a
     * control variable, and that the subtraction assignment is interpreted as a constant
     * assignment or a control variable assignment depending on the original type of the
     * left-hand side variable. Thus, if the left-hand side variable is a constant, the
     * subtraction assignment statement 'a -= b' is equivalent to the statement:
     *
     *    a <- a - b
     *
     * while if the left-hand side variable is a control variable, the subtraction assignment
     * is equivalent to
     *
     *    a <<- a - b
     *
     * Note that a subtraction assignment could not be a deterministic assignment, because
     * then it would be rejected as creating loops in the DAG, i.e., 'a' would be a parent
     * of itself.
     */
    class SyntaxSubtractionAssignment : public SyntaxElement {
        
    public:
        SyntaxSubtractionAssignment(SyntaxElement* lhsExpr, SyntaxElement* rhsExpr);                        //!< Standard constructor
        SyntaxSubtractionAssignment(const SyntaxSubtractionAssignment& x);                                  //!< Copy constructor
	    virtual                            ~SyntaxSubtractionAssignment();                                  //!< Destructor
        
        // Assignment operator
        SyntaxSubtractionAssignment&        operator=(const SyntaxSubtractionAssignment& x);                //!< Assignment operator
        
        // Basic utility functions
        SyntaxSubtractionAssignment*        clone() const;                                                  //!< Clone object
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

