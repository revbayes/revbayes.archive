/**
 * @file
 * This file contains the declaration of SyntaxFunctionCall, which is
 * used to hold function calls in the syntax tree.
 *
 * @brief Declaration of SyntaxFunctionCall
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id:$
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
                                        SyntaxFunctionCall(SyntaxVariable*                  var,
                                                           RbString*                        id,
                                                           std::list<SyntaxLabeledExpr*>*   args);              //!< Member function
                                        SyntaxFunctionCall(const SyntaxFunctionCall& x);                        //!< Copy constructor
	    virtual                        ~SyntaxFunctionCall();                                                   //!< Destructor

        // Assignment operator
        SyntaxFunctionCall&             operator=(const SyntaxFunctionCall& x);                                 //!< Assignment operator

        // Basic utility functions
        std::string                     briefInfo() const;                                                      //!< Brief info about object
        SyntaxFunctionCall*             clone() const;                                                          //!< Clone object
        const VectorString&             getClass(void) const;                                                   //!< Get class vector 
        void                            print(std::ostream& o) const;                                           //!< Print info about object

        // Regular functions
        const RbString*                 getFunctionName(void) const { return functionName; }                    //!< Get function name
        DAGNode*                        getDAGNodeExpr(VariableFrame* frame) const;                             //!< Convert to DAG node expression
        DAGNode*                        getValue(VariableFrame* frame) const;                                   //!< Get semantic value
        void                            setBaseVariable(SyntaxVariable* var) { variable = var; }                //!< Set base variable

    protected:
        std::list<SyntaxLabeledExpr*>*  arguments;                                                              //!< The arguments passed to the function
        RbString*                       functionName;                                                           //!< The name of the function
        SyntaxVariable*                 variable;                                                               //!< Variable holding member function
};

#endif

