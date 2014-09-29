#ifndef SyntaxControlAssignment_H
#define SyntaxControlAssignment_H

#include "SyntaxAssignment.h"

#include <iostream>
#include <list>
#include <string>

namespace RevLanguage {
    
    /**
     * @brief Control variable assignment
     *
     * This syntax element class is used to hold control variable
     * assignment statements. The member variables are the lhs
     * (left-hand side) and rhs (righ-hand side) expressions.
     *
     * A control variable assignment is a statement like
     *
     *    i <<- 1
     *
     * using the double left-arrow assignment operator.
     *
     * A control variable assignment differs from a constant
     * assignment only in that the control variable flag of the 
     * left-hand side variable is set to true. This means that the
     * variable will not be included in any DAGs even if it occurs
     * in a dynamic (deterministic or stochastic) expression.
     *
     * The control variable behavior is typically used for loop
     * variables. The for loop variable is implicitly set to be
     * a control variable.
     */
    class SyntaxControlAssignment : public SyntaxAssignment {
        
    public:
        SyntaxControlAssignment(SyntaxElement* lhsExpr, SyntaxElement* rhsExpr);                        //!< Basic constructor
        
	    virtual                    ~SyntaxControlAssignment();                                          //!< Destructor
        
        // Basic utility functions
        SyntaxControlAssignment*    clone() const;                                                      //!< Clone object
        
    protected:
        void                        assign(RevPtr<Variable> &lhs, RevPtr<Variable> &rhs);               //!< The assignment operation.

    };
    
}

#endif

