/**
 * @file
 * This file contains the declaration of SyntaxFunctionDef, which is
 * used to hold definitions of functions (user-defined functions).
 *
 * @brief Declaration of SyntaxFunctionDef
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef SyntaxFunctionDef_H
#define SyntaxFunctionDef_H

#include "RbString.h"
#include "SyntaxElement.h"
#include "SyntaxFormal.h"
#include "SyntaxVariable.h"
#include "TypeSpec.h"

#include <ostream>
#include <list>
#include <string>


class SyntaxFunctionDef : public SyntaxElement {

    public:
                                        SyntaxFunctionDef(  RbString*                   type,
                                                            RbString*                   name,
                                                            std::list<SyntaxFormal*>*   formals,
                                                            std::list<SyntaxElement*>*  stmts);     //!< Standard constructor
                                        SyntaxFunctionDef(const SyntaxFunctionDef& x);              //!< Copy constructor
	    virtual                        ~SyntaxFunctionDef();                                        //!< Destructor

        // Assignment operator
        SyntaxFunctionDef&              operator=(const SyntaxFunctionDef& x);                      //!< Assignment operator

        // Basic utility functions
        std::string                     briefInfo() const;                                          //!< Brief info about object
        SyntaxFunctionDef*              clone() const;                                              //!< Clone object
        const VectorString&             getClass(void) const;                                       //!< Get class vector 
        void                            print(std::ostream& o) const;                               //!< Print info about object

        // Regular functions
        Variable*                       getContentAsVariable(Environment* env) const;               //!< Get semantic value

    protected:
        TypeSpec*                       returnType;                                                 //!< The return type specification of the function
        RbString*                       functionName;                                               //!< The name of the function
        std::list<SyntaxFormal*>*       formalArgs;                                                 //!< The formal arguments
        std::list<SyntaxElement*>*      code;                                                       //!< The list of statements
};

#endif

