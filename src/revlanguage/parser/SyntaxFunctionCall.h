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

namespace RevLanguage {

class SyntaxFunctionCall : public SyntaxElement {

    public:
        SyntaxFunctionCall(const std::string &n, std::list<SyntaxLabeledExpr*>* args);                                                  //!< Standard function
        SyntaxFunctionCall(SyntaxVariable*  var, const std::string &n, std::list<SyntaxLabeledExpr*>* args);                            //!< Member function
        SyntaxFunctionCall(const SyntaxFunctionCall& x);                                                                                //!< Copy constructor
	    
        virtual                            ~SyntaxFunctionCall();                                                                       //!< Destructor

        // Assignment operator
        SyntaxFunctionCall&                 operator=(const SyntaxFunctionCall& x);                                                     //!< Assignment operator

        // Basic utility functions
        SyntaxFunctionCall*                 clone() const;                                                                              //!< Clone object
        void                                printValue(std::ostream& o) const;                                                          //!< Print info about object

        // Regular functions
        const std::string&                  getFunctionName(void) const { return functionName; }                                        //!< Get function name
        RevPtr<Variable>                    evaluateContent(Environment& env);                                                          //!< Get semantic value
        void                                setBaseVariable(SyntaxVariable* var) { variable = var; }                                    //!< Set base variable
        bool                                isConstExpression(void) const;
 
    protected:
        std::list<SyntaxLabeledExpr*>*      arguments;                                                                                  //!< The arguments passed to the function
        std::string                         functionName;                                                                               //!< The name of the function
        SyntaxVariable*                     variable;                                                                                   //!< Variable holding member function
    
};
    
}

#endif

