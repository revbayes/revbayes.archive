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
#include "Character.h"
#include "CharacterData.h"
#include "ConstantNode.h"
#include "DnaState.h"
#include "Ellipsis.h"
#include "MemberFunction.h"
#include "Natural.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "RbString.h"
#include "StochasticNode.h"
#include "TaxonData.h"
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
CharacterData::CharacterData( const std::string& charType ) : MemberObject( getMemberRules() ), typeSpec( getClassName(), new TypeSpec( Matrix::getClassTypeSpec() ),new TypeSpec(charType) ) {

    characterType = charType;
}


/** Copy constructor */
CharacterData::CharacterData(const CharacterData& x) : MemberObject( x ), typeSpec( getClassName(), new TypeSpec( Matrix::getClassTypeSpec() ), new TypeSpec(characterType) ) {

    characterType           = x.characterType;
    deletedTaxa             = x.deletedTaxa;
    deletedCharacters       = x.deletedCharacters;
    fileName                = x.fileName;
    sequenceNames           = x.sequenceNames;
    isHomologyEstablished   = x.isHomologyEstablished;
    taxonMap                = x.taxonMap;
}


/** Destructor */
CharacterData::~CharacterData( void ) {

}


/** Assignment operator */
CharacterData& CharacterData::operator=( const CharacterData& x ) {

    if ( this != &x ) 
        {
        if ( characterType != x.characterType )
            throw RbException( "Invalid assignment of character matrices: sequence data types differ" );

        MemberObject::operator=( x );

        characterType           = x.characterType;
        deletedTaxa             = x.deletedTaxa;
        deletedCharacters       = x.deletedCharacters;
        fileName                = x.fileName;
        sequenceNames           = x.sequenceNames;
        isHomologyEstablished   = x.isHomologyEstablished;
        taxonMap                = x.taxonMap;
        }
    return (*this);
}


/** Index (const) operator */
const TaxonData& CharacterData::operator[]( const size_t i ) const {

    return getTaxonData( i );
}


/** Add a sequence to the character matrix. For now, we require same data type and same length. */
void CharacterData::addTaxonData(TaxonData* obs, bool forceAdd) {
    
    // add the sequence name to the list
    sequenceNames.push_back(obs->getTaxonName());
    
    // add the sequence also as a member so that we can access it by name
    DAGNode* var = new ConstantNode( obs );
    taxonMap.insert( std::pair<std::string,RbVariablePtr>( obs->getTaxonName(), new Variable( var ) ) );
}

void CharacterData::addTaxonData( TaxonData* obs ) {

    // set the number of character per sequence
    if ( size() > 0 && obs->getNumberOfCharacters() != getNumberOfCharacters() ) 
        {
        std::ostringstream msg;
        msg << "Invalid attempt to add sequence of length " << obs->getNumberOfCharacters() << " to aligned character matrix of length " << getNumberOfCharacters();
        throw RbException( msg );
        }
        
    addTaxonData(obs, true);
}


/** clear the oblect */
void CharacterData::clear( void ) {
    
    sequenceNames.clear();
    taxonMap.clear();
}


/** Clone object */
CharacterData* CharacterData::clone(void) const {

    return new CharacterData(*this);
}


/** Exclude a character */
void CharacterData::excludeCharacter(size_t i) {

    if (i >= getNumberOfCharacters() )
        throw RbException( "Only " + RbString(int(getNumberOfCharacters())) + " characters in matrix" );

    deletedCharacters.insert( i );
}


/** Exclude a taxon */
void CharacterData::excludeTaxon(size_t i) {

    if (i >= taxonMap.size())
        throw RbException( "Only " + RbString(int(taxonMap.size())) + " taxa in matrix" );

    deletedTaxa.insert( i );
}


/** Exclude a taxon */
void CharacterData::excludeTaxon(std::string& s) {

    for (size_t i = 0; i < size(); i++) 
        {
        if (s == sequenceNames[i]) 
            {
            deletedTaxa.insert( i );
            break;
            }
        }
}


