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
 * $Id:$
 */

#ifndef SyntaxVariable_H
#define SyntaxVariable_H

#include "RbString.h"
#include "SyntaxElement.h"

#include <iostream>
#include <list>

class SyntaxFunctionCall;
class VectorIndex;


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
                                    SyntaxVariable(RbString* id, std::list<SyntaxElement*>* indx);                          //!< Global variable
                                    SyntaxVariable(SyntaxFunctionCall* fxnCall, std::list<SyntaxElement*>* indx);           //!< Global variable expression
                                    SyntaxVariable(SyntaxVariable* var, RbString* id, std::list<SyntaxElement*>* indx);     //!< Member variable 
                                    SyntaxVariable(SyntaxVariable* var, SyntaxFunctionCall* fxnCall,
                                                   std::list<SyntaxElement*>* indx);                                        //!< Member variable expression
                                    SyntaxVariable(const SyntaxVariable& x);                                                //!< Copy constructor
	    virtual                    ~SyntaxVariable(void);                                                                   //!< Destructor deletes variable, identifier and index

        // Assignment operator
        SyntaxVariable&             operator=(const SyntaxVariable& x);                                                     //!< Assignment operator

        // Basic utility functions
        std::string                 briefInfo(void) const;                                                                  //!< Brief info about object
        SyntaxVariable*             clone(void) const;                                                                      //!< Clone object
        const VectorString&         getClass(void) const;                                                                   //!< Get class vector 
        void                        print(std::ostream& o) const;                                                           //!< Print info about object

        // Regular functions
        DAGNode*                    getDAGNodeExpr(VariableFrame* frame) const;                                             //!< Convert to DAG node expression
        RbString*                   getIdentifier(void) { return identifier; }                                              //!< Get identifier
        VectorIndex                 getIndex(VariableFrame* frame) const;                                                   //!< Evaluate index
        std::string                 getFullName(VariableFrame* frame) const;                                                //!< Get full name, with indices and base obj
        DAGNode*                    getLValue(VariableFrame* frame, VariableSlot*& theSlot, VectorIndex& index) const;      //!< Get semantic value
        DAGNode*                    getValue(VariableFrame* frame) const;                                                   //!< Get semantic value

    protected:
        RbString*                   identifier;                                                                             //!< The name of the variable, if identified by name
        SyntaxFunctionCall*         functionCall;                                                                           //!< Function call giving a reference to a variable (we hope)
        std::list<SyntaxElement*>*  index;                                                                                  //!< Vector of int indices to variable element
        SyntaxVariable*             baseVariable;                                                                           //!< Base variable (pointing to a composite node)
};

#endif

