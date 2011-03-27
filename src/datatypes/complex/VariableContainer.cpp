/**
 * @file
 * This file contains the implementation of VariableContainer,
 * which is a container of variables.
 *
 * @brief Implementation of VariableContainer
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
#include "Boolean.h"
#include "Integer.H"
#include "MatrixReal.h"
#include "MemberNode.h"
#include "MemberObject.h"
#include "Natural.h"
#include "Real.h"
#include "RealPos.h"
#include "RbString.h"
#include "RbException.h"
#include "RbNames.h"
#include "TypeSpec.h"
#include "ValueContainer.h"
#include "VariableContainer.h"
#include "VectorBoolean.h"
#include "VectorInteger.h"
#include "VectorNatural.h"
#include "VectorReal.h"
#include "VectorRealPos.h"
#include "VectorString.h"

#include <cassert>
#include <iostream>
#include <sstream>


/**
 * Construct vector with n copies of x. If x is a ContainerNode,
 * we throw an error rather than trying to flatten it out.
 *
 * @note Calling the constructor with a NULL argument
 *       will result in a runtime error.
 */
VariableContainer::VariableContainer( size_t n, DAGNode* x )
    : Container( TypeSpec( x->getValueType(), 1 ) ) {

    if ( x->getSize() > 0 )
        throw RbException( "Illegal construction from container variable" );

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
 * among other problems.
 */
VariableContainer::VariableContainer( const std::vector<int>& len, DAGNode* x )
    : Container( TypeSpec( x->getValueType(), len.size() ) ) {

    // Check that length specification and input object are OK
    if ( len.size() == 0 )
        throw RbException( "Length is empty" );
    for ( size_t i = 0; i < len.size(); i++ )
        if ( len[i] < 1 )
            throw RbException( "Length is nonpositive in at least one dimension" );
    if ( x->getDim() != 0 )
        throw RbException( "Invalid attempt to construct container of containers" );

    // Calculate size
    int size = 1;
    for ( size_t i = 0; i < len.size(); i++ )
        size *= len[i];

    // Fill the container
    elements.push_back( x );
    for ( int i=1; i<size; i++ )
        elements.push_back( x->clone() );
    length = len;
}


/**
 * Construct empty variable array. The Container class will make sure the
 * type is consistent, that is, that the caller does not try to create
 * a container of containers.
 */
VariableContainer::VariableContainer( const TypeSpec& typeSpec ) : Container( typeSpec ) {
}


/**
 * Construct NULL array of specified dimensions. The Container class
 * will make sure the type is consistent, that is, that the caller does not
 * try to create a container of containers.
 */
VariableContainer::VariableContainer( const TypeSpec& typeSpec , const std::vector<int>& len )
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
        elements.push_back( new ConstantNode( NULL ) );
    length = len;
}


/**
 * Copy constructor needed to make sure elements are independent
 */
VariableContainer::VariableContainer(const VariableContainer& x)
    : Container(x) {

    for (std::vector<DAGNode*>::const_iterator i=x.elements.begin(); i!=x.elements.end(); i++)
        elements.push_back( (*i)->clone() );
}


/**
 * Destructor needed to destroy elements. This will destroy all
 * temp DAG nodes hooked up to the container elements.
 */
VariableContainer::~VariableContainer() {

    for (std::vector<DAGNode*>::const_iterator i=elements.begin(); i!=elements.end(); i++) {
        if ( (*i)->numRefs() == 0 )
            delete (*i);
    }
}


/**
 * Assignment operator needed to make sure we get independent elements
 */
VariableContainer& VariableContainer::operator=( const VariableContainer& x ) {

    if ( this != &x ) {

        // First assign using parent assignment operator. This will test to make sure the containers
        // are of the same type, and throw an error if they are not. By calling it before we destroy
        // our own elements, we can make sure that an assignment error leaves us intact, which it should
        Container::operator=( x );

        // Delete old elements; this will take all temps with it
        for ( std::vector<DAGNode*>::iterator i = elements.begin(); i != elements.end(); i++ ) {
            if ( (*i)->numRefs() == 0 )
                delete (*i);
        }
        elements.clear();

        // Fill in new elements. Container assignment operator took care of type and length
        for ( std::vector<DAGNode*>::const_iterator i=x.elements.begin(); i!=x.elements.end(); i++ )
            elements.push_back( (*i)->clone() );
    }
    return (*this);
}


/**
 * Subscript operator. ContainerIterator and VectorNatural indices can also be passed in
 * because of the type conversion constructors implemented in the VectorInteger class.
 */
DAGNode*& VariableContainer::operator[]( const VectorInteger& i ) {

    size_t offset = getOffset( i );
    return elements[offset];
}


/**
 * Subscript operator (const). ContainerIterator and VectorNatural indices can also be passed
 * in because of the type conversion constructors implemented in the VectorInteger class.
 */
const DAGNode* const& VariableContainer::operator[]( const VectorInteger& i ) const {

    size_t offset = getOffset( i );
    return elements[offset];
}


