/**
 * @file
 * This file contains the declaration of SyntaxStatement, which is
 * used to hold control and loop statements in the syntax tree.
 *
 * @brief Declaration of SyntaxStatement
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef SyntaxStatement_H
#define SyntaxStatement_H

#include "SyntaxElement.h"

#include <iostream>
#include <vector>


const std::string SyntaxStatement_name = "Statement";

class SyntaxStatement : public SyntaxElement {

    public:
        // Statement types
        enum                        statementT { If, IfElse, For, While, Next, Break, Return };                         //!< Statement type codes
        static std::string          stmtName[];                                                                         //!< Statement names for printing

        // Constructors and destructor
                                                    SyntaxStatement(statementT type);                                   //!< NEXT, BREAK, RETURN statements
                                                    SyntaxStatement(SyntaxStatement::statementT type,
                                                                    RbPtr<SyntaxElement>              expr);            //!< RETURN statement
                                                    SyntaxStatement(statementT                  type,
                                                                    RbPtr<SyntaxElement>              cond,
                                                                    RbPtr<std::list<RbPtr<SyntaxElement> > >  stmts);   //!< IF, FOR, WHILE statements
                                                    SyntaxStatement(statementT                  type,
                                                                    RbPtr<SyntaxElement>              expr,
                                                                    RbPtr<std::list<RbPtr<SyntaxElement> > >  stmts1,
                                                                    RbPtr<std::list<RbPtr<SyntaxElement> > >  stmts2);  //!< IF_ELSE statements
                                                    SyntaxStatement(const SyntaxStatement& x);                          //!< Copy constructor
	    virtual                                    ~SyntaxStatement();                                                  //!< Destroy operands

        // Assignment operator
        SyntaxStatement&                            operator=(const SyntaxStatement& x);                                //!< Assignment operator

        // Basic utility functions
        std::string                                 briefInfo() const;                                                  //!< Brief info about object
        SyntaxStatement*                            clone() const;                                                      //!< Clone object
        const VectorString&                         getClass(void) const;                                               //!< Get class vector 
        const TypeSpec&                             getTypeSpec(void) const;                                            //!< Get language type of the object
        void                                        print(std::ostream& o) const;                                       //!< Print info about object

        // Regular functions
        RbPtr<Variable>                             evaluateContent(RbPtr<Environment> env);                            //!< Get semantic value

    protected:
        bool                                        isTrue(RbPtr<SyntaxElement> expression, RbPtr<Environment> env) const;//!< Does expression evaluate to true?

        enum statementT                             statementType;                                                      //!< The type of statement
        RbPtr<SyntaxElement>                        expression;                                                         //!< Expression, conditional expr, or for condition
        RbPtr<std::list<RbPtr<SyntaxElement> > >    statements1;                                                        //!< First set of statements
        RbPtr<std::list<RbPtr<SyntaxElement> > >    statements2;                                                        //!< Second set of statements
    
    private:
        static const TypeSpec                       typeSpec;
};

#endif