/** Map calls to member methods */
const RbLanguageObject& CharacterData::executeOperationSimple(const std::string& name, const std::vector<Argument>& args) {

    if (name == "names") 
        {
        return sequenceNames;
        }
    else if (name == "[]") 
        {
        // get the member with give index
        const Natural& index = static_cast<const Natural&>( args[0].getVariable().getValue() );
        
        if (size() < (size_t)(index.getValue()) ) {
            throw RbException("Index out of bounds in []");
        }
        
        // TODO: Check what happens with DAGNodeContainers
        const TaxonData& element = getTaxonData(index.getValue() - 1);
        return element;
        }
    else if (name == "ntaxa") 
        {
        int n = (int)getNumberOfTaxa();
        numTaxa.setValue( n );
        return numTaxa;
        }
    else if (name == "nchar")
        {
        std::vector<int> nc;
        
        for (size_t i=0; i<getNumberOfTaxa(); i++)
            {
            if ( isTaxonExcluded(i) == false )
                {
                if (isHomologyEstablished == true)
                    nc.push_back( (int)getNumberOfCharacters() );
                else
                    nc.push_back( (int)getTaxonData(i).getNumberOfCharacters() );
                }
            }
        numChar.setValue( nc );
        return numChar;
        }
    else if (name == "chartype")
        {
        return characterType;
        }
    else if (name == "nexcludedtaxa")
        {
        int n = (int)deletedTaxa.size();
        numExcludedTaxa.setValue( n );
        return numExcludedTaxa;
        }
    else if (name == "nexcludedchars")
        {
        int n = (int)deletedCharacters.size();
        numExcludedChars.setValue( n );
        return numExcludedChars;
        }
    else if (name == "nincludedtaxa")
        {
        int n = (int)(getNumberOfTaxa() - deletedTaxa.size());
        numIncludedTaxa.setValue( n );
        return numIncludedTaxa;
        }
    else if (name == "nincludedchars")
        {
        int n = (int)(getNumberOfCharacters() - deletedCharacters.size());
        numIncludedChars.setValue( n );
        return numIncludedChars;
        }
    else if (name == "excludedtaxa")
        {
        std::vector<std::string> et;
        for (std::set<size_t>::iterator it = deletedTaxa.begin(); it != deletedTaxa.end(); it++)
            {
            std::string tn = getTaxonNameWithIndex(*it);
            et.push_back( tn );
            }
        excludedTaxa.setValue( et );
        return excludedTaxa;
        }
    else if (name == "excludedchars")
        {
        std::vector<int> ec;
        for (std::set<size_t>::iterator it = deletedCharacters.begin(); it != deletedCharacters.end(); it++)
            ec.push_back( (int)(*it) );
        excludedChars.setValue( ec );
        return excludedChars;
        }
    else if (name == "includedtaxa")
        {
        std::vector<std::string> it;
        for (size_t i=0; i<getNumberOfTaxa(); i++)
            {
            if ( isTaxonExcluded(i) == false )
                it.push_back( getTaxonNameWithIndex(i) );
            }
        includedTaxa.setValue( it );
        return includedTaxa;
        }
    else if (name == "includedchars")
        {
        std::vector<int> ic;
        for (size_t i=0; i<getNumberOfCharacters(); i++)
            {
            if ( isCharacterExcluded(i) == false )
                ic.push_back( (int)(i+1) );
            }
        includedChars.setValue( ic );
        return includedTaxa;
        }
    else if (name == "nconstantpatterns")
        {
        int n = (int)numConstantPatterns();
        numConstPatterns.setValue( n );
        return numConstPatterns;
        }
    else if (name == "ncharswithambiguity")
        {
        int n = (int)numMissAmbig();
            numMissing.setValue( n );
        return numMissing;
        }
    else if (name == "excludechar")
        {
        const RbLanguageObject& argument = args[1].getVariable().getValue();
        if ( argument.isTypeSpec( Natural::getClassTypeSpec() ) ) 
            {
            int n = static_cast<const Natural&>( argument ).getValue();
            deletedCharacters.insert( n );
            }
        else if ( argument.isTypeSpec( VectorNatural::getClassTypeSpec() ) ) 
            {
            std::vector<unsigned int> x = static_cast<const VectorNatural&>( argument ).getValue();
            for ( size_t i=0; i<x.size(); i++ )
                deletedCharacters.insert( x[i] );
            }
        return RbNullObject::getInstance();
        }
    else if (name == "show")
        {
        int nt = (int)getNumberOfTaxa();
        for (int i=0; i<nt; i++)
            {
            const TaxonData& taxonData = getTaxonData(i);
            std::string taxonName = getTaxonNameWithIndex(i);
            int nc = (int)taxonData.getNumberOfCharacters();
            std::cout << "   " << taxonName << std::endl;
            std::cout << "   ";
            for (int j=0; j<nc; j++)
                {
                RbObject& o = getElement(i, j);
                Character& c = dynamic_cast<Character&>(o);
                if (&c == NULL)
                    throw RbException( "Problem retreiving character from Character Data object" );
                std::string s = c.getStringValue();
                
                std::cout << s;
                if ( (j+1) % 100 == 0 && (j+1) != nc )
                    std::cout << std::endl << "   ";
                }
            std::cout << std::endl;
            }
        return RbNullObject::getInstance();
        }
    else if (name == "ishomologous")
        {
        bool ih = getIsHomologyEstablished();
        isHomologous.setValue( ih );
        return isHomologous;
        }

    return MemberObject::executeOperationSimple( name, args );
}


