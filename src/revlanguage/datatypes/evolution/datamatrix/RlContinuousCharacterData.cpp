//
//  RlContinuousCharacterData.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-27.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#include "RlContinuousCharacterData.h"



#include "ConstantNode.h"
#include "RlContinuousCharacterData.h"
#include "RbUtil.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Default constructor */
ContinuousCharacterData::ContinuousCharacterData(void) : AbstractCharacterData() {
    
}

/** Construct from bool */
ContinuousCharacterData::ContinuousCharacterData( RevBayesCore::ContinuousCharacterData *d) : AbstractCharacterData( new RevBayesCore::ContinuousCharacterData(*d) ) {
    
}


ContinuousCharacterData::ContinuousCharacterData( RevBayesCore::TypedDagNode<RevBayesCore::AbstractCharacterData> *d) : AbstractCharacterData( d )
{
    
}


ContinuousCharacterData* ContinuousCharacterData::add(const RevObject &d) const
{
    const ContinuousCharacterData* tmp = dynamic_cast<const ContinuousCharacterData*>( &d );
    if ( tmp != NULL )
    {
        return add( *tmp );
    }
    else
    {
        throw RbException("Cannot add an object of type '" + d.getType() + "' to a ContinuousCharacterData object.");
    }
}



ContinuousCharacterData* ContinuousCharacterData::add(const ContinuousCharacterData &d) const
{
    ContinuousCharacterData* cloneObj = clone();
    
    cloneObj->getDagNode()->getValue().add( d.getValue() );
    // return the copy
    return cloneObj;
}


/** Clone object */
ContinuousCharacterData* ContinuousCharacterData::clone(void) const {
    
	return new ContinuousCharacterData(*this);
}


/** Convert to type. The caller manages the returned object. */
RevObject* ContinuousCharacterData::convertTo(const TypeSpec& type) const {
    
    return RevObject::convertTo(type);
}


/** Get Rev type of object */
const std::string& ContinuousCharacterData::getClassType(void) {
    
    static std::string revType = "ContinuousCharacterData";
    
	return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& ContinuousCharacterData::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( AbstractCharacterData::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/**
 * Get member methods. We construct the appropriate static member
 * function table here.
 */
const MethodTable& ContinuousCharacterData::getMethods( void ) const
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
const TypeSpec& ContinuousCharacterData::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}



/** Is convertible to type? */
bool ContinuousCharacterData::isConvertibleTo(const TypeSpec& type, bool once) const {
    
    return RevObject::isConvertibleTo(type, once);
}


const RevBayesCore::ContinuousCharacterData& RevLanguage::ContinuousCharacterData::getValue( void ) const
{
    
    return static_cast<const RevBayesCore::ContinuousCharacterData&>( this->dagNode->getValue() );
}


