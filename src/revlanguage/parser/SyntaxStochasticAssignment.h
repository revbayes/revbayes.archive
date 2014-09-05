#ifndef SyntaxStochasticAssignment_H
#define SyntaxStochasticAssignment_H

#include "SyntaxElement.h"
#include "SyntaxVariable.h"

#include <iostream>
#include <list>
#include <string>

namespace RevLanguage {
    
    /**
     * @brief Stochastic assignment
     *
     * Objects of this class are used to hold tilde or stochastic
     * assignment statements. The member variables are the lhs
     * (left-hand side) and rhs (righ-hand side) expressions.
     *
     * A stochastic assignment or tilde assignment is a statement
     * of the type
     *
     *    a ~ norm( 0.0, 1.0 )
     *
     * and uses a tilde assignment operator.
     *
     * The rhs expression is evaluated as a dynamic expression.
     * The resulting variable is checked for its type, to make sure
     * that it is a distribution. A random variable is then drawn
     * from the distribution, and it is inserted into the variable
     * resulting from evaluation of the left-hand side expression.
     */
    class SyntaxStochasticAssignment : public SyntaxElement {
        
    public:
        SyntaxStochasticAssignment(SyntaxElement* lhsExpr, SyntaxElement* rhsExpr);                                 //!< Basic constructor
        SyntaxStochasticAssignment(const SyntaxStochasticAssignment& x);                                            //!< Copy constructor
        
	    virtual                        ~SyntaxStochasticAssignment();                                               //!< Destructor
        
        // Assignment operator
        SyntaxStochasticAssignment&     operator=(const SyntaxStochasticAssignment& x);                             //!< Assignment operator
        
        // Basic utility functions
        SyntaxStochasticAssignment*     clone() const;                                                              //!< Clone object
        bool                            isAssignment(void) const;                                                   //!< Is this an assignment statement?
        void                            printValue(std::ostream& o) const;                                          //!< Print info about object
        
        // Regular functions
        RevPtr<Variable>                evaluateContent(Environment& env);                                          //!< Get semantic value
        bool                            isFunctionSafe(const Environment&       env,
                                                       std::set<std::string>&   localVars) const;                   //!< Is this element safe in a function?
        
    protected:
        SyntaxElement*                  lhsExpression;                                                              //!< The lhs expression
        SyntaxElement*                  rhsExpression;                                                              //!< The rhs expression
        
    };
    
}

#endif

