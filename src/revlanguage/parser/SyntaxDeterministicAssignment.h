#ifndef SyntaxDeterministicAssignment_H
#define SyntaxDeterministicAssignment_H

#include "SyntaxElement.h"
#include "SyntaxVariable.h"

#include <iostream>
#include <list>
#include <string>

namespace RevLanguage {
    
    /**
     * @brief Deterministic assignments
     *
     * This syntax element class handles deterministic assignments,
     * sometimes called equation assignments. These are assignment
     * statements of the type:
     *
     *    a := foo( x )
     *
     * using the colon-equal or equation assignment operator.
     *
     * The intention of deterministic assignments is to create
     * deterministic variables for inclusion in DAGs. These are
     * typically used for variable conversions, but also for
     * operations involving the construction of complex objects
     * from component parts.
     *
     * Unlike constant variables, deterministic variables change
     * their value when variables included in the right-hand side
     * expression change their value or are reassigned new values.
     *
     * For this reason, the right-hand side expression of a deterministic
     * assignment is referred to as a dynamic expression. Its semantic
     * value is evaluated using the evaluateDynamicContent() function,
     * instead of the standard evaluateContent() function.
     *
     * Many syntax elements evaluate to the same objects in both contexts,
     * for instance constants. For other syntax elements, the behavior
     * changes subtly but with important consequences. This is most
     * obvious in the variable syntax element but also in the function
     * call, unary and binary expression elements.
     */
    class SyntaxDeterministicAssignment : public SyntaxElement {
        
    public:
        SyntaxDeterministicAssignment(SyntaxElement* lhsExpr, SyntaxElement* rhsExpr);                                      //!< Basic constructor
        SyntaxDeterministicAssignment(const SyntaxDeterministicAssignment& x);                                              //!< Copy constructor

	    virtual                             ~SyntaxDeterministicAssignment();                                               //!< Destructor
        
        // Assignment operator
        SyntaxDeterministicAssignment&      operator=(const SyntaxDeterministicAssignment& x);                              //!< Assignment operator
        
        // Basic utility functions
        SyntaxDeterministicAssignment*      clone() const;                                                                  //!< Clone object
        bool                                isAssignment(void) const;
        void                                printValue(std::ostream& o) const;                                              //!< Print info about object
        
        // Regular functions
        RevPtr<Variable>                    evaluateContent(Environment& env);                                              //!< Get semantic value
        bool                                isFunctionSafe(const Environment&       env,
                                                           std::set<std::string>&   localVars) const;                       //!< Is this element safe in a function?
        
    protected:
        SyntaxElement*                      lhsExpression;                                                                  //!< A lhs variable (or NULL)
        SyntaxElement*                      rhsExpression;                                                                  //!< The rhs expression
        
    };
    
}

#endif

