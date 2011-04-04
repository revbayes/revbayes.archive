/**
 * @file
 * This file contains the declaration of SyntaxForCondition, which is
 * used to hold expressions for the loop in a for statement.
 *
 * @brief Declaration of SyntaxForCondition
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef SyntaxForCondition_H
#define SyntaxForCondition_H

#include "Container.h"
#include "ContainerNode.h"
#include "VectorInteger.h"
#include "SyntaxElement.h"

#include <iostream>
#include <list>

class ValueRule;
class RbString;

class SyntaxForCondition : public SyntaxElement {

    public:
                                SyntaxForCondition(RbString* identifier, SyntaxElement* inExpr);    //!< Standard constructor
                                SyntaxForCondition(const SyntaxForCondition& x);                    //!< Copy constructor
	    virtual                ~SyntaxForCondition();                                               //!< Destructor

        // Assignment operator
        SyntaxForCondition&     operator=(const SyntaxForCondition& x);                             //!< Assignment operator
        
        // Basic utility functions
        std::string             briefInfo() const;                                                  //!< Brief info about object
        SyntaxElement*          clone() const;                                                      //!< Clone object
        void                    print(std::ostream& o) const;                                       //!< Print info about object

        // Regular functions
        void                    finalizeLoop(Frame* frame);                                         //!< Finalize loop
        DAGNode*                getDAGNodeExpr(Frame* frame) const;                                 //!< Convert to DAG node expression
        bool                    getNextLoopState(Frame* frame);                                     //!< Get next state of loop
        DAGNode*                getValue(Frame* frame) const;                                       //!< Get semantic value
        void                    initializeLoop(Frame* frame);                                       //!< Initialize loop

    protected:
        RbString*               varName;                                                            //!< The name of the loop variable
        SyntaxElement*          inExpression;                                                       //!< The in expression (a vector of values)
        ContainerNode*          vector;                                                             //!< Vector result of 'in' expression
        bool                    wasLoopVariableReference;                                           //!< Was loop variable reference before loop?
        int                     nextElement;                                                        //!< Next element in vector

};

#endif

