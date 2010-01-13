/**
 * @file
 * This file contains the declaration of SyntaxFunctionDef, which is
 * used to hold definitions of functions (user-defined functions).
 *
 * @brief Declaration of SyntaxFunctionDef
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
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

#include <ostream>
#include <list>
#include <string>


class SyntaxFunctionDef : public SyntaxElement {

    public:
            SyntaxFunctionDef(  RbString* type,
                                RbString* name,
                                std::list<SyntaxFormal*>* formals,
                                std::list<SyntaxElement*>* stmts);  //!< Standard constructor
            SyntaxFunctionDef(const SyntaxFunctionDef& x);          //!< Copy constructor
	        virtual ~SyntaxFunctionDef();                           //!< Destructor

        // Basic utility functions
        std::string     briefInfo() const;                          //!< Brief info about object
        SyntaxElement*  clone() const;                              //!< Clone object
        bool            equals(const SyntaxElement* elem) const;    //!< Equals comparison
        void            print(std::ostream& o) const;               //!< Print info about object

        // Regular functions
        DAGNode*        getDAGNode(Frame* frame=NULL) const;        //!< Convert to DAG node
        RbObject*       getValue(Frame* frame=NULL) const;          //!< Get semantic value

    protected:
        RbString*                       returnType;     //!< The return type of the function
        RbString*                       functionName;   //!< The name of the function
        std::list<SyntaxFormal*>*       formalArgs;     //!< The formal arguments
        std::list<SyntaxElement*>*      code;           //!< The list of statements
};

#endif

