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
     * The Division assignment operator adds the value of the right hand side variable
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
    class SyntaxDivisionAssignment : public SyntaxElement {
        
    public:
        SyntaxDivisionAssignment(SyntaxVariable* var, SyntaxElement* expr);                                                     //!< Constructor with lhs = variable
        SyntaxDivisionAssignment(SyntaxFunctionCall* fxnCall, SyntaxElement* expr);                                             //!< Constructor with lhs = function call
        SyntaxDivisionAssignment(const SyntaxDivisionAssignment& x);                                                            //!< Copy constructor
	    virtual                             ~SyntaxDivisionAssignment();                                                        //!< Destructor
        
        // Assignment operator
        SyntaxDivisionAssignment&           operator=(const SyntaxDivisionAssignment& x);                                       //!< Assignment operator
        
        // Basic utility functions
        SyntaxDivisionAssignment*           clone() const;                                                                      //!< Clone object
        bool                                isAssignment(void) const;                                                           //!< Is this syntax element an assignment?
        void                                printValue(std::ostream& o) const;                                                  //!< Print info about object
        
        // Regular functions
        RbPtr<Variable>                     evaluateContent(Environment& env);                                                  //!< Get semantic value
        void                                replaceVariableWithConstant(const std::string& name, const RbLanguageObject& c);    //!< Replace the syntax variable with name by the constant value. Loops have to do that for their index variables.
        
    protected:
        SyntaxVariable*                     variable;                                                                           //!< A lhs variable
        SyntaxFunctionCall*                 functionCall;                                                                       //!< A lhs function call
        SyntaxElement*                      expression;                                                                         //!< The rhs expression
        
    };
    
}

#endif

