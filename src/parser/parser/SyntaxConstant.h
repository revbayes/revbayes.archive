/**
 * @file
 * This file contains the declaration of SyntaxConstant, which is
 * used to hold constants in the syntax tree.
 *
 * @brief Declaration of SyntaxConstant
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef SyntaxConstant_H
#define SyntaxConstant_H

#include "SyntaxElement.h"

#include <iostream>

class VectorString;


/**
 * This is the class used to hold constants in the syntax tree.
 *
 * The result is never going to change so we can set the value
 * already in the constructor.
 *
 */

class SyntaxConstant : public SyntaxElement {

    public:
                                                SyntaxConstant(RbLanguageObject* val);                                          //!< Constructor from value
                                                SyntaxConstant(const SyntaxConstant& sc);                                       //!< Copy constructor
	    virtual                                ~SyntaxConstant();                                                               //!< Destructor deletes value

        // Assignment operator
        SyntaxConstant&                         operator=(const SyntaxConstant& x);                                             //!< Assignment operator

        // Basic utility functions
        SyntaxConstant*                         clone(void) const;                                                              //!< Clone object
        static const std::string&               getClassName(void);                                                             //!< Get class name
        static const TypeSpec&                  getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                         getTypeSpec(void) const;                                                        //!< Get language type of the object
        void                                    printValue(std::ostream& o) const;                                              //!< Print info about object

        // Regular functions
        RbVariablePtr                           evaluateContent( Environment& env );                                            //!< Get semantic value
        bool                                    isConstExpression(void) const;                                                  //!< Is the expression constant?
        void                                    replaceVariableWithConstant(const std::string& name, const RbLanguageObject& c);//!< Replace the syntax variable with name by the constant value. Loops have to do that for their index variables.

    protected:
        RbLanguageObject*                       value;                                                                          //!< The constant value
    
};

#endif

