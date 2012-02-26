/**
 * @file
 * This file contains the declaration of AbstractVector, a complex type
 * that acts as a base class for all AbstractVectors.
 *
 * @brief Declaration of AbstractVector
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

#include "AbstractVector.h"
#include "Container.h"
#include "MemberFunction.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "Vector.h"
#include "VectorString.h"
#include <algorithm>

/** Set type of elements */
AbstractVector::AbstractVector(const TypeSpec& elemType) : Container(elemType) {
    
}


/** Set type of elements */
AbstractVector::AbstractVector(const TypeSpec& elemType, const MemberRules& memberRules) : Container(elemType,memberRules) {
    
}


/** Copy Constructor */
AbstractVector::AbstractVector(const AbstractVector &v) : Container(v) {
    
}


/** Destructor. Free the memory of the elements. */
AbstractVector::~AbstractVector(void) {
    
}

/** Assignment operator; make sure we get independent elements */
AbstractVector& AbstractVector::operator=( const AbstractVector& x ) {
    
    if ( this != &x ) {
        
        if (elementType != x.elementType) {
            throw RbException("Cannot assign a vector to another vector of different type.");
        }
    }
    
    return ( *this );
}


RbObject* AbstractVector::convertTo(const TypeSpec &type) const {
    
    if ( type.getBaseType() == Vector::getClassName() ) {
        Vector* newVector = new Vector(type.getElementType());
        for (size_t i = 0; i < size(); i++) {
            newVector->push_back(getElement(i).clone());
        }
        return newVector;
    }
    
    return Container::convertTo(type);
}


/** Get class name of object */
const std::string& AbstractVector::getClassName(void) { 
    
    static std::string rbClassName = "Abstract Vector";
    
	return rbClassName; 
}


/* Get method specifications */
const MethodTable& AbstractVector::getMethods(void) const {
    
    static MethodTable methods = MethodTable();
    static bool          methodsSet = false;
    
    if ( methodsSet == false ) 
    {
        
        // sort function
        ArgumentRules* sortArgRules = new ArgumentRules();
        methods.addFunction("sort", new MemberFunction( RbVoid_name, sortArgRules) );
        
        // unique function
        ArgumentRules* uniqueArgRules = new ArgumentRules();
        methods.addFunction("unique", new MemberFunction( RbVoid_name, uniqueArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &Container::getMethods() );
        methodsSet = true;
    }
    
    return methods;
}

/** Get class type spec describing type of object */
const TypeSpec& AbstractVector::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Container::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Map calls to member methods */
const RbLanguageObject& AbstractVector::executeOperationSimple(const std::string& name, const std::vector<Argument>& args) {
    
    if (name == "sort") {
        
        // we set our value
        sort();
        
        return RbNullObject::getInstance();
    } else if (name == "unique") {
        
        // we set our value
        unique();
        
        return RbNullObject::getInstance();
    }
    
    return Container::executeOperationSimple( name, args );
}


/** Print value for user */
void AbstractVector::printValue( std::ostream& o ) const {
    
    o << "[ ";
    for ( size_t i = 0; i<size(); i++) {
        if ( i != 0 )
            o << ", ";
        (dynamic_cast<const RbLanguageObject&>( getElement(i) )).printValue(o);
    }
    o <<  " ]";
    
}


void AbstractVector::setElement(const size_t index, RbObject* x) {
    RbLanguageObject *elem = dynamic_cast<RbLanguageObject*>( x );
    if (elem != NULL)
        setElement(index, elem );
}




