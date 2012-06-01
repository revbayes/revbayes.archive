/**
 * @file
 * This file contains the implementation of Sequence, an
 * abstract base class for different types of character vectors.
 *
 * @brief Implementation of VectorCharacterObservations
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 * @extends Vector
 *
 * $Id$
 */


#include "ArgumentRule.h"
#include "CharacterState.h"
#include "Ellipsis.h"
#include "MethodTable.h"
#include "Natural.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbString.h"
#include "RlCharacterState.h"
#include "RlTaxonData.h"
#include "SimpleMemberFunction.h"


/** Constructor with element type, used to properly construct vectors */
RlTaxonData::RlTaxonData(const std::string &tname) : MemberObject(), taxon( tname ) {
    
}


/** Constructor with element type, used to properly construct vectors */
RlTaxonData::RlTaxonData(const TaxonData &td) : MemberObject(), taxon( td ) {
    
}


RlTaxonData* RlTaxonData::clone(void) const {
    
    return new RlTaxonData(*this);
}


/* Map calls to member methods */
RbPtr<RbLanguageObject> RlTaxonData::executeSimpleMethod(std::string const &name, const std::vector<const RbObject *> &args) {
    
    if (name == "size") {
        
        return RbPtr<RbLanguageObject>( new Natural( taxon.getNumberOfCharacters() ) );
    } else if ( name == "[]") {
        // get the member with give index
        const Natural& index = static_cast<const Natural&>( *args[0] );
        
        if ( taxon.getNumberOfCharacters() < (size_t)(index.getValue()) ) {
            throw RbException("Index out of bounds in [] of RlTaxonData.");
        }
        
        // we have to internally reduce the index by 1
//        CharacterState& element = taxon[index.getValue() - 1];
//        return RbPtr<RbLanguageObject>( RlCharacterState( element ) );
        throw RbException("Missing implementation in RlTaxonData::executeSimpleMethod()");
    } 
    
    return MemberObject::executeSimpleMethod( name, args );
}


/** Get class name of object */
const std::string& RlTaxonData::getClassName(void) { 
    
    static std::string rbClassName = "Taxon data";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& RlTaxonData::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( MemberObject::getClassTypeSpec() ) );
    
	return rbClass; 
}


const MemberRules& RlTaxonData::getMemberRules(void) const {
    
    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;
    
    if (!rulesSet) {
        
        memberRules.push_back( new ArgumentRule( "name", true, RbString::getClassTypeSpec() ) );
        memberRules.push_back( new ArgumentRule( "x"   , true, RlCharacterState::getClassTypeSpec() ) );
        memberRules.push_back( new Ellipsis( RlCharacterState::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}


/* Get method specifications */
const MethodTable& RlTaxonData::getMethods(void) const {
    
    static MethodTable  methods = MethodTable();
    static bool         methodsSet = false;
    
    if ( methodsSet == false ) 
    {
        ArgumentRules* sizeArgRules = new ArgumentRules();
        methods.addFunction("size", new SimpleMemberFunction( Natural::getClassTypeSpec(), sizeArgRules) );
        
        // add method for call "x[]" as a function
        ArgumentRules* squareBracketArgRules = new ArgumentRules();
        squareBracketArgRules->push_back( new ArgumentRule( "index", true, Natural::getClassTypeSpec() ) );
        methods.addFunction("[]",  new SimpleMemberFunction( RbObject::getClassTypeSpec(), squareBracketArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &MemberObject::getMethods() );
        methodsSet = true;
    }
    
    return methods;
}

/** Get type spec */
const TypeSpec& RlTaxonData::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


const TaxonData& RlTaxonData::getValue( void ) const {
    return taxon;
}


void RlTaxonData::printValue(std::ostream &o) const {
    
    o << taxon.getTaxonName() << ": ";
    for (size_t i = 0; i < taxon.getNumberOfCharacters(); i++) {
        o << taxon[i].getStringValue();
    }
}


/** Set a member variable */
void RlTaxonData::setSimpleMemberValue(const std::string& name, const RbPtr<const RbLanguageObject> &var) {
    
    if ( name == "name" ) {
        std::string n = static_cast<const RbString*>( (const RbLanguageObject *) var )->getValue();
        taxon.setTaxonName( n );
    }
    else if (name == "x" || name == "" ) { // the ellipsis variables
        const RlCharacterState* element = static_cast<const RlCharacterState*>( (const RbLanguageObject *) var );
        taxon.addCharacter( element->getValue().clone() );
    }
    else {
        MemberObject::setSimpleMemberValue(name, var);
    }
}

