/**
 * @file
 * This file contains the implementation of Mcmc, which is used to hold
 * information about and run an mcmc analysis.
 *
 * @brief Implementation of Mcmc
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

#include "CharacterMatrix.h"
#include "CharacterObservation.h"
#include "MemberFunction.h"
#include "MemberNode.h"
#include "MoveSchedule.h"
#include "Natural.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "ReferenceRule.h"
#include "StochasticNode.h"
#include "ValueRule.h"
#include "VariableNode.h"
#include "VectorCharacterObservations.h"
#include "VectorIndex.h"
#include "VectorNatural.h"
#include "VectorString.h"
#include "Workspace.h"

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>


/** Constructor passes member rules and method inits to base class */
CharacterMatrix::CharacterMatrix(void) : MemberObject(getMemberRules()) {

}


/** Add a taxon name */
void CharacterMatrix::addTaxonName(const std::string tName) {

    taxonNames.push_back( tName );
}


/** Add a vector of taxon observations */
void CharacterMatrix::addTaxonObservations(VectorCharacterObservations* obs) {

    taxonObservations.push_back( obs );
}


/** Clone object */
CharacterMatrix* CharacterMatrix::clone(void) const {

    return new CharacterMatrix(*this);
}


/** Exclude a character */
void CharacterMatrix::excludeCharacter(size_t i) {

    if (i >= taxonObservations.size())
        return;
    deletedCharacters.insert( i );
}


/** Exclude a taxon */
void CharacterMatrix::excludeTaxon(size_t i) {

    if (i >= taxonNames.size())
        return;
    deletedTaxa.insert( i );
}


/** Exclude a taxon */
void CharacterMatrix::excludeTaxon(std::string& s) {

    size_t i = indexOfTaxonWithName( s );
    if (i == -1)
        return;
    if (i >= taxonNames.size())
        return;
    deletedTaxa.insert( i );
}


/** Map calls to member methods */
DAGNode* CharacterMatrix::executeOperation(const std::string& name, ArgumentFrame& args) {

    if (name == "ntaxa") 
        {
        int n = (int)getNumTaxa();
        return ( new Natural(n) )->wrapIntoVariable();
        }
    else if (name == "nchar")
        {
        int n = (int)getNumCharacters();
        return ( new Natural(n) )->wrapIntoVariable();
        }
    else if (name == "chartype")
        {
        std::string ct = dataType;
        return ( new RbString(ct) )->wrapIntoVariable();
        }
    else if (name == "nexcludedtaxa")
        {
        int n = (int)deletedTaxa.size();
        return ( new Natural(n) )->wrapIntoVariable();
        }
    else if (name == "nexcludedchars")
        {
        int n = (int)deletedCharacters.size();
        return ( new Natural(n) )->wrapIntoVariable();
        }
    else if (name == "nincludedtaxa")
        {
        int n = (int)(getNumTaxa() - deletedTaxa.size());
        return ( new Natural(n) )->wrapIntoVariable();
        }
    else if (name == "nincludedchars")
        {
        int n = (int)(getNumCharacters() - deletedCharacters.size());
        return ( new Natural(n) )->wrapIntoVariable();
        }
    else if (name == "excludedtaxa")
        {
        std::vector<std::string> et;
        for (std::set<size_t>::iterator it = deletedTaxa.begin(); it != deletedTaxa.end(); it++)
            {
            std::string tn = getTaxonWithIndex(*it);
            et.push_back( tn );
            }
        return ( new VectorString(et) )->wrapIntoVariable();
        }
    else if (name == "excludedchars")
        {
        std::vector<int> ec;
        for (std::set<size_t>::iterator it = deletedCharacters.begin(); it != deletedCharacters.end(); it++)
            ec.push_back( (int)(*it) );
        return ( new VectorNatural(ec) )->wrapIntoVariable();
        }
    else if (name == "includedtaxa")
        {
        std::vector<std::string> it;
        for (size_t i=0; i<getNumTaxa(); i++)
            {
            if ( isTaxonExcluded(i) == false )
                it.push_back( getTaxonWithIndex(i) );
            }
        return ( new VectorString(it) )->wrapIntoVariable();
        }
    else if (name == "includedchars")
        {
        std::vector<int> ic;
        for (size_t i=0; i<getNumCharacters(); i++)
            {
            if ( isCharacterExcluded(i) == false )
                ic.push_back( (int)(i+1) );
            }
        return ( new VectorNatural(ic) )->wrapIntoVariable();
        }
    else if (name == "nconstantpatterns")
        {
        int n = (int)numConstantPatterns();
        return ( new Natural(n) )->wrapIntoVariable();
        }
    else if (name == "ncharswithambiguity")
        {
        int n = 0;
        return ( new Natural(n) )->wrapIntoVariable();
        }

    return MemberObject::executeOperation( name, args );
}


/** Get class vector describing type of object */
const VectorString& CharacterMatrix::getClass(void) const {

    static VectorString rbClass = VectorString(CharacterMatrix_name) + MemberObject::getClass();
    return rbClass;
}


/** Get member rules */
const MemberRules& CharacterMatrix::getMemberRules(void) const {

    static MemberRules memberRules;
    static bool        rulesSet = false;

    if (!rulesSet) 
        {
        rulesSet = true;
        }

    return memberRules;
}


