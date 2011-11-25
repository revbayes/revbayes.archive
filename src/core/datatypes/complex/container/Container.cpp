/**
 * @file
 * This file contains the declaration of Container, a complex type
 * that acts as a base class for all Containers.
 *
 * @brief Declaration of Container
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 * @extends Container
 *
 * $Id$
 */

#include "Container.h"
#include "MemberFunction.h"
#include "Natural.h"
#include "RbException.h"
#include "RbObject.h"
#include "RbPtr.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorString.h"

#include <algorithm>

/** Set type of elements */
Container::Container(const TypeSpec& elemType) : ConstantMemberObject(), elementType(elemType) {
    
}

/** Set type of elements */
Container::Container(const TypeSpec& elemType, RbPtr<const MemberRules> memberRules) : ConstantMemberObject(memberRules), elementType(elemType) {
    
}


/** Copy Constructor */
Container::Container(const Container &v) : ConstantMemberObject(v), elementType(v.elementType) {
    
}


/** Destructor. Free the memory of the elements. */
Container::~Container(void) {
    
}

/** Assignment operator; make sure we get independent elements */
Container& Container::operator=( const Container& x ) {
    
    if ( this != &x ) {
        
        if (elementType != x.elementType) {
            throw RbException("Cannot assign a vector to another vector of different type.");
        }
    }
    
    return ( *this );
}


/** Get class Container describing type of object */
const VectorString& Container::getClass(void) const { 
    
    static VectorString rbClass = VectorString(Container_name) + ConstantMemberObject::getClass();
	return rbClass;
}



/* Get method specifications */
RbPtr<const MethodTable> Container::getMethods(void) const {
    
    static RbPtr<MethodTable> methods( new MethodTable() );
    static RbPtr<ArgumentRules> sizeArgRules( new ArgumentRules() );
    static RbPtr<ArgumentRules> squareBracketArgRules( new ArgumentRules() );
    static bool          methodsSet = false;
    
    if ( methodsSet == false ) 
    {
        methods->addFunction("size",  RbPtr<RbFunction>(new MemberFunction(TypeSpec(Natural_name), sizeArgRules) ) );
        
        // add method for call "x[]" as a function
        squareBracketArgRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "index" , Natural_name ) ) );
        methods->addFunction("[]",  RbPtr<RbFunction>(new MemberFunction(TypeSpec(RbObject_name), squareBracketArgRules) ) );
        
        // necessary call for proper inheritance
        methods->setParentTable( RbPtr<const FunctionTable>( ConstantMemberObject::getMethods() ) );
        methodsSet = true;
    }
    
    return RbPtr<const MethodTable>( methods );
}


/* Map calls to member methods */
RbPtr<RbLanguageObject> Container::executeOperationSimple(const std::string& name, const RbPtr<Environment>& args) {
    
    if (name == "size") {
        
        return RbPtr<RbLanguageObject>( new Natural(size()) );
    } else if ( name == "[]") {
        // get the member with give index
        RbPtr<const Natural> index( static_cast<const Natural*>( (const RbObject*)(*args)[0]->getValue()) );

        if (size() < index->getValue()) {
            throw RbException("Index out of bounds in []");
        }
        
        // TODO: Check what happens with DAGNodeContainers
        RbPtr<RbLanguageObject> element( static_cast<RbLanguageObject*>( (RbObject*)getElement(index->getValue() - 1) ) );
        return element;
    }
    
    return ConstantMemberObject::executeOperationSimple( name, args );
}



