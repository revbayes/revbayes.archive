/**
 * @file
 * This file contains the implementation of ValueContainer,
 * which is a generic container of RbObject values.
 *
 * @brief Implementation of ValueContainer
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

#include "ConstantNode.h"
#include "ContainerNode.h"
#include "ContainerIterator.h"
#include "DAGNode.h"
#include "MemberNode.h"
#include "MemberObject.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbOptions.h"
#include "TypeSpec.h"
#include "ValueContainer.h"
#include "VariableContainer.h"
#include "VectorInteger.h"
#include "VectorNatural.h"
#include "VectorString.h"

#include <cassert>
#include <iostream>
#include <sstream>


/**
 * Construct vector with n copies of x. If x is a Container,
 * we throw an error rather than trying to flatten it out.
 *
 * @note Calling the constructor with a NULL argument
 *       will result in an error.
 */
ValueContainer::ValueContainer( size_t n, RbObject* x )
    : Container( TypeSpec( x->getType(), 1 ) ) {

    if ( x->getDim() > 0 )
        throw RbException( "Illegal construction of container from container variable" );

    // Fill the vector
    elements.push_back( x );
    for ( size_t i = 1; i < n; i++ )
        elements.push_back( x->clone() );
    
    // Set length
    length[0] = elements.size();
}


/**
 * Construct array of given dimensions containing copies of x.
 * If x is a container, we throw an error rather than trying
 * to flatten it out, which would violate the length specification,
 * among other problems. This is guaranteed by the call to
 * the Container constructor, which will refuse to deal with
 * containers of containers.
 *
 * @note Calling the constructor with a NULL argument
 *       will result in an error.
 */
ValueContainer::ValueContainer( const std::vector<size_t>& len, RbObject* x )
    : Container( TypeSpec( x->getType(), len.size() ) ) {

    // Check that length specification is OK
    if ( len.size() == 0 )
        throw RbException( "Length is empty" );
    for ( size_t i = 0; i < len.size(); i++ )
        if ( len[i] < 1 )
            throw RbException( "Length is nonpositive in at least one dimension" );

    // Calculate size
    int size = 1;
    for ( size_t i = 0; i < len.size(); i++ )
        size *= len[i];

    // Fill the container
    elements.push_back( x );
    for ( int i = 1; i < size; i++ )
        elements.push_back( x->clone() );
    length = len;
}


/**
 * Construct empty value array. The Container class will make sure the
 * type is consistent, that is, that the caller does not try to create
 * a container of containers.
 */
ValueContainer::ValueContainer( const TypeSpec& typeSpec ) : Container( typeSpec ) {
}


/**
 * Construct NULL array of specified dimensions. The Container class
 * will make sure the type is consistent, that is, that the caller does not
 * try to create a container of containers.
 */
ValueContainer::ValueContainer( const TypeSpec& typeSpec , const std::vector<size_t>& len )
    : Container( typeSpec ) {

    // Check that length specification is OK
    if ( len.size() == 0 )
        throw RbException( "Length is empty" );
    for ( size_t i = 0; i < len.size(); i++ )
        if ( len[i] < 1 )
            throw RbException( "Length is nonpositive in at least one dimension" );
    if ( len.size() != typeSpec.getDim() )
        throw RbException( "Mismatch between type and length specifications" );

    // Calculate size
    int size = 1;
    for ( size_t i = 0; i < len.size(); i++ )
        size *= len[i];

    // Fill the container
    for ( int i = 0; i < size; i++ )
        elements.push_back( getDefaultElement() );
    length = len;
}


/**
 * Construct value container from variable container. This
 * version will possibly trigger the evaluation of some
 * variables in the variable container. Therefore, the
 * VariableContainer reference cannot be const.
 */
ValueContainer::ValueContainer( VariableContainer& x )
    : Container( x ) {

    for ( size_t i = 0; i < x.size(); i++ )
        elements.push_back( x[i]->getValue()->clone() );

    // Note: The length variable is copied already in the Container copy constructor
}


/**
 * Construct value container from const variable container.
 * If some variables in the variable container are in a volatile
 * state (they have been touched and not recalculated), the
 * they will throw an error upon the attempt to retrieve their
 * value.
 */
ValueContainer::ValueContainer( const VariableContainer& x )
    : Container( x ) {

    for ( size_t i = 0; i < x.size(); i++ )
        elements.push_back( x[i]->getValue()->clone() );

    // Note: The length variable is copied already in the Container copy constructor
}


/**
 * Copy constructor needed to make sure elements are independent
 */
