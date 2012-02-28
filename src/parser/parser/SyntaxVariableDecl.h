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


class SyntaxVariableDecl : public SyntaxElement {
    
    public:
                                    SyntaxVariableDecl(RbString*                    typeName,
                                                       std::list<SyntaxElement*>*   lengths,
                                                       RbString*                    referenceChar,
                                                       RbString*                    varName);                       //!< Basic constructor
                                    SyntaxVariableDecl(const SyntaxVariableDecl& x);                                //!< Copy constructor
        virtual                    ~SyntaxVariableDecl();                                                           //!< Destructor
        
        // Assignment operator
        SyntaxVariableDecl&         operator=(const SyntaxVariableDecl& x);                                         //!< Assignment operator
        
        // Basic utility functions
        SyntaxVariableDecl*         clone() const;                                                                  //!< Clone object
        static const std::string&   getClassName(void);                                                             //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                                        //!< Get language type of the object
        void                        printValue(std::ostream& o) const;                                              //!< Print info about object
        
        // Regular functions
        RbVariablePtr               evaluateContent(Environment& env);                                              //!< Get semantic value
        void                        replaceVariableWithConstant(const std::string& name, const RbLanguageObject& c);//!< Replace the syntax variable with name by the constant value. Loops have to do that for their index variables.
        
    protected:
        RbString*                   elementTypeName;                                                                //!< Element type of the variable
        std::list<SyntaxElement*>*  lengthExpr;                                                                     //!< Lengths in different dimensions
        RbString*                   referenceSymbol;                                                                //!< Is reference? ("&" or "")
        RbString*                   variableName;                                                                   //!< Variable name
    
};

#endif

