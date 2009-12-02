/**
 * @file
 * This file contains the declaration of SyntaxConstant, which is
 * used to hold constants in the syntax tree.
 *
 * @brief Declaration of SyntaxConstant
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef SyntaxConstant_H
#define SyntaxConstant_H

#include "SyntaxElement.h"

#include <iostream>

class StringVector;


/**
 * This is the class used to hold constants in the syntax tree.
 *
 * The result is never going to change so we can set the value
 * already in the constructor.
 *
 */
class SyntaxConstant : public SyntaxElement {

    public:
            SyntaxConstant(RbObject* val);              //!< Constructor from value
            SyntaxConstant(const SyntaxConstant& sc);   //!< Copy constructor
	        virtual ~SyntaxConstant();                  //!< Destructor deletes value

        // Basic utility functions
        std::string     briefInfo() const;                          //!< Brief info about object
        SyntaxElement*  clone() const;                              //!< Clone object
        bool            equals(const SyntaxElement* elem) const;    //!< Equals comparison
        void            print(std::ostream& o) const;               //!< Print info about object

        // Regular functions
        DAGNode*        getDAGNode(Environment* env=NULL) const;    //!< Convert to DAG node
        RbObject*       getValue(Environment* env=NULL);            //!< Get semantic value
        bool            isConstExpr() const;                        //!< Is subtree constant expr?

    protected:
        RbObject*   value;      //!< The constant value
};

#endif

