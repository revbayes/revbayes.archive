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
#include "RbUtil.h"
#include "TypeSpec.h"
#include "VectorString.h"

#include <algorithm>

/** Set type of elements */
Container::Container(const TypeSpec& elemType) : ConstantMemberObject(), elementType(elemType) {
    
}

/** Set type of elements */
Container::Container(const TypeSpec& elemType, const MemberRules &memberRules) : ConstantMemberObject(memberRules), elementType(elemType) {
    
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
const MethodTable& Container::getMethods(void) const {
    
    static MethodTable   methods;
    static ArgumentRules sizeArgRules;
    static bool          methodsSet = false;
    
    if ( methodsSet == false ) 
    {
        methods.addFunction("size",  new MemberFunction(TypeSpec(Natural_name), sizeArgRules)  );
        
        // necessary call for proper inheritance
        methods.setParentTable( const_cast<MethodTable*>( &ConstantMemberObject::getMethods() ) );
        methodsSet = true;
    }
    
    return methods;
}


/* Map calls to member methods */
RbLanguageObject* Container::executeOperation(const std::string& name, Environment& args) {
    
    if (name == "size") {
        
        return ( new Natural(size()) );
    }
    
    return ConstantMemberObject::executeOperation( name, args );
}



