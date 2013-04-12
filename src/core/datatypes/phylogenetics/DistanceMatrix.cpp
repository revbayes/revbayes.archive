//
//  DistanceMatrix.cpp
//  RbGui
//
//  Created by Sebastian Hoehna on 4/12/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#include "DistanceMatrix.h"
#include "RbException.h"
#include "StringUtilities.h"

#include <cmath>
#include <string>

using namespace RevBayesCore;

/** Constructor requires character type; passes member rules to base class */
DistanceMatrix::DistanceMatrix(const size_t nTaxa) : MatrixReal( nTaxa, nTaxa ) {
    
}


/** Add the taxon with given name. */
void DistanceMatrix::addTaxonWithName(std::string s) {
    // @John: Not sure how this function should work (Sebastian)
    // Here is a possibility.
    
    // add the name to our names list
    sequenceNames.push_back( s );
    
    // resize the current matrix
    //elements.resize( elements.size() + 1 );
    
    
}


/** Exclude a taxon */
void DistanceMatrix::excludeTaxon(size_t i) {
    
    if (i >= sequenceNames.size()) 
    {
        std::stringstream o;
        o << "Only " << sequenceNames.size() << " taxa in matrix";
        throw RbException( o.str() );
    }
    deletedTaxa.insert( i );
}


/** Exclude a taxon */
void DistanceMatrix::excludeTaxon(std::string& s) {
    
    for (size_t i = 0; i < elements.size(); i++) 
    {
        if (s == sequenceNames[i] ) 
        {
            deletedTaxa.insert( i );
            break;
        }
    }
}


double DistanceMatrix::getDistance(size_t row, size_t col) const {
    
    double d = elements[row][col];
    return d;
}


size_t DistanceMatrix::getNumberOfTaxa(void) const {
    
    return sequenceNames.size();
}


/** Get taxon with index idx */
const std::string& DistanceMatrix::getTaxonNameWithIndex( size_t idx ) const {
    
    return sequenceNames[idx];
}


/** Return the index of the element ( the index of the taxon with name elemName ) */
size_t DistanceMatrix::indexOfTaxonWithName( const std::string& s ) const {
    
    // search through all names
    for (size_t i=0; i<sequenceNames.size(); i++) 
    {
        if (s == sequenceNames[ i ]) 
        {
            return i;
        }
    }
    return -1;
}


/** Is the taxon excluded */
bool DistanceMatrix::isTaxonExcluded(size_t i) const {
    
	std::set<size_t>::const_iterator it = deletedTaxa.find( i );
	if ( it != deletedTaxa.end() )
		return true;
    return false;
}


/** Is the taxon excluded */
bool DistanceMatrix::isTaxonExcluded(std::string& s) const {
    
    size_t i = indexOfTaxonWithName(s);
	std::set<size_t>::const_iterator it = deletedTaxa.find( i );
	if ( it != deletedTaxa.end() )
		return true;
    return false;
}


/** Restore a taxon */
void DistanceMatrix::restoreTaxon(size_t i) {
    
    if ( i >= getNumberOfTaxa() )
        return;
    deletedTaxa.erase( i );
}


/** Restore a taxon */
void DistanceMatrix::restoreTaxon(std::string& s) {
    
    size_t i = indexOfTaxonWithName( s );
    deletedTaxa.erase( i );
}


void DistanceMatrix::showData(void) {
    
}





