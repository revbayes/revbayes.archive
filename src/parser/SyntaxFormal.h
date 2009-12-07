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
 * @author The RevBayes core development team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef SyntaxFormal_H
#define SyntaxFormal_H

#include "RbString.h"
#include "SyntaxElement.h"

#include <iostream>
#include <list>


class SyntaxFormal : public SyntaxElement {

    public:
            SyntaxFormal(RbString* id, SyntaxElement* defaultVal);                      //!< Implicit type
            SyntaxFormal(RbString* typeSpec, RbString* id, SyntaxElement* defaultVal);  //!< Explicit type
            SyntaxFormal(const SyntaxFormal& x);                                        //!< Copy constructor
	        virtual ~SyntaxFormal();                                                    //!< Destructor

        // Basic utility functions
        std::string     briefInfo() const;                          //!< Brief info about object
        SyntaxElement*  clone() const;                              //!< Clone object
        bool            equals(const SyntaxElement* elem) const;    //!< Equals comparison
        void            print(std::ostream& o) const;               //!< Print info about object

        // Regular functions
        const RbString* getLabel() const { return label; }          //!< Return label    
        DAGNode*        getDAGNode(Frame* frame=NULL) const;        //!< Convert to DAG node
        RbObject*       getValue(Frame* frame=NULL) const;          //!< Get semantic value

    protected:
        RbString*                       type;           //!< The type of the argument
        RbString*                       label;          //!< The label of the argument
        SyntaxElement*                  defaultExpr;    //!< The default value of the argument (a const expr)
};

#endif

