/**
 * @file
 * This file contains the implementation of a rooted triplet distribution,
 * that holds a vector of triplets along with their frequencies.
 *
 * @brief Implementation of a rooted triplet distribution
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-07-05 16:47:08 +0200 (Thu, 05 Jul 2012) $
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-07-17, version 1.0
 *
 * $Id: RootedTripletDistribution.cpp 1651 2014-08-31 14:47:08Z Boussau $
 */

#include "RootedTripletDistribution.h"
#include "RbException.h"
#include "RbOptions.h"
#include "TreeChangeEventListener.h"
#include "Topology.h"
#include "TopologyNode.h"
#include "TimeTree.h"

using namespace RevBayesCore;

/* Default constructor */
RootedTripletDistribution::RootedTripletDistribution(void) : taxa(), species(), tripletDistribution(), numberOfTrees(0), taxaSet(), speciesSet(), taxonToIndex(), speciesToIndex(), speciesOnly() {
    
}


/* Copy constructor */
RootedTripletDistribution::RootedTripletDistribution(const RootedTripletDistribution& t)  {
    
    taxa = t.taxa;
    species = t.species;
    tripletDistribution = t.tripletDistribution;
    numberOfTrees = t.numberOfTrees;
    taxaSet = t.taxaSet;
    speciesSet = t.speciesSet;
    taxonToIndex = t.taxonToIndex;
    speciesToIndex = t.speciesToIndex;
    speciesOnly = t.speciesOnly;
}


/* Destructor */
RootedTripletDistribution::~RootedTripletDistribution(void) {
    
}


RootedTripletDistribution& RootedTripletDistribution::operator=(const RootedTripletDistribution &t) {
    
    if (this != &t) {
        taxa = t.taxa;
        species = t.species;
        tripletDistribution = t.tripletDistribution;
        numberOfTrees = t.numberOfTrees;
        taxaSet = t.taxaSet;
        speciesSet = t.speciesSet;
        taxonToIndex = t.taxonToIndex;
        speciesToIndex = t.speciesToIndex;
        speciesOnly = t.speciesOnly;
    }
    
    return *this;
}


/* Clone function */
RootedTripletDistribution* RootedTripletDistribution::clone(void) const {
    
    return new RootedTripletDistribution(*this);
}


void RootedTripletDistribution::extractTriplets( TimeTree& t ) {
    if (!t.isRooted() ) {
        throw(RbException("Tree is not rooted: cannot get rooted triplets."));
    }
    if (speciesOnly && taxa.size()!=0) {
        throw(RbException("Cannot add triplets of species to a triplet distribution on taxa."));
    }
    else if (!speciesOnly && species.size()!=0) {
        throw(RbException("Cannot add triplets of taxa to a triplet distribution on species."));
    }
    std::vector< size_t > allTips;
    
    populateTripletDistribution ( &(t.getRoot()), allTips );
    
}


void RootedTripletDistribution::extractTriplets( std::vector<TimeTree>& ts ) {
    for (size_t i = 0; i < ts.size(); ++i) {
        extractTriplets( ts[i] ) ;
    }
}


void RootedTripletDistribution::populateTripletDistribution ( TopologyNode* node, std::vector< size_t >& allTips ) {
    std::vector< size_t > leftTips;
    std::vector< size_t > rightTips;
    if (node->getNumberOfChildren() > 0 ) {
        //Assuming binary trees
        populateTripletDistribution( &( node->getChild(0) ), leftTips);
        populateTripletDistribution( &( node->getChild(1) ), rightTips);
        //Adding the triplets
        addAllTriplets( leftTips, rightTips );
        
        //filling allChildren
        for ( size_t i = 0; i<leftTips.size(); ++i ) {
            allTips.push_back(leftTips[i]);
        }
        for ( size_t i = 0; i<rightTips.size(); ++i ) {
            allTips.push_back(rightTips[i]);
        }
    }
    else {
        if ( speciesOnly ) {
            std::string sp = node->getSpeciesName();
            allTips.push_back (speciesToIndex[sp]);
        }
        else {
            Taxon t = node->getTaxon();
            allTips.push_back (taxonToIndex[t]);
        }
    }
    return;
}


void RootedTripletDistribution::addAllTriplets(std::vector< size_t >& leftTips, std::vector< size_t >& rightTips) {
    size_t rightSize = rightTips.size();
    size_t leftSize = leftTips.size();
    if ( leftSize + rightSize >= 3) { //there are triplets to add
        //One way
        addAllTripletsOneWay( leftTips, rightTips, leftSize, rightSize );
        //The other way
        addAllTripletsOneWay( rightTips, leftTips, rightSize, leftSize );
    }
    return;
}


void RootedTripletDistribution::addAllTripletsOneWay( std::vector< size_t >& leftTips,
                                                      std::vector< size_t >& rightTips,
                                                     size_t leftSize,
                                                     size_t rightSize) {
    for (size_t i = 0; i < leftSize; ++i) {
        for (size_t j = 0; j < rightSize - 1; ++j) {
            for (size_t k = j; k < rightSize; ++k) {
                std::pair < size_t, std::pair < size_t, size_t > > pairToAdd;
                if (rightTips[j] < rightTips[k])
                    pairToAdd = std::pair< size_t, std::pair<size_t, size_t> >(leftTips[i], std::pair<size_t, size_t>(rightTips[j], rightTips[k]) );
                else
                    pairToAdd = std::pair< size_t, std::pair<size_t, size_t> >(leftTips[i], std::pair<size_t, size_t>(rightTips[k], rightTips[j]) );
                std::map < std::pair < size_t, std::pair < size_t, size_t > >, size_t >::iterator it;
                it = tripletDistribution.find( pairToAdd ) ;
                if ( it == tripletDistribution.end() ) {
                    tripletDistribution[pairToAdd] = 1 ;
                }
                else {
                    it->second += 1;
                }
            }
        }
    }
    return;
}


size_t RootedTripletDistribution::getNumberOfTrees() const{
    return numberOfTrees;
}


size_t RootedTripletDistribution::getNumberOfTriplets() const{
    return tripletDistribution.size();
}


void RootedTripletDistribution::setSpecies ( std::vector< std::string > s)  {
    speciesOnly = true;
    species = s;
    return;
}


void RootedTripletDistribution::setTaxa ( std::vector< Taxon > t) {
    speciesOnly = false;
    taxa = t;
    return;
}

std::vector< std::string > RootedTripletDistribution::getSpecies ( ) const {
    return species;
}


std::vector< Taxon > RootedTripletDistribution::getTaxa ( ) const {
    return taxa;
}



std::ostream& RevBayesCore::operator<<(std::ostream& o, const RootedTripletDistribution& x) {
    o << x.getNumberOfTrees() <<" trees; " << x.getNumberOfTriplets() << " triplets.";
    
    return o;
}
