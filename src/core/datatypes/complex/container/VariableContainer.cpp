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
#include "ContainerNode.h"
#include "ConverterNode.h"
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
#include "VectorIndex.h"
#include "VectorInteger.h"
#include "VectorNatural.h"
#include "VectorReal.h"
#include "VectorRealPos.h"
#include "VectorString.h"
#include "Workspace.h"

#include <algorithm>
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
VariableContainer::VariableContainer( const std::vector<size_t>& len, DAGNode* x )
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
VariableContainer::VariableContainer( const TypeSpec& typeSpec , const std::vector<size_t>& len )
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
        elements.push_back( new ConstantNode( typeSpec.getType() ) );
    length = len;
}


/**
 * Construct variable container from value container.
 */
VariableContainer::VariableContainer( const ValueContainer& x )
    : Container( x.getTypeSpec() ) {

    // Set length
    length = x.getLength();
    
    // Calculate size
    size_t size = 1;
    for ( size_t i = 0; i < length.size(); i++ )
        size *= length[i];

    // Fill the container
    for ( size_t i = 0; i < size; i++ )
        elements.push_back( x.elements[i]->wrapIntoVariable() );
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
DAGNode*& VariableContainer::operator[]( const VectorNatural& i ) {

    size_t offset = getOffset( i );
    return elements[offset];
}


/**
 * Subscript operator (const). ContainerIterator and VectorNatural indices can also be passed
 * in because of the type conversion constructors implemented in the VectorInteger class.
 */
const DAGNode* const& VariableContainer::operator[]( const VectorNatural& i ) const {

    size_t offset = getOffset( i );
    return elements[offset];
}


/** Subscript operator for vectors. */
DAGNode*& VariableContainer::operator[]( const size_t i )  {
    
    if ( length.size() != 1 || i >= length[0] )
        throw RbException( "Index error");
    
    return elements[i];
}


/** Subscript operator (const) for vectors. */
const DAGNode* const& VariableContainer::operator[]( const size_t i ) const {
    
    if ( length.size() != 1 || i >= length[0] )
        throw RbException( "Index error");

    return elements[i];
}


/** Clear contents of variable container and make length 0 in all dimensions */
void VariableContainer::clear(void) {

    for ( size_t i = 0; i < elements.size(); i++ ) {
        if ( elements[i]->numRefs() == 0 )
            delete elements[i];
    }
    elements.clear();
    for ( std::vector<size_t>::iterator i = length.begin(); i != length.end(); i++ )
        (*i) = 0;
}


/** Clone function */
VariableContainer* VariableContainer::clone( void ) const {

    return new VariableContainer( *this );
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
ValueContainer* VariableContainer::getConstValue( void ) const {

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
    else {
        std::vector<RbObject*> temp;
        for ( size_t i=0; i<elements.size(); i++ )
            temp.push_back( elements[i]->getValue()->clone() );
        return new ValueContainer( getTypeSpec(), getLength(), temp );
    }
}


/** Get the index of a variable element */
VectorNatural VariableContainer::getIndex(const DAGNode* elem) const {

    std::vector<DAGNode*>::const_iterator it;
    it = std::find( elements.begin(), elements.end(), elem );

    if ( it == elements.end() )
        throw RbException( elem->getName() + " is not an element" );

    ContainerIterator contIt = begin();
    for ( std::vector<DAGNode*>::const_iterator i = elements.begin(); i != it; i++ )
        contIt++;

    return VectorNatural( contIt );
}


/**
 * Get element or subcontainer corresponding to index, which can have fewer dimensions
 * or more dimensions than length. Also, the index can be negative for some of the dimensions,
 * indicating that all elements in that dimension should be included in the return variable,
 * which is then itself a container.
 */
DAGNode* VariableContainer::getElement( VectorIndex& index ) {

    // Check for superfluous indices
    if ( index.size() > getDim() )
        throw RbException( "Too many indices when accessing element of " + getTypeSpec().toString() + " object" );
    
    // Drop any negative indices at the end of the index vector because they do not matter.
    // Also count number of negative indices that pertain to this container
    for ( int i = static_cast<int>( index.size() ) - 1; i >= 0; i-- ) {
        if ( index[i] < 0)
            index.pop_back();
        else
            break;
    }
    size_t numNegativeIndices = 0;
    for ( size_t i = 0; i < index.size(); i++ ) {
        if ( index[i] < 0)
            numNegativeIndices++;
    }

    // Branch out depending on the number of indices
    if ( index.size() == 0 ) {

        // We want the entire container, easy
        return new ContainerNode( this );
    }
    else if ( index.size() == length.size() && numNegativeIndices == 0 ) {

        // Get single element index
        VectorNatural elemIndex = getNaturalIndex( index );

        // Return element
        return elements[ getOffset( elemIndex ) ];
    }
    else {

        // We want a subcontainer (this code is slow, but should work)
        
        // Get the number of elements in the subcontainer and its length and dim
        // Get the index to the first included element at the same time
        std::vector<size_t> tempLen;
        VectorInteger       tempIndex;
        size_t size = 1;
        for ( size_t i = 0; i < length.size(); i++ ) {
            
            if ( ( i < index.size() && index.isEmpty( i ) ) || ( i >= index.size() ) ) {
                tempLen.push_back( length[i] );
                size *= length[i];
                tempIndex.push_back( 0 );           // An index we should vary
            }
            else {
                
                // An index to keep fixed to index[i]
                if ( index[i]->isType( Integer_name ) )
                    tempIndex.push_back( index.getInt( i ) );
                else
                    tempIndex.push_back( int( getIndexOfName( i, index.getString( i ) ) ) );
            }
        }
        
        // Check that we have at least some elements in the subcontainer
        if ( tempLen.size() == 0 || size == 1 )
            throw RbException( "Empty subcontainer" );
        
        // Create the subcontainer (empty at first)
        VariableContainer* temp = new VariableContainer( TypeSpec( elementType, tempLen.size() ) );
        
        // Translate index to integer index
        VectorInteger intIndex = getIntegerIndex( index );
        
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
                
                int i;
                for ( i = static_cast<int>( tempIndex.size() ) - 1; i >= 0; i-- ) {
                    
                    if ( i < static_cast<int>( intIndex.size() ) && tempIndex[i] == intIndex[i] )
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
                
                int i;
                for ( i = static_cast<int>( tempIndex.size() ) - 1; i >= 0; i-- ) {
                    
                    // Find out whether we will decrease the number of locks
                    if ( i < static_cast<int>( intIndex.size() ) && tempIndex[i] == intIndex[i] )
                        numLocks--;
                    
                    tempIndex[i]++;
                    if ( tempIndex[i] == length[i] )
                        tempIndex[i] = 0;
                    
                    // Find out whether we increased the number of locks
                    if ( i < static_cast<int>( intIndex.size() ) && tempIndex[i] == intIndex[i] )
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


/** Simple getElement function */
DAGNode* VariableContainer::getElement( const VectorNatural& index ) {

    return operator[]( index );
}


/** Are all elements constants? */
bool VariableContainer::isConstant( void ) const {

    for ( size_t i = 0; i < elements.size(); i++ ) {
        if ( !elements[i]->isConst() )
            return false;
    }

    return true;
}


/**
 * Print value for user. Since this function is unlikely to be
 * critical for performance, we simply get the value container
 * and use its printing function.
 */
void VariableContainer::printValue( std::ostream& o ) const {

    ValueContainer* temp = getConstValue();

    temp->printValue( o );

    delete temp;
}


/** Reset container length in different dimensions */
void VariableContainer::resize( const std::vector<size_t>& len ) {

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
    for ( int i = static_cast<int>( len.size() ) - 1; i >= 0; i-- ) {
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
        for ( size_t i = 0; i < length[length.size()-1]; i++ )
            tempElements[targetIndex++] = elements[sourceIndex++];

        size_t lastIndex = targetIndex;

        for ( int i = static_cast<int>( length.size() ) - 1; i >= 0; i-- ) {
            if ( sourceIndex % numValsSource[i] == 0 ) {
                targetIndex += numValsTarget[i] - ( targetIndex % numValsTarget[i] );
            }

            if ( Workspace::userWorkspace().isXOfTypeY( elementType, MemberObject_name ) ) {
                for ( size_t i = lastIndex; i < targetIndex; i++ )
                    tempElements[i] = new MemberNode( elementType );
            }
            else {
                for ( size_t i = lastIndex; i < targetIndex; i++ )
                    tempElements[i] = new ConstantNode( elementType );
            }
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


/** Set element, with type conversion if requested */
void VariableContainer::setElement( const VectorNatural& index, DAGNode* var, bool convert ) {

    if ( index.size() != length.size() ) {
    
        if ( var->numRefs() == 0 )
            delete var;
        throw RbException( "Invalid index to element of " + getTypeSpec().toString() );
    }

    // Check type
    if ( !(    Workspace::userWorkspace().isXOfTypeY       ( var->getValueType(), elementType )
            || ( convert == true && Workspace::userWorkspace().isXConvertibleToY( var->getValueType(), elementType ) ) ) ) {
        
        if ( var->numRefs() == 0 )
            delete var;
        throw RbException( "Invalid type when setting element of " + getTypeSpec().toString() );
    }

    // Check indices and resize if necessary
    bool grow = false;
    std::vector<size_t> newSize = length;
    for ( size_t i = 0; i < index.size(); i++ ) {

        if ( index[i] >= length[i] ) {
            newSize[i] = index[i] + 1;
            grow = true;
        }
    }
    if ( grow == true )
        resize( newSize );

    // Set the element, with type conversion if necessary
    if ( Workspace::userWorkspace().isXOfTypeY( var->getValueType(), elementType ) ) {

        // Delete the old element
        size_t offset = getOffset( index );
        if ( elements[offset]->numRefs() == 0 )
            delete elements[offset];
    
        elements[offset] = var;
    }
    else if ( convert == true && Workspace::userWorkspace().isXConvertibleToY( var->getValueType(), elementType ) ) {

        // Delete the old element
        size_t offset = getOffset( index );
        if ( elements[offset]->numRefs() == 0 )
            delete elements[offset];
    
        elements[offset] = new ConverterNode( var, elementType, 0 );
    }
}

