/**
 * @file
 * This file contains the declaration of SyntaxVariable, which is
 * used to hold variable references in the syntax tree.
 *
 * @brief Declaration of SyntaxVariable
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef SyntaxVariable_H
#define SyntaxVariable_H

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
class SyntaxVariable : public SyntaxElement {

    public:
            SyntaxVariable(RbString* id, std::vector<SyntaxElement*>* indx);  //!< Constructor from id and index
            SyntaxVariable(SyntaxVariable* var, RbString* id,
                         std::vector<SyntaxElement*>* indx);  //!< Constructor from wrapping variable, id and index
            SyntaxVariable(const SyntaxVariable& sv);       //!< Copy constructor
	        virtual ~SyntaxVariable();                      //!< Destructor deletes variable, identifier and index

        // Basic utility functions
        std::string     briefInfo() const;                          //!< Brief info about object
        SyntaxElement*  clone() const;                              //!< Clone object
        bool            equals(const SyntaxElement* elem) const;    //!< Equals comparison
        void            print(std::ostream& o) const;               //!< Print info about object

        // Regular functions
        DAGNode*        getDAGNode(Environment* env=NULL) const;    //!< Convert to DAG node
        const RbString* getIdentifier() const;                      //!< Get identifier
        RbObject*       getValue(Environment* env=NULL);            //!< Get semantic value

    protected:
        RbString*                       identifier;     //!< The name of the variable
        std::vector<SyntaxElement*>*    index;          //!< Vector of int indices to members
        SyntaxVariable*                 variable;       //!< Wrapping variable
};

#endif

