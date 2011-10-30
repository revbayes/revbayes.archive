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

class ArgumentRule;
class ArgumentEnvironment;
class RbString;
class TypeSpec;


const std::string SyntaxFormal_name = "Argument specification";

class SyntaxFormal : public SyntaxElement {

    public:
                                SyntaxFormal(RbPtr<RbString> id, RbPtr<SyntaxElement> defaultVal);                      //!< Implicit type
                                SyntaxFormal(RbPtr<RbString> typeSpec, RbPtr<RbString> id, RbPtr<SyntaxElement> defaultVal);  //!< Explicit type
                                SyntaxFormal(const SyntaxFormal& x);                                            //!< Copy constructor
	    virtual                ~SyntaxFormal();                                                                 //!< Destructor

        // Assignment operator
        SyntaxFormal&           operator=(const SyntaxFormal& x);                                               //!< Assignment operator

        // Basic utility functions
        std::string             briefInfo() const;                                                              //!< Brief info about object
        SyntaxFormal*           clone() const;                                                                  //!< Clone object
        const VectorString&     getClass(void) const;                                                           //!< Get class vector 
        const TypeSpec&         getTypeSpec(void) const;                                                        //!< Get language type of the object
        void                    print(std::ostream& o) const;                                                   //!< Print info about object

        // Regular functions
        RbPtr<const ArgumentRule>   getArgumentRule(void) const;                                                //!< Get the argument rule
        RbPtr<ArgumentRule>         getArgumentRule(void);                                                      //!< Get the argument rule (non-const to return non-const rule)
        RbPtr<const RbString>       getLabel(void) const;                                                       //!< Get label
        RbPtr<const TypeSpec>       getArgumentTypeSpec(void) const;                                            //!< Get type spec
        RbPtr<Variable>             evaluateContent(RbPtr<Environment> env);                                    //!< Get semantic value
    
    protected:
        RbPtr<ArgumentRule>         argRule;
        RbPtr<TypeSpec>             argType;                                                                    //!< The type of the argument
        RbPtr<RbString>             label;                                                                      //!< The label of the argument
        RbPtr<SyntaxElement>        defaultExpr;                                                                //!< Default value expression of argument
    
    private:
        static const TypeSpec       typeSpec;
};

#endif
