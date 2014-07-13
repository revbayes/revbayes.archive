/**
 * @file
 * This file contains the declaration of SyntaxClassDef, which is
 * used to hold the definitions of user-defined types.
 *
 * @brief Declaration of SyntaxClassDef
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef SyntaxClassDef_H
#define SyntaxClassDef_H

#include "SyntaxElement.h"
#include "SyntaxFormal.h"
#include "SyntaxVariable.h"

#include <ostream>
#include <list>
#include <string>

namespace RevLanguage {
    
class SyntaxClassDef : public SyntaxElement {

    public:
        SyntaxClassDef(const std::string &name, const std::string &base, std::list<SyntaxElement*>* defs);          //!< Standard constructor
        SyntaxClassDef(const SyntaxClassDef& x);                                                                    //!< Copy constructor
    
	    virtual                    ~SyntaxClassDef();                                                               //!< Destructor

        // Assignment operator
        SyntaxClassDef&             operator=(const SyntaxClassDef& x);                                             //!< Assignment operator

        // Basic utility functions
        SyntaxElement*              clone() const;                                                                  //!< Clone object
        void                        printValue(std::ostream& o) const;                                              //!< Print info about object

        // Regular functions
        RevPtr<Variable>            evaluateContent( Environment& env );                                            //!< Get semantic value

    protected:
        std::string                 className;                                                                      //!< The name of the class
        std::string                 baseClass;                                                                      //!< The name of the base class
        std::list<SyntaxElement*>*  definitions;                                                                    //!< Defined member variables and functions
    
};
    
}

#endif

