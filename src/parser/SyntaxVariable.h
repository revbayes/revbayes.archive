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

#include "IntVector.h"
#include "RbString.h"
#include "SyntaxElement.h"

#include <iostream>
#include <list>


/**
 * This is the class used to hold variables in the syntax tree.
 *
 * We store the identifier, the index vector and the base variable
 * here so that we can wrap these things into a DAG node if needed.
 *
 */
class SyntaxVariable : public SyntaxElement {

    public:
            SyntaxVariable(RbString* id, std::list<SyntaxElement*>* indx);  //!< Global variable
            SyntaxVariable(SyntaxVariable* var, RbString* id, std::list<SyntaxElement*>* indx); //!< Member var. 
            SyntaxVariable(const SyntaxVariable& sv);       //!< Copy constructor
	        virtual ~SyntaxVariable();                      //!< Destructor deletes variable, identifier and index

        // Basic utility functions
        std::string         briefInfo() const;                          //!< Brief info about object
        SyntaxElement*      clone() const;                              //!< Clone object
        bool                equals(const SyntaxElement* elem) const;    //!< Equals comparison
        const StringVector& getClass(void) const;                       //!< Get class vector 
        void                print(std::ostream& o) const;               //!< Print info about object

        // Regular functions
        DAGNode*            getDAGNode(Frame* frame=NULL) const;        //!< Convert to DAG node
        const RbString*     getIdentifier() const;                      //!< Get identifier
        IntVector           getIndex(Frame* frame) const;               //!< Get index
        std::string         getFullName(Frame* frame) const;            //!< Get full name, with indices and base obj
        RbObject*           getValue(Frame* frame=NULL) const;          //!< Get semantic value
        const RbObject*     getValuePtr(Frame* frame) const;            //!< Get pointer to base object
        SyntaxVariable*     getVariable(void) { return variable; }      //!< Get base variable
        bool                isMember() const { return variable!=NULL; } //!< Is member variable?

    protected:
        RbString*                       identifier;     //!< The name of the variable
        std::list<SyntaxElement*>*      index;          //!< Vector of int indices to members
        SyntaxVariable*                 variable;       //!< Base variable (pointing to a complex object)
};

#endif

