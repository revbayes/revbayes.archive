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

#include "BranchLengthTree.h"
#include "RootedTripletDistribution.h"
#include "RbException.h"
#include "RbOptions.h"
#include "TreeChangeEventListener.h"
#include "Topology.h"
#include "TopologyNode.h"
#include "Tree.h"
#include "TimeTree.h"


using namespace RevBayesCore;

/* Default constructor */
RootedTripletDistribution::RootedTripletDistribution(void) : ttrees( NULL ), bltrees( NULL ), taxa(), species(), tripletDistribution(), numberOfTrees(0),  taxonToIndex(), speciesToIndex(), speciesOnly() {
}


RootedTripletDistribution::RootedTripletDistribution( const std::vector<TimeTree>& ts, const std::vector< std::string > spNames, bool recordBls ) : ttrees(ts), bltrees( NULL ), taxa(), species(spNames), tripletDistribution(), numberOfTrees(ts.size()), taxonToIndex(), speciesToIndex(), speciesOnly( true ), recordBranchLengths( recordBls ) {
//    std::sort ( spNames.begin(), spNames.end() );
    for (size_t i = 0; i< spNames.size(); ++i) {
        speciesToIndex[spNames[i]] = i;
    }
    extractTriplets( );

}


RootedTripletDistribution::RootedTripletDistribution( const std::vector<BranchLengthTree>& ts, const std::vector< std::string > spNames, bool recordBls ) : ttrees( NULL ), bltrees(ts), taxa(), species(spNames), tripletDistribution(), numberOfTrees(ts.size()), taxonToIndex(), speciesToIndex(), speciesOnly( true ), recordBranchLengths( recordBls ) {
//    std::sort ( spNames.begin(), spNames.end() );
    for (size_t i = 0; i< spNames.size(); ++i) {
        speciesToIndex[spNames[i]] = i;
    }
    extractTriplets( );

}


RootedTripletDistribution::RootedTripletDistribution( const std::vector< std::string > spNames ): ttrees( NULL ), bltrees( NULL ), taxa(), species(spNames), tripletDistribution(), numberOfTrees(0), taxonToIndex(), speciesToIndex(), speciesOnly( true ), recordBranchLengths( false ) {
//    std::sort ( spNames.begin(), spNames.end() );
    for (size_t i = 0; i< spNames.size(); ++i) {
        speciesToIndex[spNames[i]] = i;
    }

}

RootedTripletDistribution::RootedTripletDistribution( const std::vector<TimeTree>& ts, const std::vector< Taxon > tax, bool recordBls ) : ttrees(ts), bltrees( NULL ), taxa( tax ), species(), tripletDistribution(), numberOfTrees(ts.size()), taxonToIndex(), speciesToIndex(), speciesOnly( false ), recordBranchLengths( recordBls ) {
 //   std::sort ( taxa.begin(), taxa.end() );
    for (size_t i = 0; i< taxa.size(); ++i) {
        taxonToIndex[taxa[i]] = i;
    }
    extractTriplets( );

}


RootedTripletDistribution::RootedTripletDistribution( const std::vector<BranchLengthTree>& ts, const std::vector< Taxon > tax, bool recordBls ) : ttrees( NULL ), bltrees(ts), taxa( tax ), species(), tripletDistribution(), numberOfTrees(ts.size()), taxonToIndex(), speciesToIndex(), speciesOnly( false ), recordBranchLengths( recordBls ) {
   // std::sort ( taxa.begin(), taxa.end() );
    for (size_t i = 0; i< taxa.size(); ++i) {
        taxonToIndex[taxa[i]] = i;
    }
    extractTriplets( );

}


RootedTripletDistribution::RootedTripletDistribution( const std::vector< Taxon > tax ): ttrees( NULL ), bltrees( NULL ), taxa( tax ), species( ), tripletDistribution(), numberOfTrees(0), taxonToIndex(), speciesToIndex(), speciesOnly( false ), recordBranchLengths( false ) {
  //  std::sort ( taxa.begin(), taxa.end() );
    for (size_t i = 0; i< taxa.size(); ++i) {
        taxonToIndex[taxa[i]] = i;
    }

}


/* Copy constructor */
RootedTripletDistribution::RootedTripletDistribution(const RootedTripletDistribution& t)  {
    
    ttrees = t.ttrees;
    bltrees = t.bltrees;
    taxa = t.taxa;
    species = t.species;
    tripletDistribution = t.tripletDistribution;
    numberOfTrees = t.numberOfTrees;
    taxonToIndex = t.taxonToIndex;
    speciesToIndex = t.speciesToIndex;
    speciesOnly = t.speciesOnly;
    tripletDistributionAndBranchLength = t.tripletDistributionAndBranchLength;
    recordBranchLengths = t.recordBranchLengths;
}