/** Return a pointer to a character element in the character matrix */
const Character& CharacterData::getCharacter( size_t tn, size_t cn ) const {

    if ( cn >= getNumberOfCharacters() )
        throw RbException( "Character index out of range" );

    return getTaxonData( tn )[cn];
}


/** Get class name of object */
const std::string& CharacterData::getClassName(void) { 
    
    static std::string rbClassName = "Character data";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& CharacterData::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Matrix::getClassTypeSpec() ) );
    
	return rbClass; 
}


const std::string CharacterData::getDataType(void) const {

    std::string stlStr = characterType.getValue();
    return stlStr;
}


const RbObject& CharacterData::getElement(size_t row, size_t col) const {

    const TaxonData& sequence = getTaxonData( row );
    return sequence.getElement(col);
}


RbObject& CharacterData::getElement(size_t row, size_t col) {

    throw RbException("Non-const getElement(row,col) not implemented in CharacterData.");
}


const std::string& CharacterData::getFileName(void) const {

    return fileName;
}


const std::map<std::string, RbVariablePtr>& CharacterData::getMembers(void) const {
    return taxonMap;
}


std::map<std::string, RbVariablePtr>& CharacterData::getMembers(void) {
    return taxonMap;
}


/** Get member rules */
const MemberRules& CharacterData::getMemberRules(void) const {

    static MemberRules memberRules = MemberRules();
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        
        memberRules.push_back( new ValueRule("type", RbString::getClassTypeSpec() ) );
        memberRules.push_back( new ValueRule("x", TaxonData::getClassTypeSpec() ) );
        memberRules.push_back( new Ellipsis( TaxonData::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    
    return memberRules;
}


/** Get methods */
const MethodTable& CharacterData::getMethods(void) const {

    static MethodTable    methods                     = MethodTable();
    static ArgumentRules* ncharArgRules               = new ArgumentRules();
    static ArgumentRules* namesArgRules               = new ArgumentRules();
    static ArgumentRules* ntaxaArgRules               = new ArgumentRules();
    static ArgumentRules* chartypeArgRules            = new ArgumentRules();    
    static ArgumentRules* nexcludedtaxaArgRules       = new ArgumentRules();    
    static ArgumentRules* nexcludedcharsArgRules      = new ArgumentRules();    
    static ArgumentRules* nincludedtaxaArgRules       = new ArgumentRules();    
    static ArgumentRules* nincludedcharsArgRules      = new ArgumentRules();    
    static ArgumentRules* excludedtaxaArgRules        = new ArgumentRules();    
    static ArgumentRules* excludedcharsArgRules       = new ArgumentRules();    
    static ArgumentRules* includedtaxaArgRules        = new ArgumentRules();    
    static ArgumentRules* includedcharsArgRules       = new ArgumentRules();    
    static ArgumentRules* nconstantpatternsArgRules   = new ArgumentRules();    
    static ArgumentRules* ncharswithambiguityArgRules = new ArgumentRules();
    static ArgumentRules* excludecharArgRules         = new ArgumentRules();
    static ArgumentRules* excludecharArgRules2        = new ArgumentRules();
    static ArgumentRules* showdataArgRules            = new ArgumentRules();
    static ArgumentRules* ishomologousArgRules        = new ArgumentRules();    
    static bool           methodsSet                  = false;

    if ( methodsSet == false ) 
        {

        excludecharArgRules->push_back(        new ValueRule(     "", Natural::getClassTypeSpec()       ) );
        excludecharArgRules2->push_back(       new ValueRule(     "", VectorNatural::getClassTypeSpec() ) );
        
        methods.addFunction("names",               new MemberFunction(VectorString::getClassTypeSpec(),  namesArgRules              ) );
        methods.addFunction("nchar",               new MemberFunction(VectorNatural::getClassTypeSpec(), ncharArgRules              ) );
        methods.addFunction("ntaxa",               new MemberFunction(Natural::getClassTypeSpec(),       ntaxaArgRules              ) );
        methods.addFunction("chartype",            new MemberFunction(RbString::getClassTypeSpec(),      chartypeArgRules           ) );
        methods.addFunction("nexcludedtaxa",       new MemberFunction(Natural::getClassTypeSpec(),       nexcludedtaxaArgRules      ) );
        methods.addFunction("nexcludedchars",      new MemberFunction(Natural::getClassTypeSpec(),       nexcludedcharsArgRules     ) );
        methods.addFunction("nincludedtaxa",       new MemberFunction(Natural::getClassTypeSpec(),       nincludedtaxaArgRules      ) );
        methods.addFunction("nincludedchars",      new MemberFunction(Natural::getClassTypeSpec(),       nincludedcharsArgRules     ) );
        methods.addFunction("excludedtaxa",        new MemberFunction(VectorNatural::getClassTypeSpec(), excludedtaxaArgRules       ) );
        methods.addFunction("excludedchars",       new MemberFunction(VectorNatural::getClassTypeSpec(), excludedcharsArgRules      ) );
        methods.addFunction("includedtaxa",        new MemberFunction(VectorNatural::getClassTypeSpec(), includedtaxaArgRules       ) );
        methods.addFunction("includedchars",       new MemberFunction(VectorNatural::getClassTypeSpec(), includedcharsArgRules      ) );
        methods.addFunction("nconstantpatterns",   new MemberFunction(Natural::getClassTypeSpec(),       nconstantpatternsArgRules  ) );
        methods.addFunction("ncharswithambiguity", new MemberFunction(Natural::getClassTypeSpec(),       ncharswithambiguityArgRules) );
        methods.addFunction("excludechar",         new MemberFunction(RbVoid_name,        excludecharArgRules        ) );
        methods.addFunction("excludechar",         new MemberFunction(RbVoid_name,        excludecharArgRules2       ) );
        methods.addFunction("show",                new MemberFunction(RbVoid_name,        showdataArgRules           ) );
        methods.addFunction("ishomologous",        new MemberFunction(RbBoolean::getClassTypeSpec(),     ishomologousArgRules       ) );
            
        // add method for call "x[]" as a function
        ArgumentRules* squareBracketArgRules = new ArgumentRules();
        squareBracketArgRules->push_back( new ValueRule( "index" , Natural::getClassTypeSpec() ) );
        methods.addFunction("[]",  new MemberFunction( TaxonData::getClassTypeSpec(), squareBracketArgRules) );
            
        // necessary call for proper inheritance
        methods.setParentTable( &MemberObject::getMethods() );
        methodsSet = true;
        }

    return methods;
}


/** Return the number of characters in each vector of taxon observations */
size_t CharacterData::getNumberOfCharacters(void) const {

    if (size() > 0) 
        {
        return getTaxonData(0).getNumberOfCharacters();
        }
    return 0;
}


/** Return the number of characters in each vector of taxon observations */
size_t CharacterData::getNumberOfCharacters(size_t idx) const {

    if (size() > 0) 
        {
        return getTaxonData(idx).getNumberOfCharacters();
        }
    return 0;
}


/** Get the number of states for the characters in this matrix. We
    assume that all of the characters in the matrix are of the same
    type and have the same number of potential states. */
size_t CharacterData::getNumberOfStates(void) const {

    // Get the first character in the matrix
    if ( size() == 0 )
        return 0;

    const TaxonData& sequence = getTaxonData( 0 );
    if ( sequence.getNumberOfCharacters() == 0 )
        return 0;

    return static_cast<const Character&>(sequence[0]).getNumberOfStates();
}


size_t CharacterData::getNumberOfTaxa(void) const {
    return sequenceNames.size();
}


/** Get sequence with index tn */
const TaxonData& CharacterData::getTaxonData( size_t tn ) const {

    if ( tn >= getNumberOfTaxa() )
        throw RbException( "Taxon index out of range" );

    const std::string& name = sequenceNames[tn];
    const std::map<std::string, RbVariablePtr>::const_iterator& i = taxonMap.find(name); 
    
    if (i != taxonMap.end() ) {
        return static_cast<const TaxonData&>( i->second->getValue() );
    }
    else {
        throw RbException("Cannot find the taxon with name '" + name + "' in the CharacterData matrix. This should actually never happen. Please report this bug!");
    }

}


/** Get taxon with index idx */
const std::string& CharacterData::getTaxonNameWithIndex( size_t idx ) const {

    return sequenceNames[idx];
}


/** Get the type spec of this class. We return a member variable because instances might have different element types. */
const TypeSpec& CharacterData::getTypeSpec(void) const {
    return typeSpec;
}


/** Return the index of the element ( the index of the taxon with name elemName ) */
size_t CharacterData::indexOfTaxonWithName( std::string& s ) const {
    
    // search through all names
    for (size_t i=0; i<sequenceNames.size(); i++) {
        if (s == sequenceNames[i]) {
            return i;
        }
    }

    return -1;
}


/** Is this character pattern constant? */
bool CharacterData::isCharacterConstant(size_t idx) const {

    const Character* f = NULL;
    for ( size_t i=0; i<getNumberOfTaxa(); i++ ) {

        if ( isTaxonExcluded(i) == false ) {
            
            if ( f == NULL )
                f = &getCharacter( i, idx );
            else {
                const Character* s = &getCharacter( i , idx );
                if ( (*f) != (*s) )
                    return false;
            }
        }
    }

    return true;
}


/** Is the character excluded */
bool CharacterData::isCharacterExcluded(size_t i) const {

	std::set<size_t>::const_iterator it = deletedCharacters.find( i );
	if ( it != deletedCharacters.end() )
		return true;
    return false;
}


/** Does the character have missing or ambiguous characters */
bool CharacterData::isCharacterMissingOrAmbiguous(size_t idx) const {

    for ( size_t i=0; i<getNumberOfTaxa(); i++ )
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
bool CharacterData::isTaxonExcluded(size_t i) const {

	std::set<size_t>::const_iterator it = deletedTaxa.find( i );
	if ( it != deletedTaxa.end() )
		return true;
    return false;
}


/** Is the taxon excluded */
bool CharacterData::isTaxonExcluded(std::string& s) const {

    size_t i = indexOfTaxonWithName(s);
	std::set<size_t>::const_iterator it = deletedTaxa.find( i );
	if ( it != deletedTaxa.end() )
		return true;
    return false;
}


/** Make copy of site column with index cn */
Vector* CharacterData::makeSiteColumn( size_t cn ) const {

    if ( cn >= getNumberOfCharacters() )
        throw RbException( "Site index out of range" );

    if ( getNumberOfTaxa() == 0 )
        throw RbException( "Character matrix is empty" );

    const std::string& name = sequenceNames[0];
    const std::map<std::string, RbVariablePtr>::const_iterator& it = taxonMap.find(name);
    Vector* temp = static_cast<Vector*>( ( it->second->getValue() ).clone() );
    temp->clear();
    for ( size_t i=0; i<getNumberOfTaxa(); i++ )
        temp->push_back( getCharacter( i, cn ).clone() );

    return temp;
}


/** Calculates and returns the number of constant characters */
size_t CharacterData::numConstantPatterns(void) const {

    size_t nc = 0;
    for (size_t i=0; i<getNumberOfCharacters(); i++)
        {
        if ( isCharacterExcluded(i) == false && isCharacterConstant(i) == true )
            nc++;
        }
    return nc;
}


/** Returns the number of characters with missing or ambiguous data */
size_t CharacterData::numMissAmbig(void) const {

    size_t nma = 0;
    for (size_t i=0; i<getNumberOfCharacters(); i++)
        {
        if ( isCharacterExcluded(i) == false && isCharacterMissingOrAmbiguous(i) == true )
            nma++;
        }
    return nma;
}


/** Print value for user */
void CharacterData::printValue(std::ostream& o) const {

    o << "Origination:          " << fileName << std::endl;
    o << "Data type:            " << getDataType() << std::endl;
    o << "Number of taxa:       " << getNumberOfTaxa() << std::endl;
    o << "Number of characters: " << getNumberOfCharacters() << std::endl;
}


void CharacterData::resize(size_t nRows, size_t nCols) {
    
    throw RbException("Not implemented method Alignment::resize(rows,cols)");
}


/** Restore a character */
void CharacterData::restoreCharacter(size_t i) {

    if (i >= getNumberOfCharacters() )
        throw RbException( "Character index out of range" );

    deletedCharacters.erase( i );
}


/** Restore a taxon */
void CharacterData::restoreTaxon(size_t i) {

    if ( i >= getNumberOfTaxa() )
        return;

    deletedTaxa.erase( i );
}


/** Restore a taxon */
void CharacterData::restoreTaxon(std::string& s) {

    size_t i = indexOfTaxonWithName( s );

    deletedTaxa.erase( i );
}


/** Overloaded container setElement method */
void CharacterData::setElement( const size_t index, RbLanguageObject* var ) {
    
    if (var->isTypeSpec( TaxonData::getClassTypeSpec() )) {
        TaxonData* seq = static_cast<TaxonData*>( var );
        
//        sequenceNames.erase(sequenceNames.begin() + index);
//        sequenceNames.insert(sequenceNames.begin() + index,seq->getTaxonName());        
        sequenceNames[index] = seq->getTaxonName();
        
        // add the sequence also as a member so that we can access it by name
        DAGNode* variable = new ConstantNode(var );
        taxonMap.insert( std::pair<std::string,RbVariablePtr>( seq->getTaxonName(), new Variable( variable ) ) );
    }
}

/** Overloaded container setElement method */
void CharacterData::setElement( size_t row, size_t col, RbLanguageObject* var ) {
    
    throw RbException("Not implemented method Alignment::setElement()");
}


/** Catch setting of the mixture variable */
void CharacterData::setMemberVariable(const std::string& name, Variable* var) {
    
    if ( name == "type" ) {
//        elementType = TypeSpec( static_cast<RbString&>( var->getValue() ).getValue() );
    }
    else if ( name == "x" || name == "" ) {
        TaxonData& obs = static_cast<TaxonData&>( var->getValue() );
        addTaxonData( obs.clone() );        
    }
    else {
        MemberObject::setMemberVariable(name, var);
    }
}



void CharacterData::showData(void) {

}


size_t CharacterData::size(void) const {
    return taxonMap.size();
}

/** transpose the matrix */
void CharacterData::transpose() {

    throw RbException("Transpose of Alignment not supported.");
}




