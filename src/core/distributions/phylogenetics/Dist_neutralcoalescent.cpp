/**
 * @file
 * This file contains the implementation of Dist_neutralcoalescent
 *
 * @brief Implementation of Dist_neutralcoalescent
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-07-26, version 1.0
 *
 * $Id$
 */

#include "RbBoolean.h"
#include "ConstantNode.h"
#include "DAGNode.h"
#include "Dist_neutralcoalescent.h"
#include "DistributionExponential.h"
#include "Integer.h"
#include "Natural.h"
#include "Probability.h"
#include "RbMathCombinatorialFunctions.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbUtil.h"
#include "TreePlate.h"
#include "Topology.h"
#include "TopologyNode.h"
#include "ValueRule.h"
#include "Variable.h"
#include "VectorString.h"
#include "VectorRealPos.h"

#include <algorithm>
#include <cmath>
#include <sstream>
#include <vector>


// Definition of the static type spec member
const TypeSpec Dist_neutralcoalescent::typeSpec(Dist_neutralcoalescent_name);
const TypeSpec Dist_neutralcoalescent::varTypeSpec(TreePlate_name);

/** Default constructor for parser use */
Dist_neutralcoalescent::Dist_neutralcoalescent( void ) : Distribution( getMemberRules() ) {

}

/**
    A random draw from the "Kingman distribution" (continuous time version):
    Time to go from n genes to n-1 genes; i.e. waiting time until two
    lineages coalesce.  This is a random number with an exponential
    distribution with a rate of (n choose 2).
    `haploidPopSize` is the effective *haploid* population size; i.e., number of gene
    in the population: 2 * N in a diploid population of N individuals,
    or N in a haploid population of N individuals.
    If `haploidPopSize` is 1 or 0 or None, then time is in haploid population units;
    i.e. where 1 unit of time equals 2N generations for a diploid population of
    size N, or N generations for a haploid population of size N. Otherwise time
    is in generations.
*/
double Dist_neutralcoalescent::drawWaitingTime(unsigned long numNodes, unsigned long haploidPopSize) {
    double rate = RbMath::kchoose2( int(numNodes) );
    double tmrca = RbStatistics::Exponential::rv(rate, GLOBAL_RNG) * haploidPopSize;
    return tmrca;
}

/** Clone this object */
Dist_neutralcoalescent* Dist_neutralcoalescent::clone( void ) const {

    return new Dist_neutralcoalescent( *this );
}


/** Get class vector showing type of object */
const VectorString& Dist_neutralcoalescent::getClass( void ) const {

    static VectorString rbClass = VectorString( Dist_neutralcoalescent_name ) + Distribution::getClass();
    return rbClass;
}


/** Get member variable rules */
const RbPtr<MemberRules> Dist_neutralcoalescent::getMemberRules( void ) const {

    static RbPtr<MemberRules> memberRules( new MemberRules() );
    static bool        rulesSet = false;

    if ( !rulesSet )
    {
        // memberRules.push_back( new ValueRule( "lambda"   , RealPos_name     ) );
        // memberRules.push_back( new ValueRule( "lambda"   , RealPos_name     ) );
        // memberRules.push_back( new ValueRule( "mu"       , RealPos_name     ) );
        // memberRules.push_back( new ValueRule( "rho"      , Probability_name ) );
        memberRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "tipNames", VectorString_name) ) );
        memberRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "haploidPopSize" , Natural_name, RbPtr<RbLanguageObject>( new Natural(1) ) ) ) );
        memberRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "noTaxa" , RealPos_name ) ) );

        rulesSet = true;
    }

    return memberRules;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Dist_neutralcoalescent::getTypeSpec(void) const {
    return typeSpec;
}


/** Get random variable type */
const TypeSpec& Dist_neutralcoalescent::getVariableType( void ) const {

    return varTypeSpec;
}

// TODO
//double Dist_neutralcoalescent::calInterval(TreePlate t, int i) {
//    return 0;
//}

// TODO
//double Dist_neutralcoalescent::getLineageCount(TreePlate t, int i) {
//    return 0;
//}

/**
 * This function calculates the natural log of the
 * probability of a random variable.
 *
 * @brief Natural log of probability
 *
 * @param value Observed speciation times
 * @return      Natural log of the probability
 */
