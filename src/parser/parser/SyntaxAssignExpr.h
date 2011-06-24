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
        enum operatorT { ArrowAssign, TildeAssign, EquationAssign };        //!< Enum of operator types
        static std::string  opCode[];                                       //!< Operator codes for printing

                            SyntaxAssignExpr(operatorT       op,
                                             SyntaxVariable* var,
                                             SyntaxElement* expr);          //!< Constructor
                            SyntaxAssignExpr(const SyntaxAssignExpr& x);    //!< Copy constructor
	    virtual            ~SyntaxAssignExpr();                             //!< Destructor

        // Basic utility functions
        std::string         briefInfo() const;                              //!< Brief info about object
        SyntaxElement*      clone() const;                                  //!< Clone object
        const VectorString& getClass(void) const;                           //!< Get class vector 
        void                print(std::ostream& o) const;                   //!< Print info about object

        // Regular functions
        DAGNode*            getDAGNodeExpr(VariableFrame* frame) const;     //!< Convert to DAG node expression
        DAGNode*            getValue(VariableFrame* frame) const;           //!< Get semantic value
        ContainerNode*      makeContainer(const VectorNatural& index, RbObject* elem) const;    //!< Make value container
        ContainerNode*      makeContainer(const VectorNatural& index, DAGNode* elem) const;     //!< Make variable container

    protected:
        SyntaxVariable*             variable;                               //!< The variable to receive the value
        SyntaxElement*              expression;                             //!< The expression or function call on the righ-hand side
        SyntaxAssignExpr::operatorT opType;                                 //!< The type of assignment
};

#endif

