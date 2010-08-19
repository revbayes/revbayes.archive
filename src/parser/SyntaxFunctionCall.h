/**
 * @file
 * This file contains the declaration of SyntaxFunctionCall, which is
 * used to hold function calls in the syntax tree.
 *
 * @brief Declaration of SyntaxFunctionCall
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef SyntaxFunctionCall_H
#define SyntaxFunctionCall_H

#include "SyntaxElement.h"
#include "SyntaxLabeledExpr.h"
#include "SyntaxVariable.h"

#include <iostream>
#include <list>


/**
 * This is the class used to hold function calls in the syntax tree.
 *
 * We store the arguments, function name, and variable of which the
 * function is a member, if any.
 *
 */
class SyntaxFunctionCall : public SyntaxElement {

    public:
            SyntaxFunctionCall(RbString* id, std::list<SyntaxLabeledExpr*>* args);  //!< Standard function
            SyntaxFunctionCall(SyntaxVariable* var, RbString* id,
                               std::list<SyntaxLabeledExpr*>* args);                //!< Member function
            SyntaxFunctionCall(const SyntaxFunctionCall& x);                        //!< Copy constructor
	        virtual ~SyntaxFunctionCall();                                          //!< Destructor

        // Basic utility functions
        std::string         briefInfo() const;                          //!< Brief info about object
        SyntaxElement*      clone() const;                              //!< Clone object
        bool                equals(const SyntaxElement* elem) const;    //!< Equals comparison
        const VectorString& getClass(void) const;                       //!< Get class vector 
        void                print(std::ostream& o) const;               //!< Print info about object

        // Regular functions
        const RbString*     getFunctionName(void) const { return functionName; }    //!< Get function name
        DAGNode*            getDAGNode(Frame* frame=NULL) const;                    //!< Convert to DAG node
        RbObject*           getValue(Frame* frame=NULL) const;                      //!< Get semantic value

    protected:
        std::list<SyntaxLabeledExpr*>*  arguments;      //!< The arguments passed to the function
        RbString*                       functionName;   //!< The name of the function
        SyntaxVariable*                 variable;       //!< The variable holding the function
};

#endif

