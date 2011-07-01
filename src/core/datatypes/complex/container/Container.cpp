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
 * $Id:$
 */

#include "Container.h"
#include "ContainerNode.h"
#include "RbException.h"
#include "RbNames.h"
#include "TypeSpec.h"
#include "VectorIndex.h"
#include "VectorInteger.h"
#include "VectorNatural.h"
#include "VectorString.h"
#include "Workspace.h"

#include <sstream>


/** Construct container from type specification */
Container::Container(const TypeSpec& typeSpec)
    : RbComplex(), elementType(typeSpec.getType()), length(typeSpec.getDim(),0), names() {

    // If the workspace has been initialized, we use it to test whether we have a container or member object
    if ( Workspace::userWorkspace().areTypesInitialized() ) {
        if ( Workspace::userWorkspace().isXOfTypeY( elementType, Container_name ) )
            throw RbException( "Cannot create container of containers; only primitive types allowed" );
        if ( Workspace::userWorkspace().areTypesInitialized() && Workspace::userWorkspace().isXOfTypeY( elementType, MemberObject_name ) )
            throw RbException( "Cannot create container of member objects; only primitive types allowed" );        
    }

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
        
        names  = x.names;
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


/** Get index corresponding to a string name for dimension k */
size_t Container::getIndexOfName( size_t k, const std::string& s ) const {
    
    if ( k >= getDim() )
        throw RbException( "Dimension index out of bound" );
    
    if ( names.size() == 0 )
        throw RbException( "Names not set" );
    
    for ( size_t i=0; i<length[k]; i++ ) {
        
        if ( names[k][i] == s )
            return i;
    }
    
    std::ostringstream msg;
    msg << "No match for name '" + s + "' in dimension " << k;
    throw RbException( msg );
    
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


/** Shave off an integer index to a single container element or a subcontainer */
VectorInteger Container::getIntegerIndex( const VectorIndex& index ) const {
    
    if ( index.size() > getDim() )
        throw RbException( "Index goes beyond this container" );
    
    VectorInteger intIndex;
    for ( size_t i=0; i< index.size(); i++ ) {
        
        if ( index[i]->isType( Integer_name ) ) {
            
            intIndex.push_back( index.getInt( i ) );
        }
        else {
            
            size_t j = getIndexOfName( i, index.getString( 0 ) );
            intIndex.push_back( int( j ) );
        }
    }
    
    return intIndex;
}


/** Shave off a natural index to a single container element from a generic index vector */
VectorNatural Container::getNaturalIndex( const VectorIndex& index ) const {
    
    if ( index.size() < getDim() )
        throw RbException( "Element does not exist" );
    
    VectorNatural natIndex;
    for ( size_t i=0; i< getDim(); i++ ) {
        
        if ( index[i]->isType( Integer_name ) ) {
            
            int j = index.getInt( i );
            if ( j < 0 )
                throw RbException( "Not an element index" );
            natIndex.push_back( j );
        }
        else {
            
            size_t j = getIndexOfName( i, index.getString( 0 ) );
            natIndex.push_back( int( j ) );
        }
    }
    
    return natIndex;
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
 * Are we of specified language type? Being containers, we need
 * help from the workspace because we might not have information
 * about the class vector of our elements, which could be absent or
 * NULL at the time.
 */
bool Container::isTypeSpec(const TypeSpec& typeSpec) const {

    // Match to dimensionless object
    if ( typeSpec.isDimensionlessObject() )
        return true;

    // Check dimensions
    if ( getDim() != typeSpec.getDim() )
        return false;

    // Now we need help from the workspace to get the element class vector
    return Workspace::userWorkspace().findType( elementType )->isType( typeSpec.getType() );
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

