/**
 * @file
 * This file contains the implementation of Container,
 * which is a generic container of RbObject objects.
 *
 * @brief Implementation of Container
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-05, version 1.0
 *
 * $Id$
 */

#include "Container.h"
#include "RbException.h"
#include "RbNames.h"
#include "TypeSpec.h"
#include "VectorInteger.h"
#include "VectorNatural.h"
#include "VectorString.h"
#include "Workspace.h"

#include <sstream>


/** Construct container from type specification */
Container::Container(const TypeSpec& typeSpec) : ConstantMemberObject(), elementType(typeSpec.getType()) {

    length = 0;
}

/** Construct container from type specification and member rules */
Container::Container(const TypeSpec& typeSpec, const MemberRules& mRules) : ConstantMemberObject(mRules), elementType(typeSpec.getType()) {
    
    length = 0;
}



/** Assignment operator checks type and dim */
Container& Container::operator=( const Container& x) {

    if ( this != &x ) {
        
        if ( elementType != x.elementType )
            throw RbException( "Type mismatch in container assignment" );

        length = x.length;
    }

    return ( *this );
}


/** Return begin iterator */
ContainerIterator Container::begin(void) const {

//    VectorInteger temp = getLength();
//    for (size_t i=0; i<temp.size(); i++)
//        temp[i] = 0;
//
//    return ContainerIterator(temp, getLength());
    
    throw RbException( "Not implemented method Container::begin()" );
}

/** Return end iterator */
ContainerIterator Container::end(void) const {

//    VectorInteger temp = getLength();
//    for (size_t i=0; i<temp.size(); i++)
//        temp[i]--;
//
//    ContainerIterator tempIt(temp, getLength());
//    return ++tempIt;
    
    throw RbException( "Not implemented method Container::end()" );
}


/** Get class vector describing type of object */
const VectorString& Container::getClass(void) const {

    static VectorString rbClass = VectorString(Container_name) + ConstantMemberObject::getClass();
    return rbClass;
}




