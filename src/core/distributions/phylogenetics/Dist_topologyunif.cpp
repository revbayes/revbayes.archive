/**
 * @file
 * This file contains the implementation of Dist_topologyunif, which holds
 * the parameters and functions related to a uniform distribution on
 * topologies (unrooted or rooted, binary or polytomous).
 *
 * @brief Implementation of Dist_topologyunif
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-14 12:43:32 +0100 (Mån, 14 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id: MemberObject.h 194 2009-12-14 11:43:32Z ronquist $
 */

#include "Boolean.h"
#include "DAGNode.h"
#include "Dist_topologyunif.h"
#include "Integer.h"
#include "Natural.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbNames.h"
#include "Simplex.h"
#include "Topology.h"
#include "TopologyNode.h"
#include "ValueRule.h"
#include "VectorString.h"

#include <cmath>
#include <sstream>
#include <vector>


/** Default constructor for parser use */
Dist_topologyunif::Dist_topologyunif( void ) : DistributionDiscrete( getMemberRules() ) {

    // Precalculate probability of topology
    // TODO: this will crash because we haven't set the parameter of the distribution
//    calculateTopologyProb();
}


/** Constructor for internal use */
Dist_topologyunif::Dist_topologyunif( int numTaxa, bool isRooted, bool isBinary ) : DistributionDiscrete( getMemberRules() ) {

    setValue( "numberTaxa", new Natural( numTaxa ) );
    setValue( "isRooted"  , new Boolean( isRooted ) );
    setValue( "isBinary"  , new Boolean( isBinary ) );

    // Precalculate probability of topology
    calculateTopologyProb();
}


void Dist_topologyunif::buildRandomBinaryTree(std::vector<TopologyNode *> &tips, int numTaxa) {
    
    if (tips.size() < numTaxa) {
        // Get the rng
        RandomNumberGenerator* rng = GLOBAL_RNG;
        
        // randomly draw one node from the list of tips
        size_t index = floor(rng->uniform01()*tips.size());
        
        // get the node from the list
        TopologyNode* parent = tips.at(index);
        
        // remove the randomly drawn node from the list
        tips.erase(tips.begin()+index);
        
        // add a left child
        TopologyNode *leftChild = new TopologyNode(0);
        parent->addChild(leftChild);
        tips.push_back(leftChild);
        
        // add a right child
        TopologyNode *rightChild = new TopologyNode(0);
        parent->addChild(rightChild);
        tips.push_back(rightChild);
        
        // recursive call to this function
        buildRandomBinaryTree(tips, numTaxa);
    }
}



/** Precalculate the probability and log probability of a topology drawn from the distribution. */
void Dist_topologyunif::calculateTopologyProb( void ) {
    
    // Get the parameters
    int             numTaxa  = static_cast<const Natural*>( getValue( "numberTaxa"  ) )->getValue();
    bool            isRooted = static_cast<const Boolean*>( getValue( "isRooted"    ) )->getValue();
    bool            isBinary = static_cast<const Boolean*>( getValue( "isBinary"    ) )->getValue();
    
    // Calculate probabilities of a topology drawn from the distribution
    if (isBinary) {
        // we have a binary topology
        if (isRooted) {
            // set the topology probability to default values
            lnTopologyProb = 0.0;
            topologyProb = 1.0;
            for (int i=2; i<=numTaxa; i++) {
                double tmp = 1.0/(2.0*i - 3.0);
                lnTopologyProb += log(tmp);
                topologyProb *= tmp;
            }
        } else {
            
            // set the topology probability to default values
            lnTopologyProb = 0.0;
            topologyProb = 1.0;
            for (int i=3; i<=numTaxa; i++) {
                double tmp = 1.0/(2.0*i - 5.0);
                lnTopologyProb += log(tmp);
                topologyProb *= tmp;
            }
        }
    }
}

