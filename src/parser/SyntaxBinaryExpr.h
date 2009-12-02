/**
 * @file
 * This file contains the declaration of SyntaxBinaryExpr, which is
 * used to hold binary expressions in the syntax tree.
 *
 * @brief Declaration of SyntaxBinaryExpr
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author Fredrik Ronquist and the REvBayes core team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef SyntaxBinaryExpr_H
#define SyntaxBinaryExpr_H

#include "SyntaxElement.h"
#include "RbString.h";

#include <iostream>
#include <vector>


/**
 * This is the class used to hold variables in the syntax tree.
 *
 * We store the identifier, the index vector and the environment
 * here so that we can wrap these things into a DAG node if needed.
 *
 */
class SyntaxBinaryExpr : public SyntaxElement {

    public:
            SyntaxBinaryExpr(RbString* id, std::vector<SyntaxElement*>* indx);  //!< Constructor from id and index
            SyntaxBinaryExpr(SyntaxBinaryExpr* var, RbString* id,
                         std::vector<SyntaxElement*>* indx);  //!< Constructor from wrapping variable, id and index
            SyntaxBinaryExpr(const SyntaxBinaryExpr& sv);       //!< Copy constructor
	        virtual ~SyntaxBinaryExpr();                      //!< Destructor deletes variable, identifier and index

        // Basic utility functions
        std::string     briefInfo() const;                          //!< Brief info about object
        SyntaxElement*  clone() const;                              //!< Clone object
        bool            equals(const SyntaxElement* elem) const;    //!< Equals comparison
        void            print(std::ostream& o) const;               //!< Print info about object

        // Regular functions
        DAGNode*        getDAGNode(Environment* env=NULL) const;    //!< Convert to DAG node
        const RbString* getIdentifier() const;                      //!< Get identifier
        RbObject*       getValue(Environment* env=NULL);            //!< Get semantic value
        bool            isConstExpr() const;                        //!< Is subtree constant expr?

    protected:
        SyntaxElement*  leftOperand;        //!< The name of the variable
        SyntaxElement*  rightOperand;       //!< The name of the variable
};

#endif

