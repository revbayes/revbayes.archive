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

#include "RbPtr.h"
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
const std::string SyntaxBinaryExpr_name = "Binary expression";

class SyntaxBinaryExpr : public SyntaxElement {

    public:
        // Binary operator types
        enum                    operatorT { Range, Add, Sub, Mul, Div, Exp, Lt, Le, Eq, Ne, Ge, Gt, And, Or, And2, Or2 };
        static std::string      opCode[];                                               //!< Operator codes for printing

        // Constructors and destructor
                                SyntaxBinaryExpr(SyntaxBinaryExpr::operatorT    op,
                                             RbPtr<SyntaxElement>               lhs,
                                             RbPtr<SyntaxElement>               rhs);   //!< Standard constructor 
                                SyntaxBinaryExpr(const SyntaxBinaryExpr& x);            //!< Copy constructor
        virtual                ~SyntaxBinaryExpr();                                     //!< Destroy operands

        // Assignment operator
        SyntaxBinaryExpr&       operator=(const SyntaxBinaryExpr& x);                   //!< Assignment operator

        // Basic utility functions
        std::string             briefInfo() const;                                      //!< Brief info about object
        SyntaxElement*          clone() const;                                          //!< Clone object
        const VectorString&     getClass(void) const;                                   //!< Get class vector 
        const TypeSpec&         getTypeSpec(void) const;                                //!< Get language type of the object
        void                    print(std::ostream& o) const;                           //!< Print info about object

        // Regular functions
        RbPtr<Variable>         evaluateContent(RbPtr<Environment> env);                //!< Get semantic value

    protected:
        RbPtr<SyntaxElement>    leftOperand;                                            //!< The left operand
        RbPtr<SyntaxElement>    rightOperand;                                           //!< The right operand
        enum operatorT          operation;                                              //!< The type of operation
    
    private:
        static const TypeSpec   typeSpec;
};

#endif

