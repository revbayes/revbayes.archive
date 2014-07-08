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

#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Container.h"
#include "MemberFunction.h"
#include "MethodTable.h"
#include "Natural.h"
#include "RbException.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "VectorIndexOperator.h"

#include <algorithm>

using namespace RevLanguage;

/** Set type of elements */
Container::Container(const TypeSpec& elemType) : RevObject(), elementType(elemType) {
    
}

/** Set type of elements */
Container::Container(const TypeSpec& elemType, const MemberRules& memberRules) : RevObject(memberRules), elementType(elemType) {
    
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
RevObject* Container::convertTo(const TypeSpec& typeSpec) const {
    
    return RevObject::convertTo( typeSpec );
}


/** Get class name of object */
const std::string& Container::getClassName(void) { 
    
    static std::string rbClassName = "Container";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Container::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return rbClass; 
}



/* Get method specifications */
const MethodTable& Container::getMethods(void) const {
    
    static MethodTable methods = MethodTable();
    static bool          methodsSet = false;
    
    if ( methodsSet == false ) {
        ArgumentRules* sizeArgRules = new ArgumentRules();
        methods.addFunction("size", new MemberFunction( Natural::getClassTypeSpec(), sizeArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &RevObject::getMethods() );
        methodsSet = true;
    }
    
    return methods;
}


/* Map calls to member methods */
RevPtr<Variable> Container::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if (name == "size") 
    {
        
        return new Variable( new Natural( size() ) );
    } 
    else if ( name == "[]") 
    {
        // get the member with give index
        const Natural &index = static_cast<const Natural &>( args[0].getVariable()->getRevObject() );

        if (size() < (size_t)(index.getValue()) || index.getValue() < 1 )
        {
            throw RbException("Index out of bounds in []");
        }
        
        RevPtr<Variable> element = getElement( size_t(index.getValue()) - 1);
        return element;
        
    } 
    
    return RevObject::executeMethod( name, args );
}


/** Is convertible to type? */
bool Container::isConvertibleTo(const TypeSpec& typeSpec) const {
    
    return RevObject::isConvertibleTo( typeSpec );
}


void Container::printStructure(std::ostream& o) const {

    o << "_objectType   = Container" << std::endl;
    o << "_value        = ";
    printValue( o );
    o << std::endl;
}
