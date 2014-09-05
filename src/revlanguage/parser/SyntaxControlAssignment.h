#ifndef SyntaxControlAssignment_H
#define SyntaxControlAssignment_H

#include "SyntaxElement.h"
#include "SyntaxVariable.h"

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
    class SyntaxControlAssignment : public SyntaxElement {
        
    public:
        SyntaxControlAssignment(SyntaxElement* lhsExpr, SyntaxElement* rhsExpr);                        //!< Basic constructor
        SyntaxControlAssignment(const SyntaxControlAssignment& x);                                      //!< Copy constructor
        
	    virtual                    ~SyntaxControlAssignment();                                          //!< Destructor
        
        // Assignment operator
        SyntaxControlAssignment&    operator=(const SyntaxControlAssignment& x);                        //!< Assignment operator
        
        // Basic utility functions
        SyntaxControlAssignment*    clone() const;                                                      //!< Clone object
        bool                        isAssignment(void) const;                                           //!< Is this an assignment statement?
        void                        printValue(std::ostream& o) const;                                  //!< Print info about object
        
        // Regular functions
        RevPtr<Variable>            evaluateContent(Environment& env);                                  //!< Get semantic value
        bool                        isFunctionSafe(const Environment&       env,
                                                   std::set<std::string>&   localVars) const;           //!< Is this element safe in a function?
        
    protected:
        SyntaxElement*              lhsExpression;                                                      //!< The lhs expression
        SyntaxElement*              rhsExpression;                                                      //!< The rhs expression
        
    };
    
}

#endif

