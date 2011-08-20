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
#include "Integer.h"
#include "Natural.h"
#include "Probability.h"
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


/** Default constructor for parser use */
Dist_neutralcoalescent::Dist_neutralcoalescent( void ) : Distribution( getMemberRules() ) {

}

double Dist_neutralcoalescent::drawWaitingTime(unsigned long numNodes, unsigned long haploidPopSize) {
    // TODO!!!!!!!!!
    // ( DUMMY IMPLEMENTATION )
    return 1.0;
}

void Dist_neutralcoalescent::buildRandomBinaryTree(std::vector<TopologyNode *> &internalNodes, std::vector<TopologyNode *> &tips, size_t numTaxa) {

    // if (tips.size() < numTaxa) {
    //     // Get the rng
    //     RandomNumberGenerator* rng = GLOBAL_RNG;

    //     // randomly draw one node from the list of tips
    //     size_t index = static_cast<size_t>( floor(rng->uniform01()*tips.size()) );

    //     // get the node from the list
    //     TopologyNode* parent = tips.at(index);

    //     // add the internal node
    //     internalNodes.push_back(parent);

    //     // remove the randomly drawn node from the list
    //     tips.erase(tips.begin()+index);

    //     // add a left child
    //     TopologyNode *leftChild = new TopologyNode(0);
    //     parent->addChild(leftChild);
    //     tips.push_back(leftChild);

    //     // add a right child
    //     TopologyNode *rightChild = new TopologyNode(0);
    //     parent->addChild(rightChild);
    //     tips.push_back(rightChild);

    //     // recursive call to this function
    //     buildRandomBinaryTree(internalNodes, tips, numTaxa);
    // }
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
const MemberRules& Dist_neutralcoalescent::getMemberRules( void ) const {

    static MemberRules memberRules;
    static bool        rulesSet = false;

    if ( !rulesSet )
    {
        // memberRules.push_back( new ValueRule( "lambda"   , RealPos_name     ) );
        // memberRules.push_back( new ValueRule( "lambda"   , RealPos_name     ) );
        // memberRules.push_back( new ValueRule( "mu"       , RealPos_name     ) );
        // memberRules.push_back( new ValueRule( "rho"      , Probability_name ) );
        memberRules.push_back( new ValueRule( "tipNames", VectorString_name) );
        memberRules.push_back( new ValueRule( "haploidPopSize" , Natural_name, new Natural(1) ) );
        memberRules.push_back( new ValueRule( "noTaxa" , RealPos_name ) );

        rulesSet = true;
    }

    return memberRules;
}


/** Get random variable type */
const TypeSpec Dist_neutralcoalescent::getVariableType( void ) const {

    return TypeSpec( TreePlate_name );
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
double Dist_neutralcoalescent::lnPdf( const RbLanguageObject* value ) {

    // Get the parameters
    // const VectorRealPos* times = static_cast<const VectorRealPos*>( value );
    // double                 b   = static_cast<const RealPos*    >( getMemberValue( "lambda" ) )->getValue();
    // double                 d   = static_cast<const RealPos*    >( getMemberValue( "mu"     ) )->getValue();
    // double                 p   = static_cast<const Probability*>( getMemberValue( "rho"    ) )->getValue();

    // // get the number of speciation events
    // size_t events = times->getLength();

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

    // return log_p;

    const VectorRealPos* times = static_cast<const VectorRealPos*>( value );
    TreePlate* t;// TODO get tree?

    // get the number of coalescent events
    size_t events = times->getLength();

    double log_p = 0.0;
    double startTime;

    for (size_t i = 0; i < events; i++) {


//        double finishTime = startTime + getInterval(t, i);
//        double intervalArea = calIntegral(startTime, finishTime);
//
//        int lCount = getLineageCount(i);
//        int k2 RbMath::kchoose2(lCount);
//
//        log_p += -k2 * intervalArea;
//
//
//        startTime = finishTime;
    }

//        return log_p;

    return 0.0;
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
double Dist_neutralcoalescent::pdf( const RbLanguageObject* value ) {

    // // Get the parameters
    // const VectorRealPos* times = static_cast<const VectorRealPos*>( value );
    // double                 b   = static_cast<const RealPos*    >( getMemberValue( "lambda" ) )->getValue();
    // double                 d   = static_cast<const RealPos*    >( getMemberValue( "mu"     ) )->getValue();
    // double                 p   = static_cast<const Probability*>( getMemberValue( "rho"    ) )->getValue();

    // // get the number of speciation events
    // size_t events = times->getLength();

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
TreePlate* Dist_neutralcoalescent::rv( void ) {

    // Get the parameters
    // double b                    = static_cast<const RealPos*     >( getMemberValue( "lambda"   ) )->getValue();
    // double d                    = static_cast<const RealPos*     >( getMemberValue( "mu"       ) )->getValue();
    // double p                    = static_cast<const Probability* >( getMemberValue( "rho"      ) )->getValue();
    size_t haploidPopSize        = static_cast<const Natural*     >( getMemberValue( "haploidPopSize" ) )->getValue();
    const VectorString  *names   = static_cast<const VectorString*>( getMemberValue( "tipNames" ) );
    size_t nTips                 = names->getLength();

    // create a new random tree
    Topology *top = new Topology();

    // internally we treat unrooted topologies the same as rooted
    top->setIsRooted(true);

    // track edge lengths
    Dist_neutralcoalescent::NodeDoubleMapType edge_lengths; // TODO: assign initial size

    // create pool of nodes to be coalesced
    std::vector<TopologyNode*> nodesToCoalesce;
    for (size_t i=0; i < nTips; ++i) {
        TopologyNode* node = new TopologyNode();
        nodesToCoalesce.push_back( node ); //  what should I use as index??
        edge_lengths.insert(std::pair<TopologyNode*, double>(node, 0.0));
        std::string name = (*names)[i];
        node->setName(name);
    }

    // get random number generator
    RandomNumberGenerator* rng = GLOBAL_RNG;

    // prepare the waiting times
    this->waitingTimes.clear();

    // run the coalescence process
    TopologyNode* children[2] = {NULL, NULL};
    while (nodesToCoalesce.size() > 0) {
        // create parent
        TopologyNode *parent = new TopologyNode();
        // pick and record a waiting time
        double waitingTime = this->drawWaitingTime(nodesToCoalesce.size(), haploidPopSize);
        this->waitingTimes.insert(std::pair<TopologyNode*, double>(parent, waitingTime));
        // extend all edges by waiting time
        for (Dist_neutralcoalescent::NodeDoubleMapType::iterator ei = edge_lengths.begin(); ei != edge_lengths.end(); ++ei) {
            edge_lengths[ei->first] = ei->second + waitingTime;
        }
        // pick two children to coalesce
        for (int i=0; i<2; ++i) {
            // randomly draw one node from the list of tips
            size_t index = static_cast<size_t>( floor(rng->uniform01()*nodesToCoalesce.size()) );
            // get the node from the list
            children[i] = nodesToCoalesce.at(index);
            // remove the randomly drawn node from the list
            // NOTE: memory not freed!!
            nodesToCoalesce.erase(nodesToCoalesce.begin()+index);
        }
        parent->addChild(children[0]);
        parent->addChild(children[1]);
        nodesToCoalesce.push_back(parent);
        edge_lengths[parent] = 0.0;
    }

    // TODO: use tree plate to add edge lengths to nodes


    // double scale    = b - d;
    // b /= scale;
    // d /= scale;

    // // Get the rng
    // RandomNumberGenerator* rng = GLOBAL_RNG;

    // // get a new random number
    // double ran = rng->uniform01();

    // // simulate the time of the origin
	// double t_or = log((-b * p - b * pow(ran,(1.0/nTips)) + d * pow(ran,(1.0/nTips)) + b * p * pow(ran,(1.0/nTips)))/(b * p * (-1.0 + pow(ran,(1.0/nTips))))) / (b - d);

    // // tmp vector of speciation times. we need to sort this vector before we use it later
    // std::vector<double> times;

    // // simulate speciation times
    // for (size_t i=0; i<nTips; i++) {
    //     ran = rng->uniform01();
    //     double t = 1.0/(p*b-(d-b*(1.0-p)))*log((p*b-(d-b*(1.0-p))* exp((-(p*b)+(d-b*(1.0-p)))*t_or) -(d-b*(1.0-p))*(1-exp((-(p*b)+(d-b*(1.0-p)))*t_or)) *ran )/(p*b-(d-b*(1.0-p))* exp((-(p*b)+(d-b*(1.0-p)))*t_or) -(p*b)*(1-exp((-(p*b)+(d-b*(1.0-p)))*t_or)) *ran )   );
    //     times.push_back(t);
    // }

    // // create a new random tree
    // Topology *top = new Topology();
    // // internally we treat unrooted topologies the same as rooted
    // top->setIsRooted(true);

    // TopologyNode *root = new TopologyNode((int)pow(2.0,nTips)-1);
    // std::vector<TopologyNode*> tips;
    // std::vector<TopologyNode*> interior;
    // tips.push_back(root);
    // // recursively build the tree
    // buildRandomBinaryTree(interior, tips, nTips);

    // // set tip names
    // for (int i=0; i<nTips; i++) {
    //     size_t index = size_t( floor(rng->uniform01() * tips.size()) );

    //     // get the node from the list
    //     TopologyNode* node = tips.at(index);

    //     // remove the randomly drawn node from the list
    //     tips.erase(tips.begin()+index);

    //     // set name
    //     std::string name = (*names)[i];
    //     node->setName(name);
    // }

    // // initialize the topology by setting the root
    // top->setRoot(root);

    // // sort the speciation times
    // sort (times.begin(), times.end());

    // // create a tree plate
    // TreePlate *plate = new TreePlate();
    // plate->setMemberVariable("topology", new Variable(new ConstantNode(top)));

    // // add the speciation time to the topology
    // for (size_t i=0; i<times.size(); i++) {
    //     plate->setNodeTime(interior[i],times[i]*scale);
    // }

    TreePlate *plate = new TreePlate();
    return plate;
}


