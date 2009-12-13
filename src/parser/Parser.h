/**
 * @file
 * This file contains the declaration of Parser, which is
 * used to execute syntax trees and get help information.
 *
 * @brief Declaration of Parser
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef Parser_H
#define Parser_H

class RbString;
class SyntaxElement;

class Parser {

    public:
        // Regular functions
        int     execute(SyntaxElement* root) const;     //!< Execute the syntax tree
        int     info(RbString* symbol) const;           //!< Get help for a symbol
        int     info(SyntaxElement* root) const;        //!< Get help for a function call
};

#endif

