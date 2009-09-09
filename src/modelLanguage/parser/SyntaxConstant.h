/*!
 * \file
 * This file contains the declaration of SyntaxConstant, which is
 * used to hold constants in the syntax tree.
 *
 * \brief Declaration of SyntaxConstant
 *
 * (c) Copyright 2009- under GPL version 3
 * \date Last modified: $Date$
 * \author Fredrik Ronquist and the REvBayes core team
 * \license GPL version 3
 *
 * $Id$
 */

#ifndef SyntaxConstant_H
#define SyntaxConstant_H

#include "SyntaxElement.h"
#include <iostream>

using namespace std;

/*! This is the class used to hold constants in the syntax tree.
 *
 *  The result is never going to change so we can set the value
 *  already in the constructor.
 */
class SyntaxConstant : public SyntaxElement {

    public:
            SyntaxConstant(const bool v);       //!< Constructor from bool
            SyntaxConstant(const int v);        //!< Constructor from int
            SyntaxConstant(const double v);     //!< Constructor from double
	        virtual ~SyntaxConstant() {}        //!< Destructor (value is deleted in base class)

        SyntaxConstant* copy(SymbolTable* symbols=NULL) const;  //!< Return fast copy
        const string&   getDataType() const { return value->getType(); }    //!< Return data type
        bool            isConstExpr() const { return true; }    //!< Is this a constant expression?
        void            print(ostream &c) const;                //!< Print content
        void            printConsole(ostream &c) const;         //!< Print content to console
};

#endif