/* Destructor */
RootedTripletDistribution::~RootedTripletDistribution(void) {
    
}


RootedTripletDistribution& RootedTripletDistribution::operator=(const RootedTripletDistribution &t) {
    
    if (this != &t) {
        ttrees = t.ttrees;
        bltrees = t.bltrees;
        taxa = t.taxa;
        species = t.species;
        tripletDistribution = t.tripletDistribution;
        numberOfTrees = t.numberOfTrees;
        taxonToIndex = t.taxonToIndex;
        speciesToIndex = t.speciesToIndex;
        speciesOnly = t.speciesOnly;
        tripletDistributionAndBranchLength = t.tripletDistributionAndBranchLength;
        recordBranchLengths = t.recordBranchLengths;
    }
    
    return *this;
}


/* Clone function */
RootedTripletDistribution* RootedTripletDistribution::clone(void) const {
    
    return new RootedTripletDistribution(*this);
}


void RootedTripletDistribution::executeMethod(const std::string &n, const std::vector<const DagNode *> &args, std::vector < std::string > &rv) const
{
    
    if ( n == "species" )
    {
        rv = getSpecies();
    }
    else
    {
        throw RbException("A tree object does not have a member method called '" + n + "'.");
    }
    
}


void RootedTripletDistribution::extractTriplets( const Tree& t ) {
    if (!t.isRooted() ) {
        throw(RbException("Tree is not rooted: cannot get rooted triplets."));
    }
  /*  if (speciesOnly) std::cout << "speciesOnly" <<std::endl;
    std::cout << "species.size(): " << species.size() << std::endl;
    std::cout << "taxa.size(): " << taxa.size() << std::endl;
    std::cout << "ttrees.size(): " << ttrees.size() << std::endl;
    std::cout << "tree: " << t << std::endl;*/

    if (speciesOnly && species.size()==0) {
        throw(RbException("Cannot add triplets of species to a triplet distribution on taxa."));
    }
    else if (!speciesOnly && taxa.size()==0) {
        throw(RbException("Cannot add triplets of taxa to a triplet distribution on species."));
    }
    std::vector< size_t > allTips;
    if (!recordBranchLengths) {
        populateTripletDistribution ( &(t.getRoot()), allTips );
    }
    else {
        std::vector< double > distancesToTips;
        std::map < std::pair < size_t, size_t >, double > distancesToAncestors; // contains distances between most ancient ancestor and most recent ancestor of a given pair of leaves
        populateTripletDistribution ( &(t.getRoot()), allTips, distancesToTips, distancesToAncestors );
    }
    
}


void RootedTripletDistribution::extractTriplets(  ) {
    if (ttrees.size() != 0 ) {
        for (size_t i = 0; i < ttrees.size(); ++i) {
            extractTriplets( ttrees[i] ) ;
        }
    }
    else if (bltrees.size() != 0 ) {
        for (size_t i = 0; i < bltrees.size(); ++i) {
            extractTriplets( bltrees[i] ) ;
        }

    }
}


