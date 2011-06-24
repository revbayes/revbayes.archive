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
#include "ContainerNode.h"
#include "RbException.h"
#include "RbNames.h"
#include "TypeSpec.h"
#include "VectorInteger.h"
#include "VectorNatural.h"
#include "VectorString.h"
#include "Workspace.h"

#include <sstream>


/** Construct container from type specification */
Container::Container(const TypeSpec& typeSpec)
    : RbComplex(), elementType(typeSpec.getType()), length(typeSpec.getDim(),0) {

    // If the workspace has been initialized, we use it to test whether we have a container
    if ( Workspace::userWorkspace().areTypesInitialized() && Workspace::userWorkspace().isXOfTypeY( elementType, Container_name ) )
        throw RbException( "Cannot create container of containers" );

    // Check that length specification is OK
    if ( length.size() == 0 )
        throw RbException( "Invalid attempt to create a scalar container" );
}


/** Assignment operator checks type and dim */
Container& Container::operator=( const Container& x) {

    if ( this != &x ) {
        
        if ( elementType != x.elementType )
            throw RbException( "Type mismatch in container assignment" );

        if ( length.size() != x.length.size() )
            throw RbException( "Dimension mismatch in container assignment" );

        length = x.length;
    }

    return ( *this );
}



/** Return begin iterator */
ContainerIterator Container::begin(void) const {

    VectorInteger temp = getLength();
    for (size_t i=0; i<temp.size(); i++)
        temp[i] = 0;

    return ContainerIterator(temp, getLength());
}

/** Return end iterator */
ContainerIterator Container::end(void) const {

    VectorInteger temp = getLength();
    for (size_t i=0; i<temp.size(); i++)
        temp[i]--;

    ContainerIterator tempIt(temp, getLength());
    return ++tempIt;
}


/** Get class vector describing type of object */
const VectorString& Container::getClass(void) const {

    static VectorString rbClass = VectorString(Container_name) + RbComplex::getClass();
    return rbClass;
}


/** Get container and subscript indices */
void Container::getContainerSubscriptIndices( const VectorInteger& index, VectorInteger& containerIndex, VectorInteger& subscriptIndex ) const {

    containerIndex = index;
    subscriptIndex.clear();
    for ( size_t i = length.size(); i < index.size(); i++ ) {
        subscriptIndex.push_back( index[i] );
        containerIndex.pop_back();
    }
}


/** Get offset to element or subcontainer; also check index */
size_t Container::getOffset( const VectorNatural& index ) const {

    if ( index.size() > length.size() )
        throw RbException( "Container index has too many dimensions" );

    for ( size_t i = 0; i < index.size(); i++) {
        if ( index[i] >= length[i] )
            throw RbException( "Container index out of range" );
    }

    size_t  offset  = 0;
    size_t  numVals = 1;
    int     k       = static_cast<int>( length.size() ) - 1;
    for ( ; k >= int( index.size() ); k--)
         numVals *= length[k];
    for ( ; k >= 0; k--) {
        offset += ( index[k] * numVals );
        numVals *= length[k];
    }
 
    return offset;
}


/** Get language type specification of object. Unlike other RbObject objects, we
    allow the parser to see the elements of the container */
const TypeSpec Container::getTypeSpec( void ) const {

    return TypeSpec( elementType, getDim() );
}


/**
 * Set length in each dimension: reorganize the content of the container
 *
 * Whatever order (by row or by column) assumed by the class ContainerIterator
 * will be used when reordering the elements in this function, which allows
 * us to leave the elemnts vector untouched.
 */
void Container::setLength( const std::vector<size_t>& len ) {

    if ( len.size() != length.size() )
        throw RbException( "Invalid attempt to change dimensions of container" );

    size_t  numOldVals = 1;
    size_t  numNewVals = 1;
    for ( size_t i = 0; i < len.size(); i++) {
        if ( len[i] <= 0 )
            throw RbException( "Nonpositive dimension length" );
        numNewVals *= len[i];
        numOldVals *= len[i];
    }

    if ( numNewVals != numOldVals )
        throw RbException( "New length does not match container size" );

    length = len;
}


/** Wrap value into a variable */
DAGNode* Container::wrapIntoVariable( void ) {
    
    return new ContainerNode( this );
}

