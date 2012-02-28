/**
 * @file
 * This file contains the implementation of Sequence, an
 * abstract base class for different types of character vectors.
 *
 * @brief Implementation of VectorCharacterObservations
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 * @extends Vector
 *
 * $Id$
 */


#include "Character.h"
#include "Ellipsis.h"
#include "MemberFunction.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbString.h"
#include "TaxonData.h"
#include "ValueRule.h"


/** Constructor with element type, used to properly construct vectors */
TaxonData::TaxonData(const std::string& elemType, const std::string tname) : ConstantMemberObject(), sequence(elemType), taxonName(tname) {
    
}


/** Subscript operator */
Character& TaxonData::operator[](size_t i) {
    
    if (i >= sequence.size())
        throw RbException("Index out of bounds");
    return static_cast<Character&>( sequence[i] );
}


/** Subscript const operator */
const Character& TaxonData::operator[](size_t i) const {
    
    if (i >= sequence.size())
        throw RbException("Index out of bounds");
    return static_cast<const Character&>( sequence[i] );
}

/** Push back a new character */
void TaxonData::addCharacter( Character* newChar ) {
    
    if ( newChar == NULL || !newChar->isTypeSpec( sequence.getElementType() ) )
        throw RbException( "Inappropriate character type" );
    
    sequence.push_back( newChar );
}


TaxonData* TaxonData::clone(void) const {

    return new TaxonData(*this);
}


/* Map calls to member methods */
const RbLanguageObject& TaxonData::executeOperationSimple(const std::string& name, const std::vector<Argument>& args) {
    
    if (name == "size") {
        
        // we set our value
        returnValueSize.setValue( getNumberOfCharacters() );
        
        return returnValueSize;
    } else if ( name == "[]") {
        // get the member with give index
        const Natural& index = static_cast<const Natural&>( args[0].getVariable().getValue() );
        
        if ( getNumberOfCharacters() < (size_t)(index.getValue()) ) {
            throw RbException("Index out of bounds in [] of TaxonData.");
        }
        
        // we have to internally reduce the index by 1
        Character& element = operator[](index.getValue() - 1);
        return element;
    } 
    
    return ConstantMemberObject::executeOperationSimple( name, args );
}


/** Get class name of object */
const std::string& TaxonData::getClassName(void) { 
    
    static std::string rbClassName = "Taxon data";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& TaxonData::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( ConstantMemberObject::getClassTypeSpec() ) );
    
	return rbClass; 
}


const MemberRules& TaxonData::getMemberRules(void) const {
    
    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;
    
    if (!rulesSet) {
        
        memberRules.push_back( new ValueRule( "name", RbString::getClassTypeSpec() ) );
        memberRules.push_back( new ValueRule( "x"   , Character::getClassTypeSpec() ) );
        memberRules.push_back( new Ellipsis( Character::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}


/* Get method specifications */
const MethodTable& TaxonData::getMethods(void) const {
    
    static MethodTable methods = MethodTable();
    static bool          methodsSet = false;
    
    if ( methodsSet == false ) 
    {
        ArgumentRules* sizeArgRules = new ArgumentRules();
        methods.addFunction("size", new MemberFunction( Natural::getClassTypeSpec(), sizeArgRules) );
        
        // add method for call "x[]" as a function
        ArgumentRules* squareBracketArgRules = new ArgumentRules();
        squareBracketArgRules->push_back( new ValueRule( "index" , Natural::getClassTypeSpec() ) );
        methods.addFunction("[]",  new MemberFunction( RbObject::getClassTypeSpec(), squareBracketArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &ConstantMemberObject::getMethods() );
        methodsSet = true;
    }
    
    return methods;
}

/** Get type spec */
const TypeSpec& TaxonData::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


const Character& TaxonData::getCharacter(size_t index) const {

    return operator[](index);
}


size_t TaxonData::getNumberOfCharacters(void) const {

    return sequence.size();
}


const std::string& TaxonData::getTaxonName(void) const {

    return taxonName;
}


void TaxonData::printValue(std::ostream &o) const {

    o << taxonName << ":" << std::endl;
    sequence.printValue(o);
}


/** Set a member variable */
void TaxonData::setMemberVariable(const std::string& name, Variable* var) {
    
    if ( name == "name" ) {
        taxonName = static_cast<RbString&>( var->getValue() ).getValue();
    }
    else if (name == "x" || name == "" ) { // the ellipsis variables
        Character* element = static_cast<Character*>( var->getValue().clone() );
        sequence.push_back( element );
    }
    else {
        ConstantMemberObject::setMemberVariable(name, var);
    }
}



void TaxonData::setTaxonName(std::string tn) {

    taxonName = tn;
}

