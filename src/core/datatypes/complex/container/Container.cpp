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

#include "ConstArgumentRule.h"
#include "Container.h"
#include "MemberFunction.h"
#include "MethodTable.h"
#include "Natural.h"
#include "RbException.h"
#include "RbObject.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "TypeSpec.h"

#include <algorithm>

/** Set type of elements */
Container::Container(const TypeSpec& elemType) : MemberObject(), elementType(elemType) {
    
}

/** Set type of elements */
Container::Container(const TypeSpec& elemType, const MemberRules& memberRules) : MemberObject(memberRules), elementType(elemType) {
    
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


/** Convert this object into another type. */
RbObject* Container::convertTo(const TypeSpec& typeSpec) const {
    
    return MemberObject::convertTo( typeSpec );
}


/** Get class name of object */
const std::string& Container::getClassName(void) { 
    
    static std::string rbClassName = "Container";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Container::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( MemberObject::getClassTypeSpec() ) );
    
	return rbClass; 
}



/* Get method specifications */
const MethodTable& Container::getMethods(void) const {
    
    static MethodTable methods = MethodTable();
    static bool          methodsSet = false;
    
    if ( methodsSet == false ) 
    {
        ArgumentRules* sizeArgRules = new ArgumentRules();
        methods.addFunction("size", new MemberFunction( Natural::getClassTypeSpec(), sizeArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &MemberObject::getMethods() );
        methodsSet = true;
    }
    
    return methods;
}


/* Map calls to member methods */
RbPtr<RbLanguageObject> Container::executeOperationSimple(const std::string& name, const std::vector<Argument>& args) {
    
    if (name == "size") {
        
        return RbPtr<RbLanguageObject>( new Natural( size() ) );
    } else if ( name == "[]") {
        // get the member with give index
        const Natural& index = static_cast<const Natural&>( args[0].getVariable()->getValue() );

        if (size() < (size_t)(index.getValue()) ) {
            throw RbException("Index out of bounds in []");
        }
        
        // \TODO: Check what happens with DAGNodeContainers
        RbLanguageObject& element = static_cast<RbLanguageObject&>( getElement(index.getValue() - 1) );
        return RbPtr<RbLanguageObject>( element.clone() );
    } 
    
    return MemberObject::executeOperationSimple( name, args );
}




/** Is convertible to type? */
bool Container::isConvertibleTo(const TypeSpec& typeSpec) const {
    
    return MemberObject::isConvertibleTo( typeSpec );
}



