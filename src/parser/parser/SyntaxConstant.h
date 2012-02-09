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
const std::string SyntaxConstant_name = "Constant";

class SyntaxConstant : public SyntaxElement {

    public:
                                                SyntaxConstant(RbLanguageObject* val);                              //!< Constructor from value
                                                SyntaxConstant(const SyntaxConstant& sc);                           //!< Copy constructor
	    virtual                                ~SyntaxConstant();                                                   //!< Destructor deletes value

        // Assignment operator
        SyntaxConstant&                         operator=(const SyntaxConstant& x);                                 //!< Assignment operator

        // Basic utility functions
        SyntaxConstant*                         clone(void) const;                                                  //!< Clone object
        const VectorString&                     getClass(void) const;                                               //!< Get class vector 
        const TypeSpec&                         getTypeSpec(void) const;                                            //!< Get language type of the object
        void                                    printValue(std::ostream& o) const;                                  //!< Print info about object

        // Regular functions
        Variable*                               evaluateContent(void);                                              //!< Get semantic value
        bool                                    isConstExpression(void) const { return true; }                      //!< Is subtree constant expr?

    protected:
        RbLanguageObject*                       value;                                                              //!< The constant value
    
    private:
        static const TypeSpec                   typeSpec;
};

#endif

