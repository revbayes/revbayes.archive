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

#include "RbBoolean.h"
#include "DAGNode.h"
#include "Dist_topologyunif.h"
#include "Integer.h"
#include "Natural.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbUtil.h"
#include "Simplex.h"
#include "Topology.h"
#include "TopologyNode.h"
#include "ValueRule.h"
#include "VectorString.h"

#include <cmath>
#include <sstream>
#include <vector>


// Definition of the static type spec member
const TypeSpec Dist_topologyunif::typeSpec(Dist_topologyunif_name);
const TypeSpec Dist_topologyunif::varTypeSpec(Topology_name);

/** Default constructor for parser use */
Dist_topologyunif::Dist_topologyunif( void ) : DistributionDiscrete( getMemberRules() ) {

    // Precalculate probability of topology
    // TODO: this will crash because we haven't set the parameter of the distribution
//    calculateTopologyProb();
}


void Dist_topologyunif::buildRandomBinaryTree(std::vector<RbPtr<TopologyNode> > &tips, size_t numTaxa) {
    
    if (tips.size() < numTaxa) {
        // Get the rng
        RbPtr<RandomNumberGenerator> rng = GLOBAL_RNG;
        
        // randomly draw one node from the list of tips
        size_t index = static_cast<size_t>( floor(rng->uniform01()*tips.size()) );
        
        // get the node from the list
        RbPtr<TopologyNode> parent = tips.at(index);
        
        // remove the randomly drawn node from the list
        tips.erase(tips.begin()+index);
        
        // add a left child
        RbPtr<TopologyNode> leftChild( new TopologyNode(0) );
        parent->addChild(leftChild);
        leftChild->setParent(parent);
        tips.push_back(leftChild);
        
        // add a right child
        RbPtr<TopologyNode> rightChild( new TopologyNode(0) );
        parent->addChild(rightChild);
        rightChild->setParent(parent);
        tips.push_back(rightChild);
        
        // recursive call to this function
        buildRandomBinaryTree(tips, numTaxa);
    }
}



/** Precalculate the probability and log probability of a topology drawn from the distribution. */
void Dist_topologyunif::calculateTopologyProb( void ) {
    
    // Get the parameters
    int             numTaxa  = static_cast<const Natural*>( getMemberValue( "numberTaxa"  ).get() )->getValue();
    bool            isRooted = static_cast<const RbBoolean*>( getMemberValue( "isRooted"  ).get() )->getValue();
    bool            isBinary = static_cast<const RbBoolean*>( getMemberValue( "isBinary"  ).get() )->getValue();
    
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
    int             numTaxa  = static_cast<const Natural*>( getMemberValue( "numberTaxa"  ).get() )->getValue();
    bool            isRooted = static_cast<const RbBoolean*>( getMemberValue( "isRooted"  ).get() )->getValue();
    bool            isBinary = static_cast<const RbBoolean*>( getMemberValue( "isBinary"  ).get() )->getValue();
    
    // Calculate probabilities of a topology drawn from the distribution
    if (isBinary) {
        // we have a binary topology
        if (isRooted) {
            // set the number of states to default values
            numberOfStates = 1;
            for (int i=2; i<=numTaxa; i++) {
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
            for (int i=3; i<=numTaxa; i++) {
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


/** Get member variable rules */
const RbPtr<MemberRules> Dist_topologyunif::getMemberRules( void ) const {

    static RbPtr<MemberRules> memberRules( new MemberRules() );
    static bool        rulesSet = false;

    if ( !rulesSet )
		{
        memberRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "numberTaxa" , Natural_name      ) ) );
        memberRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "tipNames"   , VectorString_name ) ) );
        memberRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "isRooted"   , RbBoolean_name    ) ) );
        memberRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "isBinary"   , RbPtr<RbLanguageObject>( new RbBoolean(true) ) ) ) );

        rulesSet = true;
		}

    return memberRules;
}


/** Get the number of states in the distribution */
size_t Dist_topologyunif::getNumberOfStates( void ) const {

    return numberOfStates;
}


