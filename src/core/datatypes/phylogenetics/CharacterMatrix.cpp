/**
 * @file
 * This file contains the declaration of CharacterMatrix, which is
 * class that holds a character matrix in RevBayes.
 *
 * @brief Implementation of CharacterMatrix
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-29 23:23:09 +0100 (Tis, 29 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 * @interface Mcmc
 * @package distributions
 *
 * $Id: Mcmc.h 211 2009-12-29 22:23:09Z ronquist $
 */

#include "Character.h"
#include "CharacterMatrix.h"
#include "ConstantNode.h"
#include "DnaState.h"
#include "MemberFunction.h"
#include "Natural.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbString.h"
#include "Sequence.h"
#include "StochasticNode.h"
#include "ValueRule.h"
#include "VariableNode.h"
#include "Vector.h"
#include "VectorNatural.h"
#include "VectorString.h"
#include "Workspace.h"

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>


/** Constructor requires character type; passes member rules to base class */
CharacterMatrix::CharacterMatrix( const std::string& charType ) : Matrix( charType, getMemberRules() ) {
    characterType = charType;
}


/** Copy constructor */
CharacterMatrix::CharacterMatrix( const CharacterMatrix& x ) : Matrix( x ) {

    characterType     = x.characterType;
    deletedTaxa       = x.deletedTaxa;
    deletedCharacters = x.deletedCharacters;
    fileName          = x.fileName;

    sequenceNames     = x.sequenceNames;
}


/** Destructor */
CharacterMatrix::~CharacterMatrix( void ) {

}


/** Assignment operator */
CharacterMatrix& CharacterMatrix::operator=( const CharacterMatrix& x ) {

    if ( this != &x ) {

        if ( characterType != x.characterType )
            throw RbException( "Invalid assignment of character matrices: sequence data types differ" );

        MemberObject::operator=( x );

        deletedTaxa       = x.deletedTaxa;
        deletedCharacters = x.deletedCharacters;
        fileName          = x.fileName;
    
    }

    return (*this);
}


/** Index (const) operator */
const Sequence& CharacterMatrix::operator[]( const size_t i ) const {

    return getSequence( i );
}


/** Add a sequence to the character matrix. For now, we require same data type and same length. */
void CharacterMatrix::addSequence( const std::string tName, Sequence* obs ) {

    // set the number of character per sequence
    if (members.size() == 0) {
        cols = obs->size();
    }
    else if ( obs->size() != getNumCharacters() ) {

        std::ostringstream msg;
        msg << "Invalid attempt to add sequence of length " << obs->size() << " to aligned character matrix of length " << getNumCharacters();
        throw RbException( msg );
    }

//    VariableSlot* slot = new VariableSlot( sequenceTypeRule );
    members.addVariable(tName, TypeSpec( characterType));
    members[ tName ].setVariable( new Variable( new ConstantNode(obs) ) );
    
    // add the sequence name to the list
    sequenceNames.push_back(tName);
}


/** clear the oblect */
void CharacterMatrix::clear( void ) {
    
    throw RbException("Clear of CharacterMatrix not implemented");
}


/** Clone object */
CharacterMatrix* CharacterMatrix::clone( void ) const {

    return new CharacterMatrix( *this );
}


/** Exclude a character */
void CharacterMatrix::excludeCharacter(size_t i) {

    if (i >= getNumCharacters() )
        throw RbException( "Only " + RbString(int(getNumCharacters())) + " characters in matrix" );

    deletedCharacters.insert( i );
}


/** Exclude a taxon */
void CharacterMatrix::excludeTaxon(size_t i) {

    if (i >= members.size())
        throw RbException( "Only " + RbString(int(members.size())) + " taxa in matrix" );

    deletedTaxa.insert( i );
}


/** Exclude a taxon */
void CharacterMatrix::excludeTaxon(std::string& s) {

//    size_t i = members.getIndex( s );
//
//    deletedTaxa.insert( i );
}


