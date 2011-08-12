/**
 * @file
 * This file contains the declaration of SyntaxAssignExpr, which is
 * used to hold assignment expressions in the syntax tree. These
 * can be left-arrow, equation or tilde assignments.
 *
 * @brief Declaration of SyntaxAssignExpr
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef SyntaxAssignExpr_H
#define SyntaxAssignExpr_H

#include "SyntaxElement.h"
#include "SyntaxVariable.h"

#include <iostream>
#include <list>
#include <string>


class SyntaxAssignExpr : public SyntaxElement {

    public:
        // Static operator types
        enum operatorT { ArrowAssign, TildeAssign, TildeIidAssign, EquationAssign };                    //!< Enum of operator types
        static std::string opCode[];                                                                    //!< Operator codes for printing

                                    SyntaxAssignExpr(operatorT              op,
                                                     SyntaxVariable*        var,
                                                     SyntaxElement*         expr);                      //!< Constructor with lhs = variable
                                    SyntaxAssignExpr(operatorT              op,
                                                     SyntaxFunctionCall*    fxnCall,
                                                     SyntaxElement*         expr);                      //!< Constructor with lhs = function call
                                    SyntaxAssignExpr(const SyntaxAssignExpr& x);                        //!< Copy constructor
	    virtual                    ~SyntaxAssignExpr();                                                 //!< Destructor

        // Assignment operator
        SyntaxAssignExpr&           operator=(const SyntaxAssignExpr& x);                               //!< Assignment operator

        // Basic utility functions
        std::string                 briefInfo() const;                                                  //!< Brief info about object
        SyntaxAssignExpr*           clone() const;                                                      //!< Clone object
        const VectorString&         getClass(void) const;                                               //!< Get class vector 
        void                        print(std::ostream& o) const;                                       //!< Print info about object

    // Regular functions
        Variable*                   getContentAsVariable(Environment* env) const;                       //!< Get semantic value

    protected:
        SyntaxVariable*             variable;                                                           //!< A lhs variable (or NULL)
        SyntaxFunctionCall*         functionCall;                                                       //!< A lhs function call (or NULL)
        SyntaxElement*              expression;                                                         //!< The rhs expression
        SyntaxAssignExpr::operatorT opType;                                                             //!< The type of assignment
};

#endif