/** Clear contents of variable container and make length 0 in all dimensions */
void VariableContainer::clear(void) {

    for ( size_t i = 0; i < elements.size(); i++ ) {
        if ( elements[i]->numRefs() == 0 )
            delete elements[i];
    }
    elements.clear();
    for ( std::vector<int>::iterator i = length.begin(); i != length.end(); i++ )
        (*i) = 0;
}


/** Clone function */
VariableContainer* VariableContainer::clone(void) const {

    return new VariableContainer(*this);
}


/** Get class vector describing type of object */
const VectorString& VariableContainer::getClass(void) const {

    static VectorString rbClass = VectorString(VariableContainer_name) + Container::getClass();
    return rbClass;
}


/**
 * Get const value. In this function, we get the value from each of the 
 * elements and create a constant value container with this content.
 */
ValueContainer* VariableContainer::getConstValue( void ) {

    if ( elementType == Integer_name && getDim() == 1 ) {

        VectorInteger* temp = new VectorInteger();
        for ( size_t i = 0; i < elements.size(); i++ )
            temp->push_back( static_cast<const Integer*>( elements[i]->getValue() )->getValue() );
        return temp;
    }
    else if ( elementType == Natural_name && getDim() == 1 ) {

        VectorNatural* temp = new VectorNatural();
        for ( size_t i = 0; i < elements.size(); i++ )
            temp->push_back( static_cast<const Natural*>( elements[i]->getValue() )->getValue() );
        return temp;
    }
    else if ( elementType == Real_name && getDim() == 1 ) {

        VectorReal* temp = new VectorReal();
        for ( size_t i = 0; i < elements.size(); i++ )
            temp->push_back( static_cast<const Real*>( elements[i]->getValue() )->getValue() );
        return temp;
    }
    else if ( elementType == RealPos_name && getDim() == 1 ) {

        VectorRealPos* temp = new VectorRealPos();
        for ( size_t i = 0; i < elements.size(); i++ )
            temp->push_back( static_cast<const RealPos*>( elements[i]->getValue() )->getValue() );
        return temp;
    }
    else if ( elementType == Boolean_name && getDim() == 1 ) {

        VectorBoolean* temp = new VectorBoolean();
        for ( size_t i = 0; i < elements.size(); i++ )
            temp->push_back( static_cast<const Boolean*>( elements[i]->getValue() )->getValue() );
        return temp;
    }
    else if ( elementType == RbString_name && getDim() == 1 ) {

        VectorString* temp = new VectorString();
        for ( size_t i = 0; i < elements.size(); i++ )
            temp->push_back( static_cast<const RbString*>( elements[i]->getValue() )->getValue() );
        return temp;
    }
    else if ( elementType == Real_name && getDim() == 2 ) {

        std::vector<double> temp;
        for ( size_t i = 0; i < elements.size(); i++ )
            temp.push_back( static_cast<const Real*>( elements[i]->getValue() )->getValue() );
        return new MatrixReal( length, temp );
        return NULL;
    }
    else
        return new ValueContainer( this );
}


/** Get element (read-only) */
const RbObject* VariableContainer::getElement( const VectorInteger& index ) const {

    // Check that the index is not to a subcontainer
    if ( index.size() < length.size() )
        throw RbException( "Invalid index to element" );

    if ( index.size() == length.size() ) {
    
        // The index is to a variable, so it is easy
        return elements[ getOffset( index ) ]->getValue();
    }
    else {
    
        // The index goes into the subscripts of a value
        // so it is a little more involved to get the element

        // First, split the index up
        VectorInteger containerIndex;
        VectorInteger subscriptIndex;
        getContainerSubscriptIndices( index, containerIndex, subscriptIndex );

        // Get the variable
        DAGNode* theVariable = elements[ getOffset( containerIndex ) ];

        // Ask the variable to give us the value element
        return theVariable->getValElement( subscriptIndex );
    }
}


/** Get element from vector of elements (read-only) */
const RbObject* VariableContainer::getElement( size_t i ) const {

    // Check that the index is within bounds
    if ( i > elements.size() )
        throw RbException( "Content element index out of bound" );

    return elements[i]->getValue();
}


/**
 * Get subcontainer corresponding to index, which can have fewer dimensions than
 * length. Also, the index can be negative for some of the dimensions, indicating
 * that all elements in that dimension should be included in the subcontainer.
 */
VariableContainer* VariableContainer::getSubContainer( const VectorInteger& index ) const {

    // Check that we have an index to a subcontainer
    if ( index.size() == 0 )
        throw RbException( "Index to subcontainer empty" );
    if ( index.size() >= length.size() )
        throw RbException( "Invalid index to subcontainer" );
    for ( size_t i = 0; i < index.size(); i++ ) {
        if ( index[i] >= length[i] )
            throw RbException( "Subcontainer index out of bound" );
    }

    // Get the number of elements in the subcontainer and its length and dim
    // Get the index to the first included element at the same time
    std::vector<int>    tempLen;
    VectorInteger       tempIndex;
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
    VariableContainer* temp = new VariableContainer( TypeSpec( elementType, tempLen.size() ) );

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

    return temp;
}