/** Map calls to member methods */
RbLanguageObject* CharacterMatrix::executeOperation(const std::string& name, Environment& args) {

    if (name == "names") 
    {
        return sequenceNames.clone();
    }
    else if (name == "ntaxa") 
        {
        int n = (int)getNumTaxa();
        return ( new Natural(n) );
        }
    else if (name == "nchar")
        {
        int n = (int)getNumCharacters();
        return ( new Natural(n) );
        }
    else if (name == "chartype")
        {
        std::string ct = getDataType();
        return ( new RbString(ct) );
        }
    else if (name == "nexcludedtaxa")
        {
        int n = (int)deletedTaxa.size();
        return ( new Natural(n) );
        }
    else if (name == "nexcludedchars")
        {
        int n = (int)deletedCharacters.size();
        return ( new Natural(n) );
        }
    else if (name == "nincludedtaxa")
        {
        int n = (int)(getNumTaxa() - deletedTaxa.size());
        return ( new Natural(n) );
        }
    else if (name == "nincludedchars")
        {
        int n = (int)(getNumCharacters() - deletedCharacters.size());
        return ( new Natural(n) );
        }
    else if (name == "excludedtaxa")
        {
        std::vector<std::string> et;
        for (std::set<size_t>::iterator it = deletedTaxa.begin(); it != deletedTaxa.end(); it++)
            {
            std::string tn = getTaxonWithIndex(*it);
            et.push_back( tn );
            }
        return ( new VectorString(et) );
        }
    else if (name == "excludedchars")
        {
        std::vector<int> ec;
        for (std::set<size_t>::iterator it = deletedCharacters.begin(); it != deletedCharacters.end(); it++)
            ec.push_back( (int)(*it) );
        return ( new VectorNatural(ec) );
        }
    else if (name == "includedtaxa")
        {
        std::vector<std::string> it;
        for (size_t i=0; i<getNumTaxa(); i++)
            {
            if ( isTaxonExcluded(i) == false )
                it.push_back( getTaxonWithIndex(i) );
            }
        return ( new VectorString(it) );
        }
    else if (name == "includedchars")
        {
        std::vector<int> ic;
        for (size_t i=0; i<getNumCharacters(); i++)
            {
            if ( isCharacterExcluded(i) == false )
                ic.push_back( (int)(i+1) );
            }
        return ( new VectorNatural(ic) );
        }
    else if (name == "nconstantpatterns")
        {
        int n = (int)numConstantPatterns();
        return ( new Natural(n) );
        }
    else if (name == "ncharswithambiguity")
        {
        int n = (int)numMissAmbig();
        return ( new Natural(n) );
        }
    else if (name == "excludechar")
        {
        const RbObject* argument = args[1].getValue();

        if ( argument->isType( Natural_name ) ) {
            
            int n = static_cast<const Natural*>( argument )->getValue();
            deletedCharacters.insert( n );
        }
        else if ( argument->isType( VectorNatural_name ) ) {
        
            std::vector<unsigned int> x = static_cast<const VectorNatural*>( argument )->getValue();
            for ( size_t i=0; i<x.size(); i++ )
                deletedCharacters.insert( x[i] );
        }
        return NULL;
        }

    return ConstantMemberObject::executeOperation( name, args );
}


/** Return a pointer to a character element in the character matrix */
const Character& CharacterMatrix::getCharacter( size_t tn, size_t cn ) const {

    if ( cn >= getNumCharacters() )
        throw RbException( "Character index out of range" );

    return static_cast<const Character&>(getSequence( tn )[cn]);
}


/** Get class vector describing type of object */
const VectorString& CharacterMatrix::getClass(void) const {

    static VectorString rbClass = VectorString(CharacterMatrix_name) + Matrix::getClass();
    return rbClass;
}


Vector* CharacterMatrix::getElement(size_t index) {
    return dynamic_cast<Vector*>(members[index].getDagNodePtr()->getValuePtr());
}

Character* CharacterMatrix::getElement(size_t row, size_t col) {
    Vector *sequence = dynamic_cast<Vector*>(members[row].getDagNodePtr()->getValuePtr());
    return static_cast<Character*>(sequence->getElement(col));
}


/** Get member rules */
const MemberRules& CharacterMatrix::getMemberRules(void) const {

    static MemberRules memberRules;
    return memberRules;
}


