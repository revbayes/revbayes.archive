/*!
 * \file
 * This file contains the declaration of SyntaxHelpExpr, which is
 * used to hold help expressions in the syntax tree.
 *
 * \brief Declaration of SyntaxHelpExpr
 *
 * (c) Copyright 2009- under GPL version 3
 * \date Last modified: $Date$
 * \author Fredrik Ronquist and the REvBayes core team
 * \license GPL version 3
 *
 * $Id$
 */

#ifndef SyntaxHelpExpr_H
#define SyntaxHelpExpr_H

#include "RbString.h"
#include "SyntaxElement.h"

using namespace std;

/*! This is the class used to hold help expressions in the syntax tree.
 *
 *  The result is never going to change so we can set the value
 *  already in the constructor.
 */
class SyntaxHelpExpr : public SyntaxElement {

    public:
            SyntaxHelpExpr(const string &id);       //!< Constructor
	        virtual ~SyntaxHelpExpr();              //!< Destructor

        virtual void    print(ostream &c) const;    //!< Print content
        virtual void    printConsole(ostream &c) const;    //!< Print content to console

    private:
        const string    topic;      //! The help topic
};

#endif
