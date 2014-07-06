/**
 * @file
 * This file contains the declaration of SyntaxBinaryExpr, which is
 * used to hold binary expressions in the syntax tree.
 *
 * @brief Declaration of SyntaxBinaryExpr
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef SyntaxBinaryExpr_H
#define SyntaxBinaryExpr_H

#include "SyntaxElement.h"

#include <iostream>
#include <vector>


/**
 * This is the class used to hold binary expressions in the syntax tree.
 *
 * We store the operands and a flag signalling the type of operation to
 * be performed when getValue is called or to be represented when
 * getDAGNodeExpr is called.
 *
 */

namespace RevLanguage {

class SyntaxBinaryExpr : public SyntaxElement {

    public:
        // Binary operator types
        enum                        operatorT { Range, Add, Sub, Mul, Div, Mod, Exp, Lt, Le, Eq, Ne, Ge, Gt, And, Or, And2, Or2 };
        static std::string          opCode[];                                               //!< Operator codes for printing

        // Constructors and destructor
                                    SyntaxBinaryExpr(SyntaxBinaryExpr::operatorT    op,
                                                     SyntaxElement*               lhs,
                                                     SyntaxElement*               rhs);                             //!< Standard constructor 
                                    SyntaxBinaryExpr(const SyntaxBinaryExpr& x);                                    //!< Copy constructor
        virtual                    ~SyntaxBinaryExpr();                                                             //!< Destroy operands

        // Assignment operator
        SyntaxBinaryExpr&           operator=(const SyntaxBinaryExpr& x);                                           //!< Assignment operator

        // Basic utility functions
        SyntaxElement*              clone() const;                                                                  //!< Clone object
        void                        printValue(std::ostream& o) const;                                              //!< Print info about object

        // Regular functions
        RevPtr<Variable>            evaluateContent(Environment& env);                                              //!< Get semantic value
        bool                        isConstExpression(void) const;                                                  //!< Is the expression constant?
        void                        replaceVariableWithConstant(const std::string& name, const RevObject& c);       //!< Replace the syntax variable with name by the constant value. Loops have to do that for their index variables.

    protected:
        SyntaxElement*              leftOperand;                                                                    //!< The left operand
        SyntaxElement*              rightOperand;                                                                   //!< The right operand
        enum operatorT              operation;                                                                      //!< The type of operation
    
};
    
}

#endif

