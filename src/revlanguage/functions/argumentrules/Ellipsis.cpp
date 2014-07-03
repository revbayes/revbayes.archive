/**
 * @file
 * This file contains the implementation of Ellipsis, which is
 * used to describe a dot dot dot formal (...).
 *
 * @brief Implementation of Ellipsis 
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 *
 * $Id$
 */

#include "Ellipsis.h"
#include "RbUtil.h"
#include "TypeSpec.h"



/** Constructor allowing all types; ref MemberObject whether arguments should be values or references */
RevLanguage::Ellipsis::Ellipsis( void ) : ArgumentRule("", true, RevObject::getClassTypeSpec() ) {
}


/** Constructor requiring a certain type specification */
RevLanguage::Ellipsis::Ellipsis(const TypeSpec& typeSp ) : ArgumentRule("", true, typeSp) {
}



bool RevLanguage::Ellipsis::isEllipsis( void ) const {
    return true;
}



/** Print value for user (in descriptions of functions, for instance */
void RevLanguage::Ellipsis::printValue(std::ostream &o) const {

    
    for ( std::vector<TypeSpec>::const_iterator it = argTypeSpecs.begin(); it != argTypeSpecs.end(); ++it )
    {
        if ( it != argTypeSpecs.begin() )
            o << "|";
        o << *it;
    }
    o << " ...";
}

