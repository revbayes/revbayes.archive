/**
 * @file
 * This file contains the declaration of SyntaxAssignExpr, which is
 * used to hold assignment expressions in the syntax tree. These
 * can be left-Constant, equation or tilde assignments.
 *
 * @brief Declaration of SyntaxAssignExpr
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-09-07 12:47:31 +0200 (Fri, 07 Sep 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: SyntaxAssignExpr.h 1801 2012-09-07 10:47:31Z hoehna $
 */

#ifndef SyntaxConstantAssignment_H
#define SyntaxConstantAssignment_H

#include "SyntaxElement.h"
#include "SyntaxVariable.h"

#include <iostream>
#include <list>
#include <string>

namespace RevLanguage {
    
    class SyntaxConstantAssignment : public SyntaxElement {
        
    public:
        SyntaxConstantAssignment(SyntaxVariable* var, SyntaxElement* expr);                                         //!< Constructor with lhs = variable
        SyntaxConstantAssignment(SyntaxFunctionCall* fxnCall, SyntaxElement* expr);                                 //!< Constructor with lhs = function call
        SyntaxConstantAssignment(const SyntaxConstantAssignment& x);                                                //!< Copy constructor
	    virtual                    ~SyntaxConstantAssignment();                                                     //!< Destructor
        
        // Assignment operator
        SyntaxConstantAssignment&   operator=(const SyntaxConstantAssignment& x);                                   //!< Assignment operator
        
        // Basic utility functions
        SyntaxConstantAssignment*   clone() const;                                                                  //!< Clone object
        bool                        isAssignment(void) const;
        void                        printValue(std::ostream& o) const;                                              //!< Print info about object
        
        // Regular functions
        RbPtr<Variable>             evaluateContent(Environment& env);                                              //!< Get semantic value
        void                        replaceVariableWithConstant(const std::string& name, const RevObject& c);//!< Replace the syntax variable with name by the constant value. Loops have to do that for their index variables.
        
    protected:
        SyntaxVariable*             variable;                                                                       //!< A lhs variable (or NULL)
        SyntaxFunctionCall*         functionCall;                                                                   //!< A lhs function call (or NULL)
        SyntaxElement*              expression;                                                                     //!< The rhs expression
        
    };
    
}

#endif