/** Get the probability mass vector */
const RbPtr<Simplex> Dist_topologyunif::getProbabilityMassVector( void ) {

    int             numTaxa  = static_cast<const Natural*>( getMemberValue( "numberTaxa"  ).get() )->getValue();

    if ( numTaxa <= 6 )
        return RbPtr<Simplex>(new Simplex( getNumberOfStates() ) );
    else
        throw RbException( "Too many topologies to generate probability mass vector" );
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Dist_topologyunif::getTypeSpec(void) const {
    return typeSpec;
}


/** Get random variable type */
const TypeSpec& Dist_topologyunif::getVariableType( void ) const {

    return varTypeSpec;
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
double Dist_topologyunif::lnPdf( const RbPtr<RbLanguageObject> value ) {

    // Get the parameters
    const RbPtr<Topology> top( static_cast<Topology*>( value.get() ) );
    unsigned int    numTaxa   = static_cast<const Natural*>( getMemberValue( "numberTaxa"  ).get() )->getValue();
    bool            isRooted  = static_cast<const RbBoolean*>( getMemberValue( "isRooted"  ).get() )->getValue();
    bool            isBinary  = static_cast<const RbBoolean*>( getMemberValue( "isBinary"  ).get() )->getValue();

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
double Dist_topologyunif::pdf( const RbPtr<RbLanguageObject> value ) {

    // Get the parameters
    const RbPtr<Topology> top( static_cast<Topology*>( value.get() ) );
    unsigned int    numTaxa   = static_cast<const Natural*>( getMemberValue( "numberTaxa"  ).get() )->getValue();
    bool            isRooted  = static_cast<const RbBoolean*>( getMemberValue( "isRooted"  ).get() )->getValue();
    bool            isBinary  = static_cast<const RbBoolean*>( getMemberValue( "isBinary"  ).get() )->getValue();

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
RbPtr<RbLanguageObject> Dist_topologyunif::rv( void ) {

    // Get the rng
    RbPtr<RandomNumberGenerator> rng = GLOBAL_RNG;
    
    // Get the parameters
    int                 numTaxa  = static_cast<const Natural*>( getMemberValue( "numberTaxa"  ).get() )->getValue();
    bool                isRooted = static_cast<const RbBoolean*>( getMemberValue( "isRooted"  ).get() )->getValue();
    bool                isBinary = static_cast<const RbBoolean*>( getMemberValue( "isBinary"  ).get() )->getValue();
    const RbPtr<VectorString> names( static_cast<VectorString*>( getMemberValue( "tipNames" ).get() ) );

    // Draw a random topology
    if (isBinary) {
        RbPtr<Topology> top( new Topology() );
        // internally we treat unrooted topologies the same as rooted
        top->setIsRooted(isRooted);
        
        RbPtr<TopologyNode> root( new TopologyNode((int)pow(2.0,numTaxa)-1) );
        std::vector<RbPtr<TopologyNode> > nodes;
        nodes.push_back(root);
        // recursively build the tree
        buildRandomBinaryTree(nodes, numTaxa);
        
        // set tip names
        for (int i=0; i<numTaxa; i++) {
            size_t index = size_t( floor(rng->uniform01() * nodes.size()) );
            
            // get the node from the list
            RbPtr<TopologyNode> node = nodes.at(index);
            
            // remove the randomly drawn node from the list
            nodes.erase(nodes.begin()+index);
            
            // set name
            std::string name = (*names)[i];
            node->setName(name);
        }
        
        // initialize the topology by setting the root
        top->setRoot(root);
        
        return RbPtr<RbLanguageObject>( top.get() );
    }
    // TODO: Draw a random multifurcating topology
    return RbPtr<RbLanguageObject>::getNullPtr();
}


/** We intercept a call to set a member variable to make sure that the topology prob gets recalculated */
void Dist_topologyunif::setMemberVariable( const std::string& name, RbPtr<Variable> var ) {

    DistributionDiscrete::setMemberVariable( name, var );

    // TODO: recaluclation of the probability crashes if not all variable are already set!!!
//    calculateTopologyProb();
}

