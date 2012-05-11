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
                                    SyntaxLabeledExpr(RbString* id, SyntaxElement* expr);                           //!< Constructor
                                    SyntaxLabeledExpr(const SyntaxLabeledExpr& x);                                  //!< Copy constructor
	    virtual                    ~SyntaxLabeledExpr();                                                            //!< Destructor

        // Assignment operator
        SyntaxLabeledExpr&          operator=(const SyntaxLabeledExpr& x);                                          //!< Assignment operator

        // Basic utility functions
        SyntaxLabeledExpr*          clone() const;                                                                  //!< Clone object
        static const std::string&   getClassName(void);                                                             //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                                        //!< Get language type of the object
        void                        printValue(std::ostream& o) const;                                              //!< Print info about object

        // Regular functions
        const SyntaxElement&        getExpression(void) const { return *expression; }                               //!< Return expression
        SyntaxElement&              getExpression(void) { return *expression; }                                     //!< Return expression
        const RbString&             getLabel() const { return *label; }                                             //!< Return label    
        RbPtr<Variable>             evaluateContent( Environment& env );                                            //!< Get semantic value
        bool                        isConstExpression(void) const;                                                  //!< Is the expression constant?
        void                        replaceVariableWithConstant(const std::string& name, const RbLanguageObject& c);//!< Replace the syntax variable with name by the constant value. Loops have to do that for their index variables.

    protected:
        RbString*                   label;                                                                          //!< The label of the argument
        SyntaxElement*              expression;                                                                     //!< The expression for the argument value
    
};

#endif

