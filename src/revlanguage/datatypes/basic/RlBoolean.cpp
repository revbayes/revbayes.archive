/**
 * @file
 * This file contains the implementation of RlBoolean, which is
 * a RevBayes wrapper around a regular bool.
 *
 * @brief Implementation of RlBoolean
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 * @extends RbObject
 *
 * $Id$
 */


#include "ConstantNode.h"
#include "RlBoolean.h"
#include "Real.h"
#include "Integer.h"
#include "RbUtil.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Default constructor */
RlBoolean::RlBoolean(void) : ModelObject<bool>() {

    setGuiVariableName("Boolean");
    setGuiLatexSymbol("B");
}

/** Construct from bool */
RlBoolean::RlBoolean(RevBayesCore::TypedDagNode<bool> *v) : ModelObject<bool>( v ) {
    
    setGuiVariableName("Boolean");
    setGuiLatexSymbol("B");
}

/** Construct from bool */
RlBoolean::RlBoolean(bool v) : ModelObject<bool>( new bool(v) ) {

    setGuiVariableName("Boolean");
    setGuiLatexSymbol("B");
}


/** Clone object */
RlBoolean* RlBoolean::clone(void) const {

	return new RlBoolean(*this);
}


/** Convert to type. The caller manages the returned object. */
RevObject* RlBoolean::convertTo(const TypeSpec& type) const {

    if (type == Integer::getClassTypeSpec()) {
        if (dagNode->getValue())
            return new Integer(1);
        else 
            return new Integer(0);
    }
    else if (type == Real::getClassTypeSpec()) {
        if (dagNode->getValue())
            return new Real(1.0);
        else 
            return new Real(0.0);
    }

    return RevObject::convertTo(type);
}


/** Get Rev type of object */
const std::string& RlBoolean::getClassType(void) { 
    
    static std::string revType = "Bool";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& RlBoolean::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/**
 * Get member methods. We construct the appropriate static member
 * function table here.
 */
const MethodTable& RlBoolean::getMethods( void ) const
{
    static MethodTable  myMethods   = MethodTable();
    static bool         methodsSet  = false;
    
    if ( !methodsSet )
    {
        myMethods = makeMethods();
        methodsSet = true;
    }
    
    return myMethods;
}


/** Get type spec */
const TypeSpec& RlBoolean::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Is convertible to type? */
bool RlBoolean::isConvertibleTo(const TypeSpec& type, bool once) const {

    if ( type == Integer::getClassTypeSpec() )
        return true;
    else if ( type == Real::getClassTypeSpec() )
        return true;

    return RevObject::isConvertibleTo(type, once);
}


/** Print value for user */
void RlBoolean::printValue(std::ostream &o) const {

    o << (dagNode->getValue() ? "true" : "false");
}

