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
class RbString;
class TypeSpec;

class SyntaxFormal : public SyntaxElement {

    public:
                        SyntaxFormal(RbString* id, SyntaxElement* defaultVal);                      //!< Implicit type
                        SyntaxFormal(RbString* typeSpec, RbString* id, SyntaxElement* defaultVal);  //!< Explicit type
                        SyntaxFormal(const SyntaxFormal& x);                                        //!< Copy constructor
	    virtual        ~SyntaxFormal();                                                             //!< Destructor

        // Assignment operator
        SyntaxFormal&   operator=(const SyntaxFormal& x);                                           //!< Assignment operator

        // Basic utility functions
        std::string     briefInfo() const;                                                          //!< Brief info about object
        SyntaxFormal*   clone() const;                                                              //!< Clone object
        void            print(std::ostream& o) const;                                               //!< Print info about object

        // Regular functions
        ArgumentRule*   getArgumentRule(VariableFrame* frame) const;                                //!< Make an argument rule
        DAGNode*        getDAGNodeExpr(VariableFrame* frame) const;                                 //!< Convert to DAG node expression
        DAGNode*        getValue(VariableFrame* frame) const;                                       //!< Get semantic value

    protected:
        TypeSpec*       type;                                                                       //!< The type of the argument
        RbString*       label;                                                                      //!< The label of the argument
        SyntaxElement*  defaultExpr;                                                                //!< Default value expression of argument
};

#endif
