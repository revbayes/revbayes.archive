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
 * @author The RevBayes core development team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef SyntaxAssignExpr_H
#define SyntaxAssignExpr_H

#include "SyntaxElement.h"
#include "RbString.h"

#include <iostream>
#include <list>


/**
 * This is the class used to hold labeled expressions in the syntax
 * tree. These are used as arguments to functions and as templates
 * for arguments (formal arguments) in function definitions.
 */
class SyntaxAssignExpr : public SyntaxElement {

    public:
        // Static operator types
        enum operatorT { ARROW_ASSIGN, TILDE_ASSIGN, EQUATION_ASSIGN };     //!< Enum of operator types
        static std::string opCode[];                                //!< Operator codes for printing

            SyntaxAssignExpr(RbString* id, SyntaxElement *expr);   //!< Constructor
            SyntaxAssignExpr(const SyntaxAssignExpr& x);           //!< Copy constructor
	        virtual ~SyntaxAssignExpr();                           //!< Destructor

        // Basic utility functions
        std::string     briefInfo() const;                          //!< Brief info about object
        SyntaxElement*  clone() const;                              //!< Clone object
        bool            equals(const SyntaxElement* elem) const;    //!< Equals comparison
        void            print(std::ostream& o) const;               //!< Print info about object

        // Regular functions
        DAGNode*        getDAGNode(Frame* frame=NULL) const;        //!< Convert to DAG node
        RbObject*       getValue(Frame* frame=NULL);                //!< Get semantic value

    protected:
        SyntaxVariable*                 lhs;        //!< The left-hand side
        SyntaxElement*                  rhs;        //!< The right-hand side
};

#endif

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