/** Get methods */
const MethodTable& CharacterMatrix::getMethods(void) const {

    static MethodTable   methods;
    static ArgumentRules ncharArgRules;
    static ArgumentRules namesArgRules;
    static ArgumentRules ntaxaArgRules;
    static ArgumentRules chartypeArgRules;    
    static ArgumentRules nexcludedtaxaArgRules;    
    static ArgumentRules nexcludedcharsArgRules;    
    static ArgumentRules nincludedtaxaArgRules;    
    static ArgumentRules nincludedcharsArgRules;    
    static ArgumentRules excludedtaxaArgRules;    
    static ArgumentRules excludedcharsArgRules;    
    static ArgumentRules includedtaxaArgRules;    
    static ArgumentRules includedcharsArgRules;    
    static ArgumentRules nconstantpatternsArgRules;    
    static ArgumentRules ncharswithambiguityArgRules;
    static ArgumentRules excludecharArgRules;
    static ArgumentRules excludecharArgRules2;
    static bool          methodsSet = false;

    if ( methodsSet == false ) 
        {

        excludecharArgRules.push_back(         new ValueRule(     "", Natural_name ) );
        excludecharArgRules2.push_back(        new ValueRule(     "", VectorNatural_name ) );
            
        methods.addFunction("names",               new MemberFunction(VectorString_name,  namesArgRules));
        methods.addFunction("nchar",               new MemberFunction(Natural_name,       ncharArgRules));
        methods.addFunction("ntaxa",               new MemberFunction(Natural_name,       ntaxaArgRules));
        methods.addFunction("chartype",            new MemberFunction(RbString_name,      chartypeArgRules));
        methods.addFunction("nexcludedtaxa",       new MemberFunction(Natural_name,       nexcludedtaxaArgRules));
        methods.addFunction("nexcludedchars",      new MemberFunction(Natural_name,       nexcludedcharsArgRules));
        methods.addFunction("nincludedtaxa",       new MemberFunction(Natural_name,       nincludedtaxaArgRules));
        methods.addFunction("nincludedchars",      new MemberFunction(Natural_name,       nincludedcharsArgRules));
        methods.addFunction("excludedtaxa",        new MemberFunction(VectorNatural_name, excludedtaxaArgRules));
        methods.addFunction("excludedchars",       new MemberFunction(VectorNatural_name, excludedcharsArgRules));
        methods.addFunction("includedtaxa",        new MemberFunction(VectorNatural_name, includedtaxaArgRules));
        methods.addFunction("includedchars",       new MemberFunction(VectorNatural_name, includedcharsArgRules));
        methods.addFunction("nconstantpatterns",   new MemberFunction(Natural_name,       nconstantpatternsArgRules));
        methods.addFunction("ncharswithambiguity", new MemberFunction(Natural_name,       ncharswithambiguityArgRules));
        methods.addFunction("excludechar",         new MemberFunction(RbVoid_name,        excludecharArgRules));
        methods.addFunction("excludechar",         new MemberFunction(RbVoid_name,        excludecharArgRules2));
        
        // necessary call for proper inheritance
        methods.setParentTable( const_cast<MethodTable*>( &MemberObject::getMethods() ) );
        methodsSet = true;
        }

    return methods;
}


/** Return the number of characters in each vector of taxon observations */
size_t CharacterMatrix::getNumCharacters(void) const {

    return cols;
}


/** Get the number of states for the characters in this matrix. We
    assume that all of the characters in the matrix are of the same
    type and have the same number of potential states. */
size_t CharacterMatrix::getNumStates(void) const {

    // Get the first character in the matrix
    if ( members.size() == 0 )
        return 0;

    const Sequence& sequence = getSequence( 0 );
    if ( sequence.size() == 0 )
        return 0;

    return static_cast<const Character&>(sequence[0]).getNumberOfStates();
}


/** Get sequence with index tn */
const Sequence& CharacterMatrix::getSequence( size_t tn ) const {

    if ( tn >= getNumTaxa() )
        throw RbException( "Taxon index out of range" );

    const Sequence* sequence = static_cast<const Sequence*>( members[members.getName(tn)].getValue() );

    return *sequence;
}


/** Get taxon with index idx */
std::string CharacterMatrix::getTaxonWithIndex( size_t idx ) const {

    return members.getName( idx );
}


/** Return the index of the element ( the index of the taxon with name elemName ) */
size_t CharacterMatrix::indexOfTaxonWithName( std::string& s ) const {
    
    // search through all names
    for (size_t i=0; i<members.size(); i++) {
        if (s == members.getName( i )) {
            return i;
        }
    }

    return -1;
}


/** Is this character pattern constant? */
bool CharacterMatrix::isCharacterConstant(size_t idx) const {

    const Character* f = NULL;
    for ( size_t i=0; i<getNumTaxa(); i++ ) {

        if ( isTaxonExcluded(i) == false ) {
            
            if ( f == NULL )
                f = &( getCharacter( i, idx ) );
            else {
                const Character* s = &( getCharacter( i , idx ) );
                if ( (*f) != (*s) )
                    return false;
            }
        }
    }

    return true;
}


