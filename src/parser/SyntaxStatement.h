/**
 * @file
 * This file contains the declaration of SyntaxStatement, which is
 * used to hold control and loop statements in the syntax tree.
 *
 * @brief Declaration of SyntaxStatement
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef SyntaxStatement_H
#define SyntaxStatement_H

#include "SyntaxElement.h"

#include <iostream>
#include <vector>


class SyntaxStatement : public SyntaxElement {

    public:
        // Statement types
        enum statementT { If, IfElse, For, While, Next, Break, Return };   //!< Statement type codes
        static std::string stmtName[];                                      //!< Statement names for printing

            // Constructors and destructor
            SyntaxStatement(statementT type);                       //!< NEXT, BREAK, RETURN
            SyntaxStatement(SyntaxStatement::statementT type,
                            SyntaxElement*              expr);      //!< RETURN expr
            SyntaxStatement(statementT                  type,
                            SyntaxElement*              cond,
                            std::list<SyntaxElement*>*  stmts);     //!< IF, FOR, WHILE cond stmts
            SyntaxStatement(statementT                  type,
                            SyntaxElement*              expr,
                            std::list<SyntaxElement*>*  stmts1,
                            std::list<SyntaxElement*>*  stmts2);    //!< IF_ELSE
            SyntaxStatement(const SyntaxStatement& x);              //!< Copy constructor
	        virtual ~SyntaxStatement();                             //!< Destroy operands

        // Basic utility functions
        std::string     briefInfo() const;                          //!< Brief info about object
        SyntaxElement*  clone() const;                              //!< Clone object
        bool            equals(const SyntaxElement* elem) const;    //!< Equals comparison
        void            print(std::ostream& o) const;               //!< Print info about object

        // Regular functions
        DAGNode*        getDAGNode(Frame* frame=NULL) const;        //!< Convert to DAG node
        RbObject*       getValue(Frame* frame=NULL) const;          //!< Get semantic value

    protected:
        enum statementT             statementType;     //!< The type of statement
        SyntaxElement*              expression;        //!< Expression, conditional expr, or for condition
        std::list<SyntaxElement*>*  statements1;       //!< First set of statements
        std::list<SyntaxElement*>*  statements2;       //!< Second set of statements
};

#endif

