/**
 * @file
 * This file contains the declaration of SyntaxFormal, which is
 * used to hold formal argument specifications in the syntax
 * tree.
 *
 * @brief Declaration of SyntaxFormal
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef SyntaxFormal_H
#define SyntaxFormal_H

#include "SyntaxElement.h"

#include <iostream>
#include <list>

namespace RevLanguage {

class ArgumentRule;
class ArgumentEnvironment;
class TypeSpec;


class SyntaxFormal : public SyntaxElement {

    public:
        SyntaxFormal(const std::string &lbl, SyntaxElement* defaultVal);                                            //!< Implicit type
        SyntaxFormal(const std::string &type, const std::string &lbl, SyntaxElement* defaultVal);                   //!< Explicit type
        SyntaxFormal(const SyntaxFormal& x);                                                                        //!< Copy constructor
	    
        virtual                    ~SyntaxFormal();                                                                 //!< Destructor

        // Assignment operator
        SyntaxFormal&               operator=(const SyntaxFormal& x);                                               //!< Assignment operator

        // Basic utility functions
        SyntaxFormal*               clone() const;                                                                  //!< Clone object
        void                        printValue(std::ostream& o) const;                                              //!< Print info about object

        // Regular functions
        const ArgumentRule*         getArgumentRule(void) const;                                                    //!< Get the argument rule
        ArgumentRule*               getArgumentRule(void);                                                          //!< Get the argument rule (non-const to return non-const rule)
        const std::string&          getLabel(void) const;                                                           //!< Get label
        const TypeSpec&             getArgumentTypeSpec(void) const;                                                //!< Get type spec
        RbPtr<Variable>             evaluateContent( Environment& env );                                            //!< Get semantic value
        void                        replaceVariableWithConstant(const std::string& name, const RbLanguageObject& c);//!< Replace the syntax variable with name by the constant value. Loops have to do that for their index variables.

    protected:
        ArgumentRule*               argRule;
        TypeSpec                    argType;                                                                        //!< The type of the argument
        std::string                 label;                                                                          //!< The label of the argument
        SyntaxElement*              defaultExpr;                                                                    //!< Default value expression of argument
    
};
    
}

#endif