/** Get methods */
const MethodTable& CharacterMatrix::getMethods(void) const {

    static MethodTable   methods;
    static ArgumentRules ncharArgRules;
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
    static bool          methodsSet = false;

    if ( methodsSet == false ) 
        {
        // this must be here so the parser can distinguish between different instances of a character matrix
        ncharArgRules.push_back(               new ReferenceRule( "", MemberObject_name ) );
        ntaxaArgRules.push_back(               new ReferenceRule( "", MemberObject_name ) );
        chartypeArgRules.push_back(            new ReferenceRule( "", MemberObject_name ) );
        nexcludedtaxaArgRules.push_back(       new ReferenceRule( "", MemberObject_name ) );
        nexcludedcharsArgRules.push_back(      new ReferenceRule( "", MemberObject_name ) );
        nincludedtaxaArgRules.push_back(       new ReferenceRule( "", MemberObject_name ) );
        nincludedcharsArgRules.push_back(      new ReferenceRule( "", MemberObject_name ) );
        excludedtaxaArgRules.push_back(        new ReferenceRule( "", MemberObject_name ) );
        excludedcharsArgRules.push_back(       new ReferenceRule( "", MemberObject_name ) );
        includedtaxaArgRules.push_back(        new ReferenceRule( "", MemberObject_name ) );
        includedcharsArgRules.push_back(       new ReferenceRule( "", MemberObject_name ) );
        nconstantpatternsArgRules.push_back(   new ReferenceRule( "", MemberObject_name ) );
        ncharswithambiguityArgRules.push_back( new ReferenceRule( "", MemberObject_name ) );
        
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
        
        // necessary call for proper inheritance
        methods.setParentTable( const_cast<MethodTable*>( &MemberObject::getMethods() ) );
        methodsSet = true;
        }

    return methods;
}


/** Return the number of characters in each vector of taxon observations */
size_t CharacterMatrix::getNumCharacters(void) const {

    if ( taxonObservations.size() == 0 )
        return 0;
    return taxonObservations[0]->getNumCharacters();
}


/** Return the index of the taxon */
size_t CharacterMatrix::indexOfTaxonWithName(std::string& s) {

    for (size_t i=0; i<taxonNames.size(); i++)
        {
        if ( taxonNames[i] == s )
            return i;
        }
    return -1;
}


/** Is this character pattern constant? */
bool CharacterMatrix::isCharacterConstant(size_t idx) {

    return false;
}


/** Is the character excluded */
bool CharacterMatrix::isCharacterExcluded(size_t i) {

	std::set<size_t>::iterator it = deletedCharacters.find( i );
	if ( it != deletedCharacters.end() )
		return true;
    return false;
}


/** Is the taxon excluded */
bool CharacterMatrix::isTaxonExcluded(size_t i) {

	std::set<size_t>::iterator it = deletedTaxa.find( i );
	if ( it != deletedTaxa.end() )
		return true;
    return false;
}


/** Is the taxon excluded */
bool CharacterMatrix::isTaxonExcluded(std::string& s) {

    size_t i = indexOfTaxonWithName(s);
	std::set<size_t>::iterator it = deletedTaxa.find( i );
	if ( it != deletedTaxa.end() )
		return true;
    return false;
}


/** Calculates and returns the number of constant characters */
size_t CharacterMatrix::numConstantPatterns(void) {

    size_t nc = 0;
    for (size_t i=0; i<getNumCharacters(); i++)
        {
        if ( isCharacterExcluded(i) == false && isCharacterConstant(i) == true )
            nc++;
        }
    
    return nc;
}


/** Print value for user */
void CharacterMatrix::printValue(std::ostream& o) const {

    o << "Origination:          " << fileName << std::endl;
    o << "Data type:            " << dataType << std::endl;
    o << "Number of taxa:       " << getNumTaxa() << std::endl;
    o << "Number of characters: " << getNumCharacters() << std::endl;
}


/** Complete info */
std::string CharacterMatrix::richInfo(void) const {

	std::ostringstream o;
    printValue( o );
    return o.str();
}


/** Restore a character */
void CharacterMatrix::restoreCharacter(size_t i) {

    if (i >= taxonObservations.size())
        return;
    deletedCharacters.erase( i );
}


/** Restore a taxon */
void CharacterMatrix::restoreTaxon(size_t i) {

    if (i >= taxonNames.size())
        return;
    deletedTaxa.erase( i );
}


/** Restore a taxon */
void CharacterMatrix::restoreTaxon(std::string& s) {

    size_t i = indexOfTaxonWithName( s );
    if (i == -1)
        return;
    if (i >= taxonNames.size())
        return;
    deletedTaxa.erase( i );
}


/** Allow only constant member variables */
void CharacterMatrix::setVariable(const std::string& name, DAGNode* var) {

    if ( name != "model" && !var->isDAGType( ConstantNode_name ) )
        throw RbException( "Only constant member values allowed" );

    if ( name == "model" && members["model"].getValue() != NULL )
        throw RbException( "Cannot reset model" );

    MemberObject::setVariable(name, var);
}


/** Wrap value into a variable */
DAGNode* CharacterMatrix::wrapIntoVariable( void ) {
    
    MemberNode* nde = new MemberNode( this );
    return static_cast<DAGNode*>(nde);
}