/** Is the character excluded */
bool CharacterMatrix::isCharacterExcluded(size_t i) const {

	std::set<size_t>::const_iterator it = deletedCharacters.find( i );
	if ( it != deletedCharacters.end() )
		return true;
    return false;
}


/** Does the character have missing or ambiguous characters */
bool CharacterMatrix::isCharacterMissingOrAmbiguous(size_t idx) const {

    for ( size_t i=0; i<getNumTaxa(); i++ )
        {
        if ( isTaxonExcluded(i) == false )
            {
            const Character& c = getCharacter( i, idx );
            if ( c.isMissingOrAmbiguous() == true )
                return true;
            }
        }
    return false;
}


/** Is the taxon excluded */
bool CharacterMatrix::isTaxonExcluded(size_t i) const {

	std::set<size_t>::const_iterator it = deletedTaxa.find( i );
	if ( it != deletedTaxa.end() )
		return true;
    return false;
}


/** Is the taxon excluded */
bool CharacterMatrix::isTaxonExcluded(std::string& s) const {

    size_t i = indexOfTaxonWithName(s);
	std::set<size_t>::const_iterator it = deletedTaxa.find( i );
	if ( it != deletedTaxa.end() )
		return true;
    return false;
}


/** Make copy of site column with index cn */
Vector* CharacterMatrix::makeSiteColumn( size_t cn ) const {

    if ( cn >= getNumCharacters() )
        throw RbException( "Site index out of range" );

    if ( getNumTaxa() == 0 )
        throw RbException( "Character matrix is empty" );

    Vector* temp = static_cast<const Vector*>( members[0].getValue() )->clone();
    temp->clear();
    for ( size_t i=0; i<getNumTaxa(); i++ )
        temp->push_back( getCharacter( i, cn ).clone() );

    return temp;
}


/** Calculates and returns the number of constant characters */
size_t CharacterMatrix::numConstantPatterns(void) const {

    size_t nc = 0;
    for (size_t i=0; i<getNumCharacters(); i++)
        {
        if ( isCharacterExcluded(i) == false && isCharacterConstant(i) == true )
            nc++;
        }
    return nc;
}


/** Returns the number of characters with missing or ambiguous data */
size_t CharacterMatrix::numMissAmbig(void) const {

    size_t nma = 0;
    for (size_t i=0; i<getNumCharacters(); i++)
        {
        if ( isCharacterExcluded(i) == false && isCharacterMissingOrAmbiguous(i) == true )
            nma++;
        }
    return nma;
}


/** Print value for user */
void CharacterMatrix::printValue(std::ostream& o) const {

    o << "Origination:          " << fileName << std::endl;
    o << "Data type:            " << getDataType() << std::endl;
    o << "Number of taxa:       " << getNumTaxa() << std::endl;
    o << "Number of characters: " << getNumCharacters() << std::endl;
}

/** Overloaded container resize method */
void CharacterMatrix::resize( size_t len ) {
    
    throw RbException("Not implemented method CharacterMatrix::resize()");
}

/** Overloaded container resize method */
void CharacterMatrix::resize( size_t rows, size_t cols ) {
    
    throw RbException("Not implemented method CharacterMatrix::resize()");
}



/** Restore a character */
void CharacterMatrix::restoreCharacter(size_t i) {

    if (i >= getNumCharacters() )
        throw RbException( "Character index out of range" );

    deletedCharacters.erase( i );
}


/** Restore a taxon */
void CharacterMatrix::restoreTaxon(size_t i) {

    if ( i >= getNumTaxa() )
        return;

    deletedTaxa.erase( i );
}


/** Restore a taxon */
void CharacterMatrix::restoreTaxon(std::string& s) {

    size_t i = indexOfTaxonWithName( s );

    deletedTaxa.erase( i );
}


/** Complete info */
std::string CharacterMatrix::richInfo(void) const {

	std::ostringstream o;
    printValue( o );
    return o.str();
}

/** Overloaded container setElement method */
void CharacterMatrix::setElement( size_t index, RbLanguageObject* var ) {
    
    throw RbException("Not implemented method CharacterMatrix::setElement()");
}

/** Overloaded container setElement method */
void CharacterMatrix::setElement( size_t row, size_t col, RbLanguageObject* var ) {
    
    throw RbException("Not implemented method CharacterMatrix::setElement()");
}

/** the number of elements in this matrix */
size_t CharacterMatrix::size() const {
    return getNumTaxa() * getNumCharacters();
}

/** transpose the matrix */
void CharacterMatrix::transpose() {
    throw RbException("Transpose of CharacterMatrix not supported.");
}




