/**
 * @file
 * This file contains the implementation of Dist_topologyunif, which holds
 * the parameters and functions related to a uniform distribution on
 * topologies (unrooted or rooted, binary or polytomous).
 *
 * @brief Implementation of Dist_topologyunif
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id$
 */

#include "Dist_topologyunif.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "Topology.h"
#include "TopologyNode.h"

#include <cmath>
#include <sstream>
#include <vector>


/** Default constructor for parser use */
Dist_topologyunif::Dist_topologyunif( void ) : DistributionDiscrete(),
                                                nTaxa(),
                                                rooted(),
                                                binary(),
                                                names() {

    // Precalculate probability of topology
    // \TODO: this will crash because we haven't set the parameter of the distribution
//    calculateTopologyProb();
}


void Dist_topologyunif::buildRandomBinaryTree(std::vector<TopologyNode*> &tips, size_t numTaxa) {
    
    if (tips.size() < numTaxa) {
        // Get the rng
        RandomNumberGenerator* rng = GLOBAL_RNG;
        
        // randomly draw one node from the list of tips
        size_t index = static_cast<size_t>( floor(rng->uniform01()*tips.size()) );
        
        // get the node from the list
        TopologyNode* parent = tips.at(index);
        
        // remove the randomly drawn node from the list
        tips.erase(tips.begin()+index);
        
        // add a left child
        TopologyNode* leftChild = new TopologyNode(0);
        parent->addChild(leftChild);
        leftChild->setParent(parent);
        tips.push_back(leftChild);
        
        // add a right child
        TopologyNode* rightChild = new TopologyNode(0);
        parent->addChild(rightChild);
        rightChild->setParent(parent);
        tips.push_back(rightChild);
        
        // recursive call to this function
        buildRandomBinaryTree(tips, numTaxa);
    }
}



/** Precalculate the probability and log probability of a topology drawn from the distribution. */
void Dist_topologyunif::calculateTopologyProb( void ) {
    
    // Calculate probabilities of a topology drawn from the distribution
    if (binary.value[0]) {
        // we have a binary topology
        if (rooted.value[0]) {
            // set the topology probability to default values
            lnTopologyProb = 0.0;
            topologyProb = 1.0;
            for (int i=2; i<=nTaxa.value[0]; i++) {
                double tmp = 1.0/(2.0*i - 3.0);
                lnTopologyProb += log(tmp);
                topologyProb *= tmp;
            }
        } else {
            
            // set the topology probability to default values
            lnTopologyProb = 0.0;
            topologyProb = 1.0;
            for (int i=3; i<=nTaxa.value[0]; i++) {
                double tmp = 1.0/(2.0*i - 5.0);
                lnTopologyProb += log(tmp);
                topologyProb *= tmp;
            }
        }
    }
}

/** Precalculate the probability and log probability of a topology drawn from the distribution. */
void Dist_topologyunif::calculateNumberOfStates( void ) {
    
    // Calculate probabilities of a topology drawn from the distribution
    if (binary.value[0]) {
        // we have a binary topology
        if (rooted.value[0]) {
            // set the number of states to default values
            numberOfStates = 1;
            for (int i=2; i<=nTaxa.value[0]; i++) {
                size_t tmp = (2*i - 3);
                if (RbConstants::Size_t::max/numberOfStates < tmp) {
                    numberOfStates = RbConstants::Size_t::inf;
                    break;
                }
                numberOfStates *= tmp;
            }
        } else {
            // set the number of states to default values
            numberOfStates = 1;
            for (int i=3; i<=nTaxa.value[0]; i++) {
                size_t tmp = (2*i - 5);
                if (RbConstants::Size_t::max/numberOfStates < tmp) {
                    numberOfStates = RbConstants::Size_t::inf;
                    break;
                }
                numberOfStates *= tmp;
            }
        }
    }
}


/** Clone this object */
Dist_topologyunif* Dist_topologyunif::clone( void ) const {

    return new Dist_topologyunif( *this );
}


/** Get the number of states in the distribution */
size_t Dist_topologyunif::getNumberOfStates( void ) const {

    return numberOfStates;
}


