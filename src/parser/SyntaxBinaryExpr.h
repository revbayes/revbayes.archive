/**
 * @file
 * This file contains the declaration of SyntaxBinaryExpr, which is
 * used to hold binary expressions in the syntax tree.
 *
 * @brief Declaration of SyntaxBinaryExpr
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
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
 * getDAGNode is called.
 *
 */
class SyntaxBinaryExpr : public SyntaxElement {

    public:
        // Binary operator types
        enum operatorT { RANGE, ADD, SUB, MUL, DIV, EXP, LT, LE, EQ, NE, GE, GT, AND, OR, AND2, OR2 };
        static std::string opCode[];                                //!< Operator codes for printing

            // Constructors and destructor
            SyntaxBinaryExpr(SyntaxBinaryExpr::operatorT op,
                             SyntaxElement* lhs, SyntaxElement* rhs);   //!< Standard constructor 
            SyntaxBinaryExpr(const SyntaxBinaryExpr& x);                //!< Copy constructor
	        virtual ~SyntaxBinaryExpr();                                //!< Destroy operands

        // Basic utility functions
        std::string     briefInfo() const;                          //!< Brief info about object
        SyntaxElement*  clone() const;                              //!< Clone object
        bool            equals(const SyntaxElement* elem) const;    //!< Equals comparison
        void            print(std::ostream& o) const;               //!< Print info about object

        // Regular functions
        DAGNode*        getDAGNode(Frame* frame=NULL) const;        //!< Convert to DAG node
        RbObject*       getValue(Frame* frame=NULL);                //!< Get semantic value

    protected:
        SyntaxElement*  leftOperand;        //!< The left operand
        SyntaxElement*  rightOperand;       //!< The right operand
        enum operatorT  operation;          //!< The type of operation
};

#endif

