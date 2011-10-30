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


const std::string SyntaxFunctionDef_name = "Function definition";

class SyntaxFunctionDef : public SyntaxElement {

    public:
                                        SyntaxFunctionDef(  RbPtr<RbString>                   type,
                                                            RbPtr<RbString>                   name,
                                                            RbPtr<std::list<RbPtr<SyntaxFormal> > >   formals,
                                                            RbPtr<std::list<RbPtr<SyntaxElement> > >  stmts);     //!< Standard constructor
                                        SyntaxFunctionDef(const SyntaxFunctionDef& x);              //!< Copy constructor
	    virtual                        ~SyntaxFunctionDef();                                        //!< Destructor

        // Assignment operator
        SyntaxFunctionDef&              operator=(const SyntaxFunctionDef& x);                      //!< Assignment operator

        // Basic utility functions
        std::string                     briefInfo() const;                                          //!< Brief info about object
        SyntaxFunctionDef*              clone() const;                                              //!< Clone object
        const VectorString&             getClass(void) const;                                       //!< Get class vector 
        const TypeSpec&                 getTypeSpec(void) const;                                    //!< Get language type of the object
        void                            print(std::ostream& o) const;                               //!< Print info about object

        // Regular functions
        RbPtr<Variable>                 evaluateContent(RbPtr<Environment> env);                    //!< Get semantic value

    protected:
        RbPtr<TypeSpec>                 returnType;                                                 //!< The return type specification of the function
        RbPtr<RbString>                 functionName;                                               //!< The name of the function
        RbPtr<std::list<RbPtr<SyntaxFormal> > >       formalArgs;                                                 //!< The formal arguments
        RbPtr<std::list<RbPtr<SyntaxElement> > >      code;                                                       //!< The list of statements
    
    private:
        static const TypeSpec       typeSpec;
};

#endif

