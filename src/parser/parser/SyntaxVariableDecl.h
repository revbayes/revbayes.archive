/**
 * @file
 * This file contains the declaration of SyntaxVariableDecl, which is
 * used to hold assignment expressions in the syntax tree. These
 * can be left-arrow, equation or tilde assignments.
 *
 * @brief Declaration of SyntaxVariableDecl
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef SyntaxVariableDecl_H
#define SyntaxVariableDecl_H

#include "SyntaxElement.h"
#include "SyntaxVariable.h"

#include <iostream>
#include <list>
#include <string>


const std::string SyntaxVariableDecl_name = "Variable declaration";

class SyntaxVariableDecl : public SyntaxElement {
    
    public:
                                    SyntaxVariableDecl(RbPtr<RbString>                    typeName,
                                                       RbPtr<std::list<RbPtr<SyntaxElement> > >   lengths,
                                                       RbPtr<RbString>                    referenceChar,
                                                       RbPtr<RbString>                    varName);       //!< Basic constructor
                                    SyntaxVariableDecl(const SyntaxVariableDecl& x);                //!< Copy constructor
        virtual                    ~SyntaxVariableDecl();                                           //!< Destructor
        
        // Assignment operator
        SyntaxVariableDecl&         operator=(const SyntaxVariableDecl& x);                         //!< Assignment operator
        
        // Basic utility functions
        std::string                 briefInfo() const;                                              //!< Brief info about object
        SyntaxVariableDecl*         clone() const;                                                  //!< Clone object
        const VectorString&         getClass(void) const;                                           //!< Get class vector 
        const TypeSpec&             getTypeSpec(void) const;                                        //!< Get language type of the object
        void                        print(std::ostream& o) const;                                   //!< Print info about object
        
        // Regular functions
        RbPtr<Variable>             evaluateContent(RbPtr<Environment> env);                        //!< Get semantic value
        
    protected:
        RbPtr<RbString>             elementTypeName;                                                //!< Element type of the variable
        RbPtr<std::list<RbPtr<SyntaxElement> > >  lengthExpr;                                       //!< Lengths in different dimensions
        RbPtr<RbString>             referenceSymbol;                                                //!< Is reference? ("&" or "")
        RbPtr<RbString>             variableName;                                                   //!< Variable name
    
    private:
        static const TypeSpec       typeSpec;
};

#endif

