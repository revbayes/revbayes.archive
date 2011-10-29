/**
 * @file
 * This file contains the declaration of Alignment, which is
 * class that holds a character matrix in RevBayes.
 *
 * @brief Implementation of Alignment
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 * @interface Mcmc
 * @package datatypes
 *
 * $Id$
 */

#include "AbstractVector.h"
#include "Alignment.h"
#include "Character.h"
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
Alignment::Alignment( const std::string& charType ) : Matrix( charType, getMemberRules() ), typeSpec(Alignment_name, RbPtr<TypeSpec>( new TypeSpec(charType) ) ) {
    characterType = charType;
}


/** Copy constructor */
Alignment::Alignment( const Alignment& x ) : Matrix( x ), typeSpec(Alignment_name, RbPtr<TypeSpec>( new TypeSpec(characterType) ) ) {

    characterType     = x.characterType;
    deletedTaxa       = x.deletedTaxa;
    deletedCharacters = x.deletedCharacters;
    fileName          = x.fileName;

    sequenceNames     = x.sequenceNames;
}


/** Destructor */
Alignment::~Alignment( void ) {

}


/** Assignment operator */
Alignment& Alignment::operator=( const Alignment& x ) {

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
const RbPtr<Sequence> Alignment::operator[]( const size_t i ) const {

    return getSequence( i );
}


/** Add a sequence to the character matrix. For now, we require same data type and same length. */
void Alignment::addSequence( RbPtr<Sequence> obs ) {

    // set the number of character per sequence
    if ( size() > 0 && obs->size() != getNumberOfCharacters() ) {

        std::ostringstream msg;
        msg << "Invalid attempt to add sequence of length " << obs->size() << " to aligned character matrix of length " << getNumberOfCharacters();
        throw RbException( msg );
    }

    push_back(RbPtr<RbObject>( obs.get() ));
    
    // add the sequence name to the list
    sequenceNames.push_back(obs->getTaxonName());
    
    // add the sequence also as a member so that we can access it by name
    RbPtr<Variable> var( new Variable( RbPtr<DAGNode>( new ConstantNode(RbPtr<RbLanguageObject>( obs.get() ) ) ) ) );
    members->addVariable(obs->getTaxonName(), var);
}


/** clear the oblect */
void Alignment::clear( void ) {
    
    sequenceNames.clear();
    
    Matrix::clear();
}


/** Clone object */
Alignment* Alignment::clone( void ) const {

    return new Alignment( *this );
}


/** Exclude a character */
void Alignment::excludeCharacter(size_t i) {

    if (i >= getNumberOfCharacters() )
        throw RbException( "Only " + RbString(int(getNumberOfCharacters())) + " characters in matrix" );

    deletedCharacters.insert( i );
}


/** Exclude a taxon */
void Alignment::excludeTaxon(size_t i) {

    if (i >= members->size())
        throw RbException( "Only " + RbString(int(members->size())) + " taxa in matrix" );

    deletedTaxa.insert( i );
}


/** Exclude a taxon */
void Alignment::excludeTaxon(std::string& s) {

    for (size_t i = 0; i < size(); i++) {
        if (s == sequenceNames[i]) {
            deletedTaxa.insert( i );
            
            break;
        }
    }
}


/** Map calls to member methods */
RbPtr<RbLanguageObject> Alignment::executeOperation(const std::string& name, Environment& args) {

    if (name == "names") 
    {
        return RbPtr<RbLanguageObject>( new VectorString(sequenceNames) );
    }
    else if (name == "ntaxa") 
        {
        int n = (int)getNumberOfTaxa();
        return RbPtr<RbLanguageObject>( new Natural(n) );
        }
    else if (name == "nchar")
        {
        int n = (int)getNumberOfCharacters();
        return RbPtr<RbLanguageObject>( new Natural(n) );
        }
    else if (name == "chartype")
        {
        std::string ct = getDataType();
        return RbPtr<RbLanguageObject>( new RbString(ct) );
        }
    else if (name == "nexcludedtaxa")
        {
        int n = (int)deletedTaxa.size();
        return RbPtr<RbLanguageObject>( new Natural(n) );
        }
    else if (name == "nexcludedchars")
        {
        int n = (int)deletedCharacters.size();
        return RbPtr<RbLanguageObject>( new Natural(n) );
        }
    else if (name == "nincludedtaxa")
        {
        int n = (int)(getNumberOfTaxa() - deletedTaxa.size());
        return RbPtr<RbLanguageObject>( new Natural(n) );
        }
    else if (name == "nincludedchars")
        {
        int n = (int)(getNumberOfCharacters() - deletedCharacters.size());
        return RbPtr<RbLanguageObject>( new Natural(n) );
        }
    else if (name == "excludedtaxa")
        {
        std::vector<std::string> et;
        for (std::set<size_t>::iterator it = deletedTaxa.begin(); it != deletedTaxa.end(); it++)
            {
            std::string tn = getTaxonNameWithIndex(*it);
            et.push_back( tn );
            }
        return RbPtr<RbLanguageObject>( new VectorString(et) );
        }
    else if (name == "excludedchars")
        {
        std::vector<int> ec;
        for (std::set<size_t>::iterator it = deletedCharacters.begin(); it != deletedCharacters.end(); it++)
            ec.push_back( (int)(*it) );
        return RbPtr<RbLanguageObject>( new VectorNatural(ec) );
        }
    else if (name == "includedtaxa")
        {
        std::vector<std::string> it;
        for (size_t i=0; i<getNumberOfTaxa(); i++)
            {
            if ( isTaxonExcluded(i) == false )
                it.push_back( getTaxonNameWithIndex(i) );
            }
        return RbPtr<RbLanguageObject>( new VectorString(it) );
        }
    else if (name == "includedchars")
        {
        std::vector<int> ic;
        for (size_t i=0; i<getNumberOfCharacters(); i++)
            {
            if ( isCharacterExcluded(i) == false )
                ic.push_back( (int)(i+1) );
            }
        return RbPtr<RbLanguageObject>( new VectorNatural(ic) );
        }
    else if (name == "nconstantpatterns")
        {
        int n = (int)numConstantPatterns();
        return RbPtr<RbLanguageObject>( new Natural(n) );
        }
    else if (name == "ncharswithambiguity")
        {
        int n = (int)numMissAmbig();
        return RbPtr<RbLanguageObject>( new Natural(n) );
        }
    else if (name == "excludechar")
        {
        const RbPtr<RbLanguageObject> argument = args[1]->getValue();

        if ( argument->isTypeSpec( TypeSpec(Natural_name) ) ) {
            
            int n = static_cast<const Natural*>( argument.get() )->getValue();
            deletedCharacters.insert( n );
        }
        else if ( argument->isTypeSpec( TypeSpec(VectorNatural_name) ) ) {
        
            std::vector<unsigned int> x = static_cast<const VectorNatural*>( argument.get() )->getValue();
            for ( size_t i=0; i<x.size(); i++ )
                deletedCharacters.insert( x[i] );
        }
            return RbPtr<RbLanguageObject>::getNullPtr();
        }

    return ConstantMemberObject::executeOperation( name, args );
}


/** Return a pointer to a character element in the character matrix */
const RbPtr<Character> Alignment::getCharacter( size_t tn, size_t cn ) const {

    if ( cn >= getNumberOfCharacters() )
        throw RbException( "Character index out of range" );

    return RbPtr<Character>((*getSequence( tn ))[cn]);
}


/** Get class vector describing type of object */
const VectorString& Alignment::getClass(void) const {

    static VectorString rbClass = VectorString(Alignment_name) + AbstractVector::getClass();
    return rbClass;
}


const std::string& Alignment::getDataType(void) const {
    return characterType;
}


RbPtr<RbObject> Alignment::getElement(size_t row, size_t col) const {
    const RbPtr<Sequence> sequence( dynamic_cast<Sequence*>(elements[row].get()) );
    return (sequence->getElement(col));
}


const std::string& Alignment::getFileName(void) const {
    return fileName;
}


/** Get member rules */
const RbPtr<MemberRules> Alignment::getMemberRules(void) const {

    static RbPtr<MemberRules> memberRules( new MemberRules() );
    return memberRules;
}


/** Get methods */
const RbPtr<MethodTable> Alignment::getMethods(void) const {

    static RbPtr<MethodTable> methods( new MethodTable() );
    static RbPtr<ArgumentRules> ncharArgRules( new ArgumentRules() );
    static RbPtr<ArgumentRules> namesArgRules( new ArgumentRules() );
    static RbPtr<ArgumentRules> ntaxaArgRules( new ArgumentRules() );
    static RbPtr<ArgumentRules> chartypeArgRules( new ArgumentRules() );    
    static RbPtr<ArgumentRules> nexcludedtaxaArgRules( new ArgumentRules() );    
    static RbPtr<ArgumentRules> nexcludedcharsArgRules( new ArgumentRules() );    
    static RbPtr<ArgumentRules> nincludedtaxaArgRules( new ArgumentRules() );    
    static RbPtr<ArgumentRules> nincludedcharsArgRules( new ArgumentRules() );    
    static RbPtr<ArgumentRules> excludedtaxaArgRules( new ArgumentRules() );    
    static RbPtr<ArgumentRules> excludedcharsArgRules( new ArgumentRules() );    
    static RbPtr<ArgumentRules> includedtaxaArgRules( new ArgumentRules() );    
    static RbPtr<ArgumentRules> includedcharsArgRules( new ArgumentRules() );    
    static RbPtr<ArgumentRules> nconstantpatternsArgRules( new ArgumentRules() );    
    static RbPtr<ArgumentRules> ncharswithambiguityArgRules( new ArgumentRules() );
    static RbPtr<ArgumentRules> excludecharArgRules( new ArgumentRules() );
    static RbPtr<ArgumentRules> excludecharArgRules2( new ArgumentRules() );
    static bool          methodsSet = false;

    if ( methodsSet == false ) 
        {

        excludecharArgRules->push_back(        RbPtr<ArgumentRule>( new ValueRule(     "", Natural_name       ) ) );
        excludecharArgRules2->push_back(       RbPtr<ArgumentRule>( new ValueRule(     "", VectorNatural_name ) ) );
            
        methods->addFunction("names",               RbPtr<RbFunction>( new MemberFunction(VectorString_name,  namesArgRules) ) );
        methods->addFunction("nchar",               RbPtr<RbFunction>( new MemberFunction(Natural_name,       ncharArgRules) ) );
        methods->addFunction("ntaxa",               RbPtr<RbFunction>( new MemberFunction(Natural_name,       ntaxaArgRules) ) );
        methods->addFunction("chartype",            RbPtr<RbFunction>( new MemberFunction(RbString_name,      chartypeArgRules) ) );
        methods->addFunction("nexcludedtaxa",       RbPtr<RbFunction>( new MemberFunction(Natural_name,       nexcludedtaxaArgRules) ) );
        methods->addFunction("nexcludedchars",      RbPtr<RbFunction>( new MemberFunction(Natural_name,       nexcludedcharsArgRules) ) );
        methods->addFunction("nincludedtaxa",       RbPtr<RbFunction>( new MemberFunction(Natural_name,       nincludedtaxaArgRules) ) );
        methods->addFunction("nincludedchars",      RbPtr<RbFunction>( new MemberFunction(Natural_name,       nincludedcharsArgRules) ) );
        methods->addFunction("excludedtaxa",        RbPtr<RbFunction>( new MemberFunction(VectorNatural_name, excludedtaxaArgRules) ) );
        methods->addFunction("excludedchars",       RbPtr<RbFunction>( new MemberFunction(VectorNatural_name, excludedcharsArgRules) ) );
        methods->addFunction("includedtaxa",        RbPtr<RbFunction>( new MemberFunction(VectorNatural_name, includedtaxaArgRules) ) );
        methods->addFunction("includedchars",       RbPtr<RbFunction>( new MemberFunction(VectorNatural_name, includedcharsArgRules) ) );
        methods->addFunction("nconstantpatterns",   RbPtr<RbFunction>( new MemberFunction(Natural_name,       nconstantpatternsArgRules) ) );
        methods->addFunction("ncharswithambiguity", RbPtr<RbFunction>( new MemberFunction(Natural_name,       ncharswithambiguityArgRules) ) );
        methods->addFunction("excludechar",         RbPtr<RbFunction>( new MemberFunction(RbVoid_name,        excludecharArgRules) ) );
        methods->addFunction("excludechar",         RbPtr<RbFunction>( new MemberFunction(RbVoid_name,        excludecharArgRules2) ) );
        
        // necessary call for proper inheritance
        methods->setParentTable( RbPtr<FunctionTable>( MemberObject::getMethods().get() ) );
        methodsSet = true;
        }

    return methods;
}


/** Return the number of characters in each vector of taxon observations */
size_t Alignment::getNumberOfCharacters(void) const {

    if (size() > 0) {
        return getSequence(0)->size();
    }
    return 0;
}


/** Get the number of states for the characters in this matrix. We
    assume that all of the characters in the matrix are of the same
    type and have the same number of potential states. */
size_t Alignment::getNumberOfStates(void) const {

    // Get the first character in the matrix
    if ( size() == 0 )
        return 0;

    const RbPtr<Sequence> sequence = getSequence( 0 );
    if ( sequence->size() == 0 )
        return 0;

    return static_cast<Character*>((*sequence)[0].get())->getNumberOfStates();
}


size_t Alignment::getNumberOfTaxa(void) const {
    return sequenceNames.size();
}


/** Get sequence with index tn */
const RbPtr<Sequence> Alignment::getSequence( size_t tn ) const {

    if ( tn >= getNumberOfTaxa() )
        throw RbException( "Taxon index out of range" );

    const RbPtr<Sequence> sequence( static_cast<Sequence*>( elements[tn].get() ) );

    return sequence;
}


/** Get taxon with index idx */
const std::string& Alignment::getTaxonNameWithIndex( size_t idx ) const {

    return sequenceNames[idx];
}


/** Get the type spec of this class. We return a member variable because instances might have different element types. */
const TypeSpec& Alignment::getTypeSpec(void) const {
    return typeSpec;
}


/** Return the index of the element ( the index of the taxon with name elemName ) */
size_t Alignment::indexOfTaxonWithName( std::string& s ) const {
    
    // search through all names
    for (size_t i=0; i<members->size(); i++) {
        if (s == members->getName( i )) {
            return i;
        }
    }

    return -1;
}


/** Is this character pattern constant? */
bool Alignment::isCharacterConstant(size_t idx) const {

    RbPtr<Character> f( NULL );
    for ( size_t i=0; i<getNumberOfTaxa(); i++ ) {

        if ( isTaxonExcluded(i) == false ) {
            
            if ( f == NULL )
                f = getCharacter( i, idx );
            else {
                const RbPtr<Character> s = getCharacter( i , idx );
                if ( (*f) != (*s) )
                    return false;
            }
        }
    }

    return true;
}


/** Is the character excluded */
bool Alignment::isCharacterExcluded(size_t i) const {

	std::set<size_t>::const_iterator it = deletedCharacters.find( i );
	if ( it != deletedCharacters.end() )
		return true;
    return false;
}


/** Does the character have missing or ambiguous characters */
bool Alignment::isCharacterMissingOrAmbiguous(size_t idx) const {

    for ( size_t i=0; i<getNumberOfTaxa(); i++ )
        {
        if ( isTaxonExcluded(i) == false )
            {
            const RbPtr<Character> c = getCharacter( i, idx );
            if ( c->isMissingOrAmbiguous() == true )
                return true;
            }
        }
    return false;
}


/** Is the taxon excluded */
bool Alignment::isTaxonExcluded(size_t i) const {

	std::set<size_t>::const_iterator it = deletedTaxa.find( i );
	if ( it != deletedTaxa.end() )
		return true;
    return false;
}


/** Is the taxon excluded */
bool Alignment::isTaxonExcluded(std::string& s) const {

    size_t i = indexOfTaxonWithName(s);
	std::set<size_t>::const_iterator it = deletedTaxa.find( i );
	if ( it != deletedTaxa.end() )
		return true;
    return false;
}


/** Make copy of site column with index cn */
RbPtr<Vector> Alignment::makeSiteColumn( size_t cn ) const {

    if ( cn >= getNumberOfCharacters() )
        throw RbException( "Site index out of range" );

    if ( getNumberOfTaxa() == 0 )
        throw RbException( "Character matrix is empty" );

    RbPtr<Vector> temp( static_cast<const Vector*>( (*members)[0]->getValue().get() )->clone() );
    temp->clear();
    for ( size_t i=0; i<getNumberOfTaxa(); i++ )
        temp->push_back( RbPtr<RbObject>( getCharacter( i, cn )->clone() ) );

    return temp;
}


/** Calculates and returns the number of constant characters */
size_t Alignment::numConstantPatterns(void) const {

    size_t nc = 0;
    for (size_t i=0; i<getNumberOfCharacters(); i++)
        {
        if ( isCharacterExcluded(i) == false && isCharacterConstant(i) == true )
            nc++;
        }
    return nc;
}


/** Returns the number of characters with missing or ambiguous data */
size_t Alignment::numMissAmbig(void) const {

    size_t nma = 0;
    for (size_t i=0; i<getNumberOfCharacters(); i++)
        {
        if ( isCharacterExcluded(i) == false && isCharacterMissingOrAmbiguous(i) == true )
            nma++;
        }
    return nma;
}


/** Print value for user */
void Alignment::printValue(std::ostream& o) const {

    o << "Origination:          " << fileName << std::endl;
    o << "Data type:            " << getDataType() << std::endl;
    o << "Number of taxa:       " << getNumberOfTaxa() << std::endl;
    o << "Number of characters: " << getNumberOfCharacters() << std::endl;
}


void Alignment::resize(size_t nRows, size_t nCols) {
    
    throw RbException("Not implemented method Alignment::resize(rows,cols)");
}


/** Restore a character */
void Alignment::restoreCharacter(size_t i) {

    if (i >= getNumberOfCharacters() )
        throw RbException( "Character index out of range" );

    deletedCharacters.erase( i );
}


/** Restore a taxon */
void Alignment::restoreTaxon(size_t i) {

    if ( i >= getNumberOfTaxa() )
        return;

    deletedTaxa.erase( i );
}


/** Restore a taxon */
void Alignment::restoreTaxon(std::string& s) {

    size_t i = indexOfTaxonWithName( s );

    deletedTaxa.erase( i );
}


/** Complete info */
std::string Alignment::richInfo(void) const {

	std::ostringstream o;
    printValue( o );
    return o.str();
}

/** Overloaded container setElement method */
void Alignment::setElement( const size_t index, RbPtr<RbLanguageObject> var ) {
    
    if (var->isTypeSpec(TypeSpec(Sequence_name))) {
        RbPtr<Sequence> seq( static_cast<Sequence*>(var.get()) );
        
        sequenceNames.erase(sequenceNames.begin() + index);
        sequenceNames.insert(sequenceNames.begin() + index,seq->getTaxonName());
        elements.insert( elements.begin() + index, var );
        
        // add the sequence also as a member so that we can access it by name
        RbPtr<Variable> variable( new Variable( RbPtr<DAGNode>( new ConstantNode(var) ) ) );
        members->addVariable(seq->getTaxonName(), variable );
    }
}

/** Overloaded container setElement method */
void Alignment::setElement( size_t row, size_t col, RbPtr<RbLanguageObject> var ) {
    
    throw RbException("Not implemented method Alignment::setElement()");
}


/** transpose the matrix */
void Alignment::transpose() {
    throw RbException("Transpose of Alignment not supported.");
}