/** Set value element or elements from value */
void VariableContainer::setElement( const VectorInteger& index, RbObject* val ) {

    // Resize but only if it is an assignment to a container element
    if ( index.size() == length.size() ) {

        std::vector<int>    tempLen  = length;
        bool                growSize = false;

        for ( size_t i = 0; i < index.size(); i++ ) {
            
            if ( index[i] >= tempLen[i] ) {
                tempLen[i] = index[i] + 1;
                growSize = true;
            }
        }

        if ( growSize )
            resize( tempLen );
    }

    // Get container and subscript indices
    VectorInteger   containerIndex;
    VectorInteger   subscriptIndex;
    getContainerSubscriptIndices( index, containerIndex, subscriptIndex );

    // Get offset; also checks for errors in index
    size_t offset = getOffset( containerIndex );

    // Branch depending on index of call
    if ( containerIndex.size() < length.size() ) {

        /* TODO: Do we want to allow this? See code below if we do */
        throw RbException( "Invalid assignment to subcontainer" );

        // Check that the source is a container
        if ( !val->isType( RbComplex_name ) )
            throw RbException( "Source does not have elements" );
        RbComplex* source = dynamic_cast<RbComplex*>( val );
    
        // Count number of elements
        const std::vector<int>& sourceLen           = source->getLength();
        int                     numSourceElements   = 1;
        int                     numTargetElements   = 1;

        for ( size_t i = 0; i < sourceLen.size(); i++ )
            numSourceElements *= sourceLen[i];

        for ( size_t i = index.size(); i < length.size(); i++ )
            numTargetElements *= length[i];

        // Throw an error if a mismatch
        if ( numSourceElements != numTargetElements )
            throw RbException("Unequal source and target sizes");

        // Cycle through assignments
        for ( ContainerIterator i = source->begin(); i != source->end(); ++i ) {

            // Do the assignment. Note that we want to do type checking
            // here to allow assignment when the source elements have the
            // right type regardless of what the general return type of
            // the 'subscript operator' (getElement) of the source object might be
            const RbObject* elem = source->getElement( i );
            if ( elem == NULL || elem->isType( elementType ) ) {
                delete elements[offset];
                if ( elem == NULL )
                    elements[offset] = new ConstantNode( elementType );
                else if ( elem->isType( MemberObject_name ) )
                    elements[offset] = new MemberNode( static_cast<MemberObject*>( elem->clone() ) );
                else
                    elements[offset] = new ConstantNode( elem->clone() );
            }
            else
                throw RbException( "Incompatible types in subcontainer assignment" );

            // Increase offset for target elements
            offset++;
        }

        delete val;     // We are responsible for deleting the input container
    }
    else if ( subscriptIndex.size() == 0 ) {

        // Parser wants to set a single element
        if ( val == NULL || val->isType( elementType ) ) {

            if ( val == NULL )
                elements[offset] = new ConstantNode( elementType );
            else if (val->isType( MemberObject_name ) )
                elements[offset] = new MemberNode( static_cast<MemberObject*>( val ) );
            else
                elements[offset] = new ConstantNode( val );
        }
        else
            throw RbException( "Incompatible types in container element assignment" );
    }
    else {

        // Parser wants to set a subscript element
        // We delegate to the variable element
        elements[offset]->setElement( subscriptIndex, val );
    }
}


/**
 * Print value for user. Since this function is unlikely to be
 * critical for performance, we simply get the value container
 * and use its printing function.
 */
void VariableContainer::printValue( std::ostream& o ) const {

    ValueContainer* temp = new ValueContainer( *this );

    temp->printValue( o );

    delete temp;
}


/** Reset container length in different dimensions */
void VariableContainer::resize( const std::vector<int>& len ) {

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
    std::vector<DAGNode*> tempElements = std::vector<DAGNode*>( numValsTarget[0] );

    // Reorganize content
    size_t  sourceIndex  = 0;
    size_t  targetIndex  = 0;
    do {
        for ( int i = 0; i < length[length.size()-1]; i++ )
            tempElements[targetIndex++] = elements[sourceIndex++];

        size_t lastIndex = targetIndex;

        for ( size_t i = length.size()-1; i >= 0; i-- ) {
            if ( sourceIndex % numValsSource[i] == 0 ) {
                targetIndex += numValsTarget[i] - ( targetIndex % numValsTarget[i] );
            }

        for ( size_t i = lastIndex; i < targetIndex; i++ )
            tempElements[i] = new ConstantNode( elementType );
        }
    } while ( sourceIndex < elements.size() );

    // Copy the new container and length specification
    elements  = tempElements;
    length = len;
}


/** Get complete info about object */
std::string VariableContainer::richInfo( void ) const {

    std::ostringstream o;
    o << "Variable container:" << std::endl;
    o << "elementType = " << elementType << std::endl;
    o << "dim         = " << getDim() << std::endl; 
    o << "length      = " << VectorNatural(length) << std::endl;
    o << "value       = " << std::endl;
    printValue( o );
    o << std::endl;

    return o.str();
}

