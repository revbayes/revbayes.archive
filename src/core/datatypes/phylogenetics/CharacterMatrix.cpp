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
#include "MemberFunction.h"
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
    static bool          methodsSet = false;

    if (!methodsSet) 
        {
        // this must be here so the parser can distinguish between different instances of a character matrix
        ncharArgRules.push_back(    new ReferenceRule( "", MemberObject_name ) );
        ntaxaArgRules.push_back(    new ReferenceRule( "", MemberObject_name ) );
        chartypeArgRules.push_back( new ReferenceRule( "", MemberObject_name ) );
        
        methods.addFunction("nchar",    new MemberFunction(Natural_name,  ncharArgRules));
        methods.addFunction("ntaxa",    new MemberFunction(Natural_name,  ntaxaArgRules));
        methods.addFunction("chartype", new MemberFunction(RbString_name, chartypeArgRules));
        
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