ValueContainer::ValueContainer( const ValueContainer& x )
    : Container( x ) {

    for ( std::vector<RbObject*>::const_iterator i = x.elements.begin(); i != x.elements.end(); i++) {
        if ( (*i) == NULL )
            elements.push_back( NULL );
        else
            elements.push_back( (*i)->clone() );
    }

    length = x.length;
}


/**
 * Destructor needed to destroy elements. This will destroy all
 * DAG nodes used as member variables by MemberObject elements,
 * unless they are references or they are being referred to by
 * other variables, in which case ownership is passed on to one
 * of the referring slots.
 */
ValueContainer::~ValueContainer() {

    for (std::vector<RbObject*>::iterator i=elements.begin(); i!=elements.end(); i++)
        delete (*i);
}


/** Assignment operator; make sure we get independent elements */
ValueContainer& ValueContainer::operator=( const ValueContainer& x ) {

    if ( this != &x ) {

        // First assign using parent assignment operator. This will test to make sure the containers
        // are of the same type, and throw an error if they are not. By calling it before we destroy
        // our own elements, we can make sure that an assignment error leaves us intact, which it should
        Container::operator=( x );

        for ( std::vector<RbObject*>::iterator i = elements.begin(); i != elements.end(); i++ )
            delete ( *i );
        elements.clear();

        for ( std::vector<RbObject*>::const_iterator i = x.elements.begin(); i != x.elements.end(); i++ )
            elements.push_back( (*i)->clone() );
    }

    return ( *this );
}


/** Clear contents of value container and make length 0 in all dimensions */
void ValueContainer::clear( void ) {

    for ( std::vector<RbObject*>::iterator i = elements.begin(); i != elements.end(); i++ ) {
        delete ( *i );
    }
    elements.clear();
    for ( std::vector<size_t>::iterator i = length.begin(); i != length.end(); i++ )
        (*i) = 0;
}


/** Clone function */
ValueContainer* ValueContainer::clone(void) const {

    return new ValueContainer(*this);
}


/** Get class vector describing type of object */
const VectorString& ValueContainer::getClass(void) const {

    static VectorString rbClass = VectorString(ValueContainer_name) + Container::getClass();
    return rbClass;
}


/**
 * Get element or subcontainer corresponding to index, which can have fewer dimensions
 * or more dimensions than length. Also, the index can be negative for some of the dimensions,
 * indicating that all elements in that dimension should be included in the return variable,
 * which is then itself a container.
 */
