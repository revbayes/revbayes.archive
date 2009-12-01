/**
 * @file
 * This file contains the declaration of SyntaxVariable, which is
 * used to hold variable references in the syntax tree.
 *
 * @brief Declaration of SyntaxVariable
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author Fredrik Ronquist and the REvBayes core team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef SyntaxVariable_H
#define SyntaxVariable_H

#include "SyntaxElement.h"

#include <iostream>

class StringVector;


/**
 * This is the class used to hold variables in the syntax tree.
 *
 * We store the identifier, the index vector and the environment
 * here so that we can wrap these things into a DAG node if needed.
 *
 */
class SyntaxVariable : public SyntaxElement {

    public:
            SyntaxVariable(RbString* id, std::list<SyntaxElement*>* indx);  //!< Constructor from id and index
            SyntaxVariable(const SyntaxVariable& sv);       //!< Copy constructor
	        virtual ~SyntaxVariable();                      //!< Destructor deletes identifier and index

        // Basic utility functions
        std::string     briefInfo() const;                  //!< Brief info about object
        SyntaxVariable* clone() const;                      //!< Clone object
        bool            equals(const RbObject* obj) const;  //!< Equals comparison
        void            print(std::ostream& o) const;       //!< Print info about object

        // Regular functions
        DAGNode*        getDAGNode(Environment* env=NULL) const;    //!< Convert to DAG node
        StringVector&   getReturnType() const;                      //!< Get type of semantic value
        RbObject*       getValue(Environment* env=NULL);            //!< Get semantic value
        bool            isConstExpr() const;                        //!< Is subtree constant expr?

    protected:
        RbString*       identifier;     //!< The name of the variable
        IntVector*      index;          //!< Vector of int indices to members
        SyntaxVariable* variable;       //!< Wrapping variable
};

#endif

