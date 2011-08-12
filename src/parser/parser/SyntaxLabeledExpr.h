/**
 * @file
 * This file contains the declaration of SyntaxLabeledExpr, which is
 * used to hold labeled expressions that represent arguments to
 * functions in the syntax tree.
 *
 * @brief Declaration of SyntaxLabeledExpr
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef SyntaxLabeledExpr_H
#define SyntaxLabeledExpr_H

#include "RbString.h"
#include "SyntaxElement.h"

#include <iostream>
#include <list>


class SyntaxLabeledExpr : public SyntaxElement {

    public:
                            SyntaxLabeledExpr(RbString* id, SyntaxElement *expr);   //!< Constructor
                            SyntaxLabeledExpr(const SyntaxLabeledExpr& x);          //!< Copy constructor
	    virtual            ~SyntaxLabeledExpr();                                    //!< Destructor

        // Assignment operator
        SyntaxLabeledExpr&  operator=(const SyntaxLabeledExpr& x);                  //!< Assignment operator

        // Basic utility functions
        std::string         briefInfo() const;                                      //!< Brief info about object
        SyntaxLabeledExpr*  clone() const;                                          //!< Clone object
        const VectorString& getClass(void) const;                                   //!< Get class vector 
        void                print(std::ostream& o) const;                           //!< Print info about object

        // Regular functions
        SyntaxElement*      getExpression(void) { return expression; }              //!< Return expression
        const RbString*     getLabel() const { return label; }                      //!< Return label    
        Variable*           getContentAsVariable(Environment* env) const;           //!< Get semantic value

    protected:
        RbString*           label;                                                  //!< The label of the argument
        SyntaxElement*      expression;                                             //!< The expression for the argument value
};

#endif

