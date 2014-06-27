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


/**
 * This is the class used to hold constants in the syntax tree.
 *
 * The result is never going to change so we can set the value
 * already in the constructor.
 *
 */

namespace RevLanguage {

class SyntaxConstant : public SyntaxElement {

    public:
                                                SyntaxConstant(RevObject* val);                                                 //!< Constructor from value
                                                SyntaxConstant(const SyntaxConstant& sc);                                       //!< Copy constructor
	    virtual                                ~SyntaxConstant();                                                               //!< Destructor deletes value

        // Assignment operator
        SyntaxConstant&                         operator=(const SyntaxConstant& x);                                             //!< Assignment operator

        // Basic utility functions
        SyntaxConstant*                         clone(void) const;                                                              //!< Clone object
        void                                    printValue(std::ostream& o) const;                                              //!< Print info about object

        // Regular functions
        RevPtr<Variable>                        evaluateContent( Environment& env );                                            //!< Get semantic value
        bool                                    isConstExpression(void) const;                                                  //!< Is the expression constant?
        void                                    replaceVariableWithConstant(const std::string& name, const RevObject& c);       //!< Replace the syntax variable with name by the constant value. Loops have to do that for their index variables.

    protected:
        RevObject*                              value;                                                                          //!< The constant value
    
};
    
}

#endif

