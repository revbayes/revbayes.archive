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
const std::string SyntaxFunctionCall_name = "Function call";

class SyntaxFunctionCall : public SyntaxElement {

    public:
                                            SyntaxFunctionCall(RbPtr<RbString> id, RbPtr<std::list<RbPtr<SyntaxLabeledExpr> > > args);  //!< Standard function
                                            SyntaxFunctionCall(RbPtr<SyntaxVariable>                  var,
                                                           RbPtr<RbString>                        id,
                                                           RbPtr<std::list<RbPtr<SyntaxLabeledExpr> > >   args);                        //!< Member function
                                            SyntaxFunctionCall(const SyntaxFunctionCall& x);                                            //!< Copy constructor
	    virtual                            ~SyntaxFunctionCall();                                                                       //!< Destructor

        // Assignment operator
        SyntaxFunctionCall&                 operator=(const SyntaxFunctionCall& x);                                                     //!< Assignment operator

        // Basic utility functions
        std::string                         briefInfo() const;                                                                          //!< Brief info about object
        SyntaxFunctionCall*                 clone() const;                                                                              //!< Clone object
        const VectorString&                 getClass(void) const;                                                                       //!< Get class vector
        const TypeSpec&                     getTypeSpec(void) const;                                                                    //!< Get language type of the object 
        void                                print(std::ostream& o) const;                                                               //!< Print info about object

        // Regular functions
        const RbPtr<RbString>               getFunctionName(void) const { return functionName; }                                        //!< Get function name
        RbPtr<Variable>                     evaluateContent(RbPtr<Environment> env);                                                    //!< Get semantic value
        void                                setBaseVariable(RbPtr<SyntaxVariable> var) { variable = var; }                              //!< Set base variable

    protected:
        RbPtr<std::list<RbPtr<SyntaxLabeledExpr> > >  arguments;                                                                        //!< The arguments passed to the function
        RbPtr<RbString>                     functionName;                                                                               //!< The name of the function
        RbPtr<SyntaxVariable>               variable;                                                                                   //!< Variable holding member function
    
    private:
        static const TypeSpec       typeSpec;
};

#endif

