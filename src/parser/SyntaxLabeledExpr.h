/**
 * @file
 * This file contains the declaration of SyntaxLabeledExpr, which is
 * used to hold labeled expressions in the syntax tree. Labeled
 * expressions are used in function calls for arguments and in
 * function definitions for argument labels and default values.
 *
 * @brief Declaration of SyntaxLabeledExpr
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef SyntaxLabeledExpr_H
#define SyntaxLabeledExpr_H

#include "SyntaxElement.h"
#include "RbString.h"

#include <iostream>
#include <list>


/**
 * This is the class used to hold labeled expressions in the syntax
 * tree. These are used as arguments to functions and as templates
 * for arguments (formal arguments) in function definitions.
 */
class SyntaxLabeledExpr : public SyntaxElement {

    public:
            SyntaxLabeledExpr(RbString* id, SyntaxElement *expr);   //!< Constructor
            SyntaxLabeledExpr(const SyntaxLabeledExpr& x);          //!< Copy constructor
	        virtual ~SyntaxLabeledExpr();                           //!< Destructor

        // Basic utility functions
        std::string     briefInfo() const;                          //!< Brief info about object
        SyntaxElement*  clone() const;                              //!< Clone object
        bool            equals(const SyntaxElement* elem) const;    //!< Equals comparison
        void            print(std::ostream& o) const;               //!< Print info about object

        // Regular functions
        const RbString* getLabel() const { return label; }          //!< Return label    
        DAGNode*        getDAGNode(Environment* env=NULL) const;    //!< Convert to DAG node
        RbObject*       getValue(Environment* env=NULL);            //!< Get semantic value

    protected:
        RbString*                       label;          //!< The label of the argument
        SyntaxElement*                  expression;     //!< The expression for argument value or default value
};

#endif

