#ifndef SyntaxConstantAssignment_H
#define SyntaxConstantAssignment_H

#include "SyntaxElement.h"
#include "SyntaxVariable.h"

#include <iostream>
#include <list>
#include <string>

namespace RevLanguage {
    
    /**
     * @brief Constant assignment
     *
     * Objects of this class are used to hold left-arrow or constant
     * assignment statements. The member variables are the lhs
     * (left-hand side) and rhs (righ-hand side) expressions.
     *
     * A constant assignment or left-arrow assignment is a statement
     * of the type
     *
     *    a <- x + y
     *
     * and uses a left-arrow assignment operator. The rhs expression
     * is evaluated as a constant or static expression using the
     * standard evaluateContent() function, and not its dynamic or
     * lhs versions.
     */
    class SyntaxConstantAssignment : public SyntaxElement {
        
    public:
        SyntaxConstantAssignment(SyntaxElement* lhsExpr, SyntaxElement* rhsExpr);                                   //!< Basic constructor
        SyntaxConstantAssignment(const SyntaxConstantAssignment& x);                                                //!< Copy constructor

	    virtual                    ~SyntaxConstantAssignment();                                                     //!< Destructor
        
        // Assignment operator
        SyntaxConstantAssignment&   operator=(const SyntaxConstantAssignment& x);                                   //!< Assignment operator
        
        // Basic utility functions
        SyntaxConstantAssignment*   clone() const;                                                                  //!< Clone object
        bool                        isAssignment(void) const;                                                       //!< Is this an assignment statement?
        void                        printValue(std::ostream& o) const;                                              //!< Print info about object
        
        // Regular functions
        RevPtr<Variable>            evaluateContent(Environment& env);                                              //!< Get semantic value
        bool                        isFunctionSafe(const Environment& env) const;                                   //!< Is this element safe in a function?
        
    protected:
        SyntaxElement*              lhsExpression;                                                                  //!< The lhs expression
        SyntaxElement*              rhsExpression;                                                                  //!< The rhs expression
        
    };
    
}

#endif