double Dist_neutralcoalescent::lnPdf( const RbPtr<RbLanguageObject> value ) {

    const RbPtr<VectorRealPos> waitingTimes = RbPtr<VectorRealPos>( static_cast<VectorRealPos*>( value.get() ) );
    size_t haploidPopSize                   = static_cast<const Natural*>( getMemberValue( "haploidPopSize" ).get() )->getValue();
    size_t nWaitingTimes                    = waitingTimes->size();
    size_t k                                = nWaitingTimes + 1;

    double log_p = 0.0;

    for (size_t i = 0; i < nWaitingTimes; ++i) {
        double k2N = (k*(k-1)/2) / haploidPopSize;
        log_p = log_p + log(k2N) - (k2N * (*waitingTimes)[i]);
        k -= 1;
    }

    return log_p;
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
double Dist_neutralcoalescent::pdf( const RbPtr<RbLanguageObject> value ) {

    // // get the number of speciation events
    // size_t events = times->size();

    // // get the time of the orign. Note, we assume the process starts at the origin and not at the root. Further, we do not assume that the process starts with t_0 = 0.
    // double now   = (*times)[events-1];
    // double t_or  = now - (*times)[0] ;

    // // calculate the probability
    // double log_p = events * (2*log(b-d) - (log(p*b + (b*(1.0-p)-d)*exp((d-b)*t_or)) + log(1.0 - exp((d-b)*t_or))));

    // // iterate over all speciation events
    // for (size_t i=1; i<events; i++){
    //     double t = now - (*times)[i];
    //     log_p += (d-b)*t - 2*log(p*b + (b*(1.0-p)-d)*exp((d-b)*t));
    // }

    // return exp(log_p);
    return 0.0;
}


/**
 * This function generates a vector of speciation times from the distribution
 *
 * @brief Random draw from distribution
 *
 * @return      Generate random coalescent tree
 */
RbPtr<RbLanguageObject> Dist_neutralcoalescent::rv( void ) {

//    // Get the parameters
//    // double b                    = static_cast<const RealPos*     >( getMemberValue( "lambda"   ) )->getValue();
//    // double d                    = static_cast<const RealPos*     >( getMemberValue( "mu"       ) )->getValue();
//    // double p                    = static_cast<const Probability* >( getMemberValue( "rho"      ) )->getValue();
//    size_t haploidPopSize        = static_cast<const Natural*     >( getMemberValue( "haploidPopSize" ) )->getValue();
//    const VectorString  *names   = static_cast<const VectorString*>( getMemberValue( "tipNames" ) );
//    size_t nTips                 = names->size();
//
//    // clear tracking data
//    this->nodeEdgeLengths.clear();
//    this->numNodesToWaitingTimeMap.clear();
//
//    // create a new random tree
//    Topology *top = new Topology();
//
//    // internally we treat unrooted topologies the same as rooted
//    top->setIsRooted(true);
//
//    // create pool of nodes to be coalesced
//    std::vector<TopologyNode*> nodesToCoalesce;
//    for (size_t i=0; i < nTips; ++i) {
//        TopologyNode* node = new TopologyNode();
//        nodesToCoalesce.push_back( node );
//        this->nodeEdgeLengths.insert(std::pair<TopologyNode*, double>(node, 0.0));
//        std::string name = (*names)[i];
//        node->setName(name);
//    }
//
//    // get random number generator
//    RandomNumberGenerator* rng = GLOBAL_RNG;
//
//    // run the coalescence process
//    TopologyNode* children[2] = {NULL, NULL};
//    while (nodesToCoalesce.size() > 1) {
//        // create parent
//        TopologyNode *parent = new TopologyNode();
//        // pick and record a waiting time
//        double waitingTime = this->drawWaitingTime(nodesToCoalesce.size(), haploidPopSize);
//        // store number of waiting times to coalesce event given current number of nodes
//        this->numNodesToWaitingTimeMap.insert(std::pair<unsigned long, double>(nodesToCoalesce.size(), waitingTime));
//        // extend edges of all nodes waiting to coalesce by waiting time
//        for (std::vector<TopologyNode*>::iterator ni = nodesToCoalesce.begin(); ni != nodesToCoalesce.end(); ++ni) {
//            this->nodeEdgeLengths[*ni] = this->nodeEdgeLengths[*ni] + waitingTime;
//        }
//        // pick two children to coalesce
//        if (nodesToCoalesce.size() == 2) {
//            // probably no need to special case this,
//            // but more efficient than trying to randomly select
//            // two items from pool of two
//            children[0] = nodesToCoalesce[0];
//            children[1] = nodesToCoalesce[1];
//            nodesToCoalesce.clear();
//        } else {
//            for (int i=0; i<2; ++i) {
//                // randomly draw one node from the list of tips
//                size_t index = static_cast<size_t>( floor(rng->uniform01()*nodesToCoalesce.size()) );
//                // get the node from the list
//                children[i] = nodesToCoalesce.at(index);
//                // remove the randomly drawn node from the list
//                nodesToCoalesce.erase(nodesToCoalesce.begin()+index);
//            }
//        }
//        parent->addChild(children[0]);
//        parent->addChild(children[1]);
//        nodesToCoalesce.push_back(parent);
//        this->nodeEdgeLengths[parent] = 0.0;
//    }
//
//    // set the root node
//    // assert(nodesToCoalesce.size() == 1);
//    top->setRoot( nodesToCoalesce[0] );
//
//    // build the tree plate
//    TreePlate *plate = new TreePlate();
//    plate->setMemberVariable("topology", new Variable(new ConstantNode(top)));
//    for (TopologyNodeToRealMapType::iterator ndi = this->nodeEdgeLengths.begin(); ndi !=  this->nodeEdgeLengths.end(); ++ndi) {
////        plate->setBranchLength(ndi->first, ndi->second);
//    }
//    return plate;
    
    return RbPtr<RbLanguageObject>::getNullPtr();
}