/**
 * This function calculates the natural log of the
 * probability of a random topology variable.
 *
 * @brief Natural log of probability of topology
 *
 * @param value Observed topology
 * @return      Natural log of the probability
 */
double Dist_topologyunif::lnPdfSingleValue( std::vector<size_t> &offset ) const {

    // Get the parameters
    int  n  = nTaxa.value[offset[0]];
    bool r = rooted.value[offset[2]];
    bool b = binary.value[offset[3]];
    const Topology& top = topology.value[offset[4]];

    // If numTaxa, isRooted and isBinary fits top, then return precalculated probability stored in the class
    // Otherwise return negative infinity
    if ( top.getNumberOfNodes() == n && top.getIsRooted() == r && top.getIsBinary() == b )
        return lnTopologyProb;
    else
        return RbConstants::Double::neginf;
}


/**
 * This function calculates the probability
 * of a categorical random variable.
 *
 * @brief Categorical probability density
 *
 * @param value Observed value
 * @return      Probability density
 */
double Dist_topologyunif::pdfSingleValue( std::vector<size_t> &offset ) const {

    // Get the parameters
    // Get the parameters
    int  n  = nTaxa.value[offset[0]];
    bool r = rooted.value[offset[2]];
    bool b = binary.value[offset[3]];
    const Topology& top = topology.value[offset[4]];

    // If numTaxa, isRooted and isBinary fits top, then return precalculated probability stored in the class
    // Otherwise return 0.0
    if ( top.getNumberOfNodes() == n && top.getIsRooted() == r && top.getIsBinary() == b )
        return topologyProb;
    else
        return 0.0;
}


/**
 * This function generates a topology from the distribution
 *
 * @brief Random draw from distribution
 *
 * @return      Randomly drawn topology
 */
void Dist_topologyunif::rvSingleValue( std::vector<size_t> &offset ) {

    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // Get the parameters
    // Get the parameters
    int  n = nTaxa.value[offset[0]];
    std::vector<std::string>& l = names.value[offset[1]];
    bool r = rooted.value[offset[2]];
    bool b = binary.value[offset[3]];

    // Draw a random topology
    if (b) {
        Topology randomVariable = Topology();
        // internally we treat unrooted topologies the same as rooted
        randomVariable.setIsRooted(r);
        
        TopologyNode* root = new TopologyNode((int)pow(2.0,n)-1);
        std::vector<TopologyNode* > nodes;
        nodes.push_back(root);
        // recursively build the tree
        buildRandomBinaryTree(nodes, n);
        
        // set tip names
        for (int i=0; i<n; i++) {
            size_t index = size_t( floor(rng->uniform01() * nodes.size()) );
            
            // get the node from the list
            TopologyNode* node = nodes.at(index);
            
            // remove the randomly drawn node from the list
            nodes.erase(nodes.begin()+index);
            
            // set name
            std::string& name = l[i];
            node->setName(name);
        }
        
        // initialize the topology by setting the root
        randomVariable.setRoot(root);
        
        topology.value[offset[4]] = randomVariable;
    }
    
}


/** We catch here the setting of the member variables to store our parameters. */
void Dist_topologyunif::setInternalObservedValue(const RbValue<void *> &v) {
    
    topology.value    = static_cast<Topology*>( v.value );
    topology.lengths  = v.lengths;
    
}


/** We catch here the setting of the member variables to store our parameters. */
void Dist_topologyunif::setInternalParameters(const std::vector<RbValue<void *> > &p) {
    
    nTaxa.value         = static_cast<int *>( p[0].value );
    nTaxa.lengths       = p[0].lengths;
    
    names.value         = static_cast<std::vector<std::string>*>( p[1].value );
    names.lengths       = p[1].lengths;
    
    rooted.value        = static_cast<bool*>( p[2].value );
    rooted.lengths      = p[2].lengths;
    
    binary.value        = static_cast<bool*>( p[3].value );
    binary.lengths      = p[3].lengths;
    
    topology.value      = static_cast<Topology*>( p[4].value );
    topology.lengths    = p[4].lengths;
    
}

