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
                                SyntaxFormal(RbString* id, SyntaxElement* defaultVal);                      //!< Implicit type
                                SyntaxFormal(RbString* typeSpec, RbString* id, SyntaxElement* defaultVal);  //!< Explicit type
                                SyntaxFormal(const SyntaxFormal& x);                                        //!< Copy constructor
	    virtual                ~SyntaxFormal();                                                             //!< Destructor

        // Assignment operator
        SyntaxFormal&           operator=(const SyntaxFormal& x);                                           //!< Assignment operator

        // Basic utility functions
        std::string             briefInfo() const;                                                          //!< Brief info about object
        SyntaxFormal*           clone() const;                                                              //!< Clone object
        const VectorString&     getClass(void) const;                                                       //!< Get class vector 
        const TypeSpec&         getTypeSpec(void) const;                                                    //!< Get language type of the object
        void                    print(std::ostream& o) const;                                               //!< Print info about object

        // Regular functions
        ArgumentRule*           getArgumentRule(Environment* env) const;                                    //!< Make an argument rule
        RbString*               getLabel(void) const { return label; }                                      //!< Get label
        TypeSpec*               getArgumentTypeSpec(void) const { return argType; }                         //!< Get type spec
        Variable*               getContentAsVariable(Environment* env) const;                               //!< Get semantic value
    
    protected:
        TypeSpec*               argType;                                                                    //!< The type of the argument
        RbString*               label;                                                                      //!< The label of the argument
        SyntaxElement*          defaultExpr;                                                                //!< Default value expression of argument
    
    private:
        static const TypeSpec   typeSpec;
};

#endif