void RootedTripletDistribution::populateTripletDistribution ( const TopologyNode* node, std::vector< size_t >& allTips ) {
    std::vector< size_t > leftTips;
    std::vector< size_t > rightTips;
    if ( node->getNumberOfChildren() > 0 ) {
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
            allTips.push_back (speciesToIndex.at(sp));
        }
        else {
            Taxon t = node->getTaxon();
            allTips.push_back (taxonToIndex.at(t));
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
    bool toAdd = false;
    for (size_t i = 0; i < leftSize; ++i) {
        for (size_t j = 0; j < rightSize - 1; ++j) {
            for (size_t k = j; k < rightSize; ++k) {
                std::pair < size_t, std::pair < size_t, size_t > > pairToAdd;
                toAdd = false;
                if (rightTips[j] < rightTips[k]) {
                    toAdd = true;
                    pairToAdd = std::pair< size_t, std::pair<size_t, size_t> >(leftTips[i], std::pair<size_t, size_t>(rightTips[j], rightTips[k]) );
                }
                else if (rightTips[j] > rightTips[k]) {
                    toAdd = true;
                    pairToAdd = std::pair< size_t, std::pair<size_t, size_t> >(leftTips[i], std::pair<size_t, size_t>(rightTips[k], rightTips[j]) );
                }
                if (toAdd) {
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
    }
    return;
}








void RootedTripletDistribution::populateTripletDistribution ( const TopologyNode* node, std::vector< size_t >& allTips, std::vector< double >& distancesToTips, std::map < std::pair < size_t, size_t >, double >& distancesToAncestors ) {
    std::vector< size_t > leftTips;
    std::vector< size_t > rightTips;
    std::vector< double > leftDistancesToTips;
    std::vector< double > rightDistancesToTips;
    std::map < std::pair < size_t, size_t >, double > leftDistancesToAncestors; // contains distances between most ancient ancestor and most recent ancestor of a given pair of leaves
    std::map < std::pair < size_t, size_t >, double > rightDistancesToAncestors; // contains distances between most ancient ancestor and most recent ancestor of a given pair of leaves

    if ( node->getNumberOfChildren() > 0 ) {
        //Assuming binary trees
        populateTripletDistribution( &( node->getChild(0) ), leftTips, leftDistancesToTips, leftDistancesToAncestors);
        populateTripletDistribution( &( node->getChild(1) ), rightTips, rightDistancesToTips, rightDistancesToAncestors);
        
        double leftDist = node->getChild(0).getBranchLength();
        double rightDist = node->getChild(1).getBranchLength();
        for ( std::map < std::pair < size_t, size_t >, double >::iterator it = rightDistancesToAncestors.begin(); it != rightDistancesToAncestors.end(); ++it ) {
            it->second += rightDist;
            distancesToAncestors[it->first] = it->second;
        }
        for ( std::map < std::pair < size_t, size_t >, double >::iterator it = leftDistancesToAncestors.begin(); it != leftDistancesToAncestors.end(); ++it ) {
            it->second += leftDist;
            distancesToAncestors[it->first] = it->second;
        }
        for ( size_t i = 0; i<leftTips.size(); ++i ) {
            leftDistancesToTips[i] += leftDist;
            distancesToTips.push_back(leftDistancesToTips[i]);
        }
        for ( size_t i = 0; i<rightTips.size(); ++i ) {
            rightDistancesToTips[i] += rightDist;
            distancesToTips.push_back(rightDistancesToTips[i]);
        }
        
        //Adding all the pairs that appear at this level
        for ( size_t i = 0; i<leftTips.size(); ++i ) {
            for ( size_t j = 0; j<rightTips.size(); ++j ) {
                if (leftTips[i]<rightTips[j]) {
                    distancesToAncestors[std::pair<size_t, size_t>(leftTips[i], rightTips[j])] = 0.0;
                }
                else if (rightTips[j]<leftTips[i]) {
                    distancesToAncestors[std::pair<size_t, size_t>(rightTips[j], leftTips[i])] = 0.0;
                }
            }
        }
        
        //filling allChildren
        for ( size_t i = 0; i<leftTips.size(); ++i ) {
            allTips.push_back(leftTips[i]);
        }
        for ( size_t i = 0; i<rightTips.size(); ++i ) {
            allTips.push_back(rightTips[i]);
        }

        
        //Adding the triplets
        addAllTriplets( leftTips, rightTips, leftDistancesToTips, rightDistancesToTips, distancesToAncestors );
        
    }
    else { // at a leaf
        if ( speciesOnly ) {
            std::string sp = node->getSpeciesName();
            allTips.push_back (speciesToIndex.at(sp));
            distancesToTips.push_back(0.0);
        }
        else {
            Taxon t = node->getTaxon();
            allTips.push_back (taxonToIndex.at(t));
            distancesToTips.push_back(0.0);
        }
    }
    return;
}


void RootedTripletDistribution::addAllTriplets(std::vector< size_t >& leftTips, std::vector< size_t >& rightTips, std::vector< double >& leftDistancesToTips, std::vector< double >& rightDistancesToTips, std::map < std::pair < size_t, size_t >, double >& distancesToAncestors ) {
    size_t rightSize = rightTips.size();
    size_t leftSize = leftTips.size();
    if ( leftSize + rightSize >= 3) { //there are triplets to add
        //One way
        addAllTripletsOneWay( leftTips, rightTips, leftSize, rightSize, leftDistancesToTips, rightDistancesToTips, distancesToAncestors );
        //The other way
        addAllTripletsOneWay( rightTips, leftTips, rightSize, leftSize, rightDistancesToTips, leftDistancesToTips, distancesToAncestors );
    }
    return;
}


void RootedTripletDistribution::addAllTripletsOneWay( std::vector< size_t >& leftTips,
                                                     std::vector< size_t >& rightTips,
                                                     size_t leftSize,
                                                     size_t rightSize,
                                                     std::vector<double>& leftDistancesToTips,
                                                     std::vector<double>& rightDistancesToTips,
                                                     std::map < std::pair < size_t, size_t >, double >& distancesToAncestors ) {
   
    bool toAdd = false;
    for (size_t i = 0; i < leftSize; ++i) {
        for (size_t j = 0; j < rightSize - 1; ++j) {
            for (size_t k = j; k < rightSize; ++k) {
                std::pair < size_t, std::pair < size_t, size_t > > pairToAdd;
                toAdd = false;
                if (rightTips[j] < rightTips[k]) {
                    toAdd = true;
                    pairToAdd = std::pair< size_t, std::pair<size_t, size_t> >(leftTips[i], std::pair<size_t, size_t>(rightTips[j], rightTips[k]) );
                }
                else if (rightTips[j] > rightTips[k]) {
                    toAdd = true;
                    pairToAdd = std::pair< size_t, std::pair<size_t, size_t> >(leftTips[i], std::pair<size_t, size_t>(rightTips[k], rightTips[j]) );
                }
                if (toAdd) {
                    std::map < std::pair < size_t, std::pair < size_t, size_t > >, size_t >::iterator it;
                    it = tripletDistribution.find( pairToAdd ) ;
                    if ( it == tripletDistribution.end() ) {
                        tripletDistribution[pairToAdd] = 1 ;
                        tripletDistributionAndBranchLength[pairToAdd] = std::vector<double> ();
                        tripletDistributionAndBranchLength.at(pairToAdd).push_back(distancesToAncestors.at(pairToAdd.second));
                    }
                    else {
                        it->second += 1;
                        tripletDistributionAndBranchLength.at(pairToAdd).push_back(distancesToAncestors.at(pairToAdd.second));
                    }
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


void RootedTripletDistribution::setTrees( const std::vector< TimeTree >& ts ) {
    
    ttrees = ts;
    bltrees.clear() ;
    numberOfTrees = ttrees.size();
    
}

void RootedTripletDistribution::setTrees( const std::vector< BranchLengthTree >& ts ) {
    
    ttrees.clear() ;
    bltrees = ts;
    numberOfTrees = bltrees.size();

}


void RootedTripletDistribution::setRecordBranchLengths ( const bool b ) {
    recordBranchLengths = b;
}


std::vector< std::string > RootedTripletDistribution::getSpecies ( ) const {
    return species;
}


std::vector< Taxon > RootedTripletDistribution::getTaxa ( ) const {
    return taxa;
}

std::string RootedTripletDistribution::getSpecies ( size_t i ) const {
    return species[i];
}


Taxon RootedTripletDistribution::getTaxon ( size_t i ) const {
    return taxa[i];
}


void RootedTripletDistribution::resetDistribution (  ) {
    tripletDistribution.clear();
    tripletDistributionAndBranchLength.clear();
    return ;
}




const std::string RootedTripletDistribution::getStringRepresentation() const {
    // create the newick string
    std::stringstream o;
    std::fixed(o);
    o.precision( 6 );

    o << getNumberOfTrees() <<" trees; " << getNumberOfTriplets() << " triplets." <<std::endl;
    o << "Triplets: " <<std::endl;
    if (speciesOnly) {
        if (recordBranchLengths) {
            for (std::map < std::pair < size_t, std::pair < size_t, size_t > >, size_t >::const_iterator it = tripletDistribution.begin(); it != tripletDistribution.end(); ++it) {
                std::vector<double> bls = tripletDistributionAndBranchLength.at(it->first);
                for ( size_t j = 0; j < bls.size(); ++j ) {
                    o << "(" << getSpecies(it->first.first) << " , (" <<  getSpecies(it->first.second.first) << " , " << getSpecies(it->first.second.second) << " ):"<< bls[j] <<" ); ";
                }
                o << "Weight: "<<  it->second << std::endl;
            }
        }
        else {
            for (std::map < std::pair < size_t, std::pair < size_t, size_t > >, size_t >::const_iterator it = tripletDistribution.begin(); it != tripletDistribution.end(); ++it) {
                o << "(" << getSpecies(it->first.first) << " , (" <<  getSpecies(it->first.second.first) << " , " << getSpecies(it->first.second.second) << " ) ); Weight: "<<  it->second << std::endl;
            }
        }
    }
    else {
        if (recordBranchLengths) {
            for (std::map < std::pair < size_t, std::pair < size_t, size_t > >, size_t >::const_iterator it = tripletDistribution.begin(); it != tripletDistribution.end(); ++it) {
                std::vector<double> bls = tripletDistributionAndBranchLength.at(it->first);
                for ( size_t j = 0; j < bls.size(); ++j ) {
                    o << "(" << getTaxon(it->first.first) << " , (" <<  getTaxon(it->first.second.first) << " , " << getTaxon(it->first.second.second) << " ):"<< bls[j] <<" ); ";
                }
                o << "Weight: "<<  it->second << std::endl;
            }
        }
        else {
            for (std::map < std::pair < size_t, std::pair < size_t, size_t > >, size_t >::const_iterator it = tripletDistribution.begin(); it != tripletDistribution.end(); ++it) {
                o << "(" << getTaxon(it->first.first) << " , (" <<  getTaxon(it->first.second.first) << " , " << getTaxon(it->first.second.second) << " ) ); Weight: "<<  it->second << std::endl;
            }
        }
    }
    
    return o.str();
}



std::ostream& RevBayesCore::operator<<(std::ostream& o, const RootedTripletDistribution& x) {
    o << x.getStringRepresentation() << std::endl;
    return o;
}
