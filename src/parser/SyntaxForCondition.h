/**
 * @file
 * This file contains the declaration of SyntaxForCondition, which is
 * used to hold expressions for the loop in a for statement.
 *
 * @brief Declaration of SyntaxForCondition
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef SyntaxForCondition_H
#define SyntaxForCondition_H

#include "Container.h"
#include "VectorInteger.h"
#include "SyntaxElement.h"

#include <iostream>
#include <list>

class ArgumentRule;
class RbString;

class SyntaxForCondition : public SyntaxElement {

    public:
            SyntaxForCondition(RbString* identifier, SyntaxElement* inExpr);    //!< Standard constructor
            SyntaxForCondition(const SyntaxForCondition& x);                    //!< Copy constructor
	        virtual ~SyntaxForCondition();                                      //!< Destructor

        // Basic utility functions
        std::string     briefInfo() const;                          //!< Brief info about object
        SyntaxElement*  clone() const;                              //!< Clone object
        bool            equals(const SyntaxElement* elem) const;    //!< Equals comparison
        void            print(std::ostream& o) const;               //!< Print info about object

        // Regular functions
        DAGNode*        getDAGNode(Frame* frame=NULL) const;        //!< Convert to DAG node
        bool            getNextLoopState(Frame* frame);             //!< Get next state of loop
        RbObject*       getValue(Frame* frame=NULL) const;          //!< Get semantic value

    protected:
        RbString*       varName;                                    //!< The name of the variable
        SyntaxElement*  inExpression;                               //!< The expression that should result in a vector of values
        bool            isLoopInitialized;                          //!< Is loop state initialized?
        Container*      vector;                                     //!< Vector result of 'in' expression
        VectorInteger*      intVector;                                  //!< Vector result of 'in' expression
        size_t          nextElement;                                //!< Next element in vector

        void            initializeLoop(Frame* frame);               //!< Initialize loop
};

#endif

