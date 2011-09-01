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

#include "AbstractVector.h"
#include "VectorInteger.h"
#include "SyntaxElement.h"

#include <iostream>
#include <list>

class ValueRule;
class RbString;

class SyntaxForCondition : public SyntaxElement {

    public:
                                SyntaxForCondition(RbString* identifier, SyntaxElement* inExpr);        //!< Standard constructor
                                SyntaxForCondition(const SyntaxForCondition& x);                        //!< Copy constructor
	    virtual                ~SyntaxForCondition();                                                   //!< Destructor

        // Assignment operator
        SyntaxForCondition&     operator=(const SyntaxForCondition& x);                                 //!< Assignment operator
        
        // Basic utility functions
        std::string             briefInfo() const;                                                      //!< Brief info about object
        SyntaxElement*          clone() const;                                                          //!< Clone object
        void                    print(std::ostream& o) const;                                           //!< Print info about object

        // Regular functions
        void                    finalizeLoop(Environment* env);                                         //!< Finalize loop
        bool                    getNextLoopState(Environment* env);                                     //!< Get next state of loop
        Variable*               getContentAsVariable(Environment* env) const;                           //!< Get semantic value
        void                    initializeLoop(Environment* env);                                       //!< Initialize loop

    protected:
        RbString*               varName;                                                                //!< The name of the loop variable
        SyntaxElement*          inExpression;                                                           //!< The in expression (a vector of values)
        AbstractVector*         vector;                                                                 //!< Vector result of 'in' expression
        int                     nextElement;                                                            //!< Next element in vector

};

#endif