/** Precalculate the probability and log probability of a topology drawn from the distribution. */
void Dist_topologyunif::calculateNumberOfStates( void ) {
    
    // Get the parameters
    int             numTaxa  = static_cast<const Natural*>( getValue( "numberTaxa"  ) )->getValue();
    bool            isRooted = static_cast<const Boolean*>( getValue( "isRooted"    ) )->getValue();
    bool            isBinary = static_cast<const Boolean*>( getValue( "isBinary"    ) )->getValue();
    
    // Calculate probabilities of a topology drawn from the distribution
    if (isBinary) {
        // we have a binary topology
        if (isRooted) {
            // set the number of states to default values
            numberOfStates = 1;
            for (size_t i=2; i<=numTaxa; i++) {
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
            for (size_t i=3; i<=numTaxa; i++) {
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


/** Get class vector showing type of object */
const VectorString& Dist_topologyunif::getClass( void ) const {

    static VectorString rbClass = VectorString( Dist_topologyunif_name ) + DistributionDiscrete::getClass();
    return rbClass;
}


/** Get default move */
Move* Dist_topologyunif::getDefaultMove( StochasticNode* node ) {

	// TODO: Add a default move
    return NULL;
}


/** Get member variable rules */
const MemberRules& Dist_topologyunif::getMemberRules( void ) const {

    static MemberRules memberRules;
    static bool        rulesSet = false;

    if ( !rulesSet )
		{
        memberRules.push_back( new ValueRule( "numberTaxa" , Natural_name      ) );
        memberRules.push_back( new ValueRule( "tipNames"   , VectorString_name ) );
        memberRules.push_back( new ValueRule( "isRooted"   , Boolean_name      ) );
        memberRules.push_back( new ValueRule( "isBinary"   , new Boolean(true) ) );

        rulesSet = true;
		}

    return memberRules;
}


/** Get the number of states in the distribution */
size_t Dist_topologyunif::getNumStates( void ) const {

    return numberOfStates;
}


/** Get the probability mass vector */
const Simplex* Dist_topologyunif::getProbabilityMassVector( void ) {

    int             numTaxa  = static_cast<const Natural*>( getValue( "numberTaxa"  ) )->getValue();

    if ( numTaxa <= 6 )
        return new Simplex( getNumStates() );
    else
        throw RbException( "Too many topologies to generate probability mass vector" );
}


/** Get random variable type */
const TypeSpec Dist_topologyunif::getVariableType( void ) const {

    return TypeSpec( Topology_name );
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
double Dist_topologyunif::lnPdf( const RbObject* value ) {

    // Get the parameters
    const Topology* top      = static_cast<const Topology*    >( value );
    int             numTaxa  = static_cast<const Natural*>( getValue( "numberTaxa"  ) )->getValue();
    bool            isRooted = static_cast<const Boolean*>( getValue( "isRooted"    ) )->getValue();
    bool            isBinary = static_cast<const Boolean*>( getValue( "isBinary"    ) )->getValue();

    // If numTaxa, isRooted and isBinary fits top, then return precalculated probability stored in the class
    // Otherwise return negative infinity
    if ( top->getNumberOfNodes() == numTaxa && top->getIsRooted() == isRooted && top->getIsBinary() == isBinary )
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
double Dist_topologyunif::pdf( const RbObject* value ) {

    // Get the parameters
    const Topology* top      = static_cast<const Topology*    >( value );
    int             numTaxa  = static_cast<const Natural*>( getValue( "numberTaxa"  ) )->getValue();
    bool            isRooted = static_cast<const Boolean*>( getValue( "isRooted"    ) )->getValue();
    bool            isBinary = static_cast<const Boolean*>( getValue( "isBinary"    ) )->getValue();

    // If numTaxa, isRooted and isBinary fits top, then return precalculated probability stored in the class
    // Otherwise return 0.0
    if ( top->getNumberOfNodes() == numTaxa && top->getIsRooted() == isRooted && top->getIsBinary() == isBinary )
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
Topology* Dist_topologyunif::rv( void ) {

    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // Get the parameters
    int                 numTaxa  = static_cast<const Natural*>( getValue( "numberTaxa"  ) )->getValue();
    bool                isRooted = static_cast<const Boolean*>( getValue( "isRooted"    ) )->getValue();
    bool                isBinary = static_cast<const Boolean*>( getValue( "isBinary"    ) )->getValue();
    const VectorString  *names   = static_cast<const VectorString*>( getValue( "tipNames"    ) );

    // Draw a random topology
    if (isBinary) {
        Topology *top = new Topology();
        // internally we treat unrooted topologies the same as rooted
        top->setIsRooted(isRooted);
        
        TopologyNode *root = new TopologyNode((int)pow(2,numTaxa)-1);
        std::vector<TopologyNode*> nodes;
        nodes.push_back(root);
        // recursively build the tree
        buildRandomBinaryTree(nodes, numTaxa);
        
        // set tip names
        for (size_t i=0; i<numTaxa; i++) {
            size_t index = floor(rng->uniform01() * nodes.size());
            
            // get the node from the list
            TopologyNode* node = nodes.at(index);
            
            // remove the randomly drawn node from the list
            nodes.erase(nodes.begin()+index);
            
            // set name
            std::string name = (*names)[i];
            node->setName(name);
        }
        
        // initialize the topology by setting the root
        top->setRoot(root);
        
        return top;
    }
    // TODO: Draw a random multifurcating topology
    return NULL;
}


/** We intercept a call to set a member variable to make sure that the topology prob gets recalculated */
void Dist_topologyunif::setVariable( const std::string& name, DAGNode* var ) {

    DistributionDiscrete::setVariable( name, var );

    // TODO: recaluclation of the probability crashes if not all variable are already set!!!
//    calculateTopologyProb();
}

