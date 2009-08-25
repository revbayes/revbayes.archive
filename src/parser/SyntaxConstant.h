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

using namespace std;

/*! This is the class used to hold constants in the syntax tree.
 *
 *  The result is never going to change so we can set the value
 *  already in the constructor.
 */
class SyntaxConstant : public SyntaxElement {

    public:
        enum operatorT { BOOL, INT, REAL };

            SyntaxConstant(const bool v);       //!< Constructor from RbBool
            SyntaxConstant(const int v);        //!< Constructor from RbBool
            SyntaxConstant(const double v);     //!< Constructor from RbBool
	        virtual ~SyntaxConstant() {}        //!< Destructor

        virtual bool        isSyntaxCorrect() const { return true; }    //!< Syntax errors not possible
        virtual void        print(ostream &c) const;    //!< Print content

    private:
        operatorT   op;    
};

#endif
