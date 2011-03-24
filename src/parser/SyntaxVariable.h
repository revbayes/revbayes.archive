/**
 * @file
 * This file contains the declaration of SyntaxVariable, which is
 * used to hold variable references in the syntax tree.
 *
 * @brief Declaration of SyntaxVariable
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef SyntaxVariable_H
#define SyntaxVariable_H

#include "VectorInteger.h"
#include "RbString.h"
#include "SyntaxElement.h"

#include <iostream>
#include <list>


/**
 * This is the class used to hold variables in the syntax tree.
 *
 * We store the identifier, the index vector and the base variable
 * here so that we can wrap these things into a DAG node expression
 * if needed.
 *
 */
class SyntaxVariable : public SyntaxElement {

    public:
                                    SyntaxVariable(RbString* id, std::list<SyntaxElement*>* indx);                      //!< Global variable
                                    SyntaxVariable(SyntaxVariable* var, RbString* id, std::list<SyntaxElement*>* indx); //!< Member variable 
                                    SyntaxVariable(const SyntaxVariable& x);                                            //!< Copy constructor
	    virtual                    ~SyntaxVariable(void);                                                               //!< Destructor deletes variable, identifier and index

        // Assignment operator
        SyntaxVariable&             operator=(const SyntaxVariable& x);                                                 //!< Assignment operator

        // Basic utility functions
        std::string                 briefInfo(void) const;                                                              //!< Brief info about object
        SyntaxVariable*             clone(void) const;                                                                  //!< Clone object
        const VectorString&         getClass(void) const;                                                               //!< Get class vector 
        void                        print(std::ostream& o) const;                                                       //!< Print info about object

        // Regular functions
        SyntaxVariable*             getBaseVariable(void) { return baseVariable; }                                      //!< Get base variable
        DAGNode*                    getDAGNodeExpr(Frame* frame) const;                                                 //!< Convert to DAG node expression
        const RbString*             getIdentifier() const { return identifier; }                                        //!< Get identifier
        VectorInteger               getIndex(Frame* frame) const;                                                       //!< Get index
        std::string                 getFullName(Frame* frame) const;                                                    //!< Get full name, with indices and base obj
        DAGNode*                    getValue(Frame* frame) const;                                                       //!< Get semantic value
        VariableSlot&               getVariableSlot(Frame* frame) const;                                                //!< Is the variable a reference?
        void                        getVariableInfo(Frame* frame, VariableSlot* slot, VectorInteger* index) const;      //!< Get info about the variable (for SyntaxAssignExpr)

    protected:
        RbString*                   identifier;                                                                         //!< The name of the variable
        std::list<SyntaxElement*>*  index;                                                                              //!< Vector of int indices to variable element
        SyntaxVariable*             baseVariable;                                                                       //!< Base variable (pointing to a composite node)
};

#endif