DAGNode* ValueContainer::getElement( VectorInteger& index ) {

    // Drop any negative indices at the end of the index vector because they do not matter.
    // Also count number of negative indices that pertain to this container
    for ( size_t i = index.size() - 1; i >= 0; i-- ) {
        if ( index[i] < 0)
            index.pop_back();
        else
            break;
    }
    size_t numNegativeIndices = 0;
    for ( size_t i = 0; i < index.size(); i-- ) {
        if ( index[i] < 0)
            numNegativeIndices++;
    }

    // Check that all relevant indices are within bounds
    size_t min = index.size() < length.size() ? index.size() : length.size();
    for ( size_t i = 0; i < min; i++ ) {
    
        if ( index[i] >= int( length[i] ) )
            throw RbException( "Index out of bounds" );
    }

    // Branch out depending on the number of indices
    if ( index.size() > length.size() ) {

        // Index goes into elements; check that it actually points to an element
        for ( size_t i = 0; i < length.size(); i++ ) {
            if ( index[i] < 0 ) {
                std::ostringstream msg;
                msg << "Invalid index into subcontainer element of " << getTypeSpec();
                throw RbException( msg );
            }
        }

        VectorNatural elemIndex;
        VectorInteger valueIndex;
        size_t i = 0;
        for ( ; i < length.size(); i++ )
            elemIndex.push_back( index[i] );
        for ( ; i < index.size(); i++ )
            valueIndex.push_back( index[i] );

        RbObject* elemPtr = elements[ getOffset( elemIndex ) ];
        if ( elemPtr == NULL )
            throw RbException( "Index goes into a NULL object" );
        if ( !elemPtr->isType( MemberObject_name ) )
            throw RbException( "Container element does not support subscripting" );
        else {
            // Get member object pointer to the element
            MemberObject* elem = static_cast<MemberObject*>( elemPtr );            

            // Truncate index and delegate job to subelement
            size_t subIndex = valueIndex[0];
            valueIndex.pop_front();
            index = valueIndex;
            if ( index.size() == 0 )
                return elem->getSubelement( subIndex );
            else
                return elem->getSubelement( subIndex )->getElement( index );

        }
    }
    else if ( index.size() == 0 ) {

        // We want the entire container, easy
        return new ContainerNode( this->clone() );
    }
    else if ( index.size() == length.size() && numNegativeIndices == 0 ) {

        // We want an element, easy
        RbObject* elemPtr = elements[ getOffset( index ) ];
        if ( elemPtr == NULL )
            return new ConstantNode( elementType );
        else if ( elemPtr->isType( MemberObject_name ) )
            return new MemberNode( static_cast<MemberObject*>( elemPtr->clone() ) );
        else
            return new ConstantNode( elemPtr->clone() );
    }
    else {

        // We want a subcontainer

        // Get the number of elements in the subcontainer and its length and dim
        // Get the index to the first included element at the same time
        std::vector<size_t> tempLen;
        VectorInteger       tempIndex;      // VectorNatural does not trust us to get l-values so we can modify its content directly...
        size_t size = 1;
        for ( size_t i = 0; i < length.size(); i++ ) {
            
            if ( ( i < index.size() && index[i] < 0 ) || ( i >= index.size() ) ) {
                tempLen.push_back( length[i] );
                size *= length[i];
                tempIndex.push_back( 0 );           // An index we should vary
            }
            else
                tempIndex.push_back( index[i] );    // An index to keep fixed to index[i]
        }

        // Check that we have at least some elements in the subcontainer
        if ( tempLen.size() == 0 || size == 1 )
            throw RbException( "Empty subcontainer" );
        
        // Create the subcontainer (empty at first is probably best option)
        ValueContainer* temp = new ValueContainer( TypeSpec( elementType, tempLen.size() ) );

        // Now extract the elements
        size_t fromIndex = getOffset( tempIndex );
        for ( ; ; ) {
        
            // Set toIndex to fromIndex, keep track of number of locked dimensions in tempIndex
            size_t  toIndex     = fromIndex;
            size_t  numLocks    = length.size() - tempLen.size(); 

            // Increment tempIndex and toIndex until next nonincluded element or the end of the array
            for ( toIndex = fromIndex; toIndex < elements.size(); toIndex++ ) {
            
                // Check if we are still inside the subcontainer
                if ( numLocks < length.size() - tempLen.size() )
                    break;

                size_t i;
                for ( i = tempIndex.size() - 1; i >= 0; i-- ) {
                    
                    if ( i < index.size() && tempIndex[i] == index[i] )
                        numLocks--;

                    tempIndex[i]++;
                    if ( tempIndex[i] == length[i] )
                        tempIndex[i] = 0;

                    if ( tempIndex[i] != 0)
                        break;
                }
            }
            
            // Add vector of elements from fromIndex to (but not including) toIndex
            for ( size_t i = fromIndex; i < toIndex; i++ )
                temp->elements.push_back( elements[i]->clone() );

            // Increment tempIndex and fromIndex until next included element or the end of the array
            for ( fromIndex = toIndex; fromIndex < elements.size(); fromIndex++ ) {
            
                // Check if we now have an included element
                if ( numLocks == length.size() - tempLen.size() )
                    break;

                size_t i;
                for ( i = tempIndex.size() - 1; i >= 0; i-- ) {
                    
                    // Find out whether we will decrease the number of locks
                    if ( i < index.size() && tempIndex[i] == index[i] )
                            numLocks--;

                    tempIndex[i]++;
                    if ( tempIndex[i] == length[i] )
                        tempIndex[i] = 0;

                    // Find out whether we increased the number of locks
                    if ( i < index.size() && tempIndex[i] == index[i] )
                            numLocks++;
                    
                    if ( tempIndex[i] != 0 )
                        break;
                }
            }
        }

        // Check that we have all elements
        assert( temp->elements.size() == size );

        // We now set length of the subcontainer, and then we are done
        temp->length = tempLen;

        return new ContainerNode( temp );

    }
}


/** Set element of container with type conversion */
void ValueContainer::setElement( const VectorNatural& index, DAGNode* var ) {

    // Disallow subcontainer assignment (implict loop) for now
    if ( index.size() != length.size() ) {
    
        if ( var->numRefs() == 0 )
            delete var;
        throw RbException( "Invalid index to element" );
    }

    // Get value and check type
    RbObject* value = var->getValue()->clone();
    if ( var->numRefs() == 0 )
        delete var;

    if ( !( value->isType( elementType ) || value->isConvertibleTo( elementType, 0, true ) ) ) {

        delete value;
        throw RbException( "Invalid type when setting element of " + getTypeSpec().toString() );
    }

    // Check indices and resize if necessary
    bool grow = false;
    std::vector<size_t> newSize;
    for ( size_t i = 0; i < index.size(); i++ ) {

        if ( index[i] >= length[i] ) {
            newSize[i] = index[i] + 1;
            grow = true;
        }
        else
            newSize[i] = length[i];
    }
    if ( grow == true )
        resize( newSize );

    // Set the element, with type conversion if necessary
    if ( value == NULL || value->isType( elementType ) ) {

        // Delete the old element
        size_t offset = getOffset( index );
        if ( elements[offset] != NULL )
            delete elements[offset];
    
        elements[offset] = value;
    }
    else if ( value->isConvertibleTo( elementType, 0, true ) ) {

        // Delete the old element
        size_t offset = getOffset( index );
        if ( elements[offset] != NULL )
            delete elements[offset];
    
        elements[offset] = value->convertTo( elementType, 0 );
        delete value;
    }
}


/** Print value for user */
void ValueContainer::printValue( std::ostream& o ) const {

    if ( length.size() == 1 ) {
        o << "[ ";
        for ( std::vector<RbObject*>::const_iterator i = elements.begin(); i != elements.end(); i++ ) {
            if ( i != elements.begin() )
                o << ", ";
            if ( (*i) == NULL )
                o << "NULL";
            else
                (*i)->printValue(o);
        }
        o <<  " ]";
    }
    else {
        size_t elementsIndex = 0;
        ContainerIterator index = begin();
        while ( index != end() ) {
            /* print one row in the last dimension at a time */
            for ( size_t i = 0; i < length.size() - 1; i++ )
                o << "[" << index[i] << "]";
            o << "[]" << std::endl;
            o << "[ ";
            for ( size_t i=0; i < length[ length.size()-1 ]; i++, index++, elementsIndex++) {
                if ( i != 0 )
                    o << ", ";
                if ( elements[elementsIndex] == NULL )
                    o << "NULL";
                else
                    elements[elementsIndex]->printValue(o);
            }
            o <<  " ]";
        }
    }
}


/** Reset container length in different dimensions */
void ValueContainer::resize( const std::vector<size_t>& len ) {

    // Check if there is anything to do
    if ( len == length )
        return;

    // Check that new length specification has the right number of dimensions
    if ( len.size() != length.size() )
        throw RbException( "Invalid attempt to change container dimensions" );

    // Check new length specification is OK
    for ( size_t i = 0; i < len.size(); i++ ) {
        if ( len[i] < length[i] )
            throw RbException( "Invalid attempt to shrink variable container length" );
    }

    // Calculate handy numbers
    VectorInteger   numValsSource   = VectorInteger(len.size(), 0);
    VectorInteger   numValsTarget   = VectorInteger(len.size(), 0);
    int             numSourceVals   = 1;
    int             numTargetVals   = 1;
    for ( size_t i = len.size() - 1; i >= 0; i-- ) {
        numSourceVals *= length[i];
        numTargetVals *= len[i];
        numValsSource[i] = numSourceVals;
        numValsTarget[i] = numTargetVals;
    }

    // Create new empty elements vector
    std::vector<RbObject*> tempElements = std::vector<RbObject*>( numValsTarget[0] );

    // Reorganize content
    size_t  sourceIndex  = 0;
    size_t  targetIndex  = 0;
    do {
        for ( size_t i = 0; i < length[length.size()-1]; i++ )
            tempElements[targetIndex++] = elements[sourceIndex++];

        size_t lastIndex = targetIndex;

        for ( size_t i = length.size()-1; i >= 0; i-- ) {
            if ( sourceIndex % numValsSource[i] == 0 ) {
                targetIndex += numValsTarget[i] - ( targetIndex % numValsTarget[i] );
            }

        for ( size_t i = lastIndex; i < targetIndex; i++ )
            tempElements[i] = getDefaultElement();
        }
    } while ( sourceIndex < elements.size() );

    // Copy the new container and length specification
    elements  = tempElements;
    length = len;
}


/** Get complete info about object */
std::string ValueContainer::richInfo(void) const {

    std::ostringstream o;
    o << "Value container:" << std::endl;
    o << "objectType  = " << getType() << std::endl;
    o << "elementType = " << getElementType() << std::endl;
    o << "dim         = " << getDim() << std::endl; 
    o << "length      = " << VectorInteger(length) << std::endl;
    o << "value       = " <<  std::endl;
    printValue(o);

    return o.str();
}

