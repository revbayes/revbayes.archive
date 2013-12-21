#include "Clade.h"
#include "BirthDeathProcess.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathCombinatorialFunctions.h"
#include "TopologyNode.h"
#include "Topology.h"

#include <algorithm>
#include <cmath>

using namespace RevBayesCore;

BirthDeathProcess::BirthDeathProcess(const TypedDagNode<double> *o, 
                                                     const TypedDagNode<double> *r, const std::string& ss, const std::string &cdt, unsigned int nTaxa, 
                                                     const std::vector<std::string> &tn, const std::vector<Clade> &c) : TypedDistribution<TimeTree>( new TimeTree() ), 
origin( o ), rho( r ), samplingStrategy( ss ), condition( cdt ), numTaxa( nTaxa ), taxonNames( tn ), constraints( c ) {
    
    addParameter( origin );
    addParameter( rho );
    
    double lnFact = 0.0;
    for (size_t i = 2; i < numTaxa; i++) {
        lnFact += std::log(i);
    }
    
    logTreeTopologyProb = (numTaxa - 1) * RbConstants::LN2 - 2.0 * lnFact - std::log( numTaxa ) ;
        
}



void BirthDeathProcess::attachTimes(TimeTree *psi, std::vector<TopologyNode *> &tips, size_t index, const std::vector<double> &times, double T) {
    
    if (index < numTaxa-1) 
    {
        // Get the rng
        RandomNumberGenerator* rng = GLOBAL_RNG;
        
        // randomly draw one node from the list of tips
        size_t tip_index = static_cast<size_t>( floor(rng->uniform01()*tips.size()) );
        
        // get the node from the list
        TopologyNode* parent = tips.at(tip_index);
        psi->setAge( parent->getIndex(), T - times[index] );
        
        // remove the randomly drawn node from the list
        tips.erase(tips.begin()+tip_index);
        
        // add a left child
        TopologyNode* leftChild = &parent->getChild(0);
        if ( !leftChild->isTip() ) 
        {
            tips.push_back(leftChild);
        }
        
        // add a right child
        TopologyNode* rightChild = &parent->getChild(1);
        if ( !rightChild->isTip() ) 
        {
            tips.push_back(rightChild);
        }
        
        // recursive call to this function
        attachTimes(psi, tips, index+1, times, T);
    }
}


void BirthDeathProcess::buildRandomBinaryTree(std::vector<TopologyNode*> &tips) {
    
    if (tips.size() < numTaxa) 
    {
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
        buildRandomBinaryTree(tips);
    }
}



double BirthDeathProcess::computeLnProbability( void ) {
    
    // variable declarations and initialization
    double lnProbTimes = 0;
    
    
    if ( !matchesConstraints() ) 
    {
        return RbConstants::Double::neginf;
    }
    
    double samplingProbability = 1.0;
    if ( samplingStrategy == "uniform" ) 
        samplingProbability = rho->getValue();
    
    // retrieved the speciation times
    std::vector<double> times;
    for (size_t i = 0; i < value->getNumberOfInteriorNodes()+1; ++i) 
    {
        const TopologyNode& n = value->getInteriorNode( i );
        double t = n.getTime();
        times.push_back(t);
    }
    // sort the vector of times in ascending order
    std::sort(times.begin(), times.end());
    
    // present time 
    double T = value->getTipNode(0).getTime();
    
    
    // what do we condition on?
    // did we condition on survival?
    if ( condition == "survival" )    lnProbTimes = - pSurvival(0,T,T,samplingProbability);
    
    // multiply the probability of a descendant of the initial species
    lnProbTimes += lnP1(0,T,samplingProbability);
    
    // add the survival of a second species if we condition on the MRCA
    int numInitialSpecies = 1;
    // check if we condition on the root or origin
    if ( times[0] == 0.0 ) {
        ++numInitialSpecies;
        lnProbTimes *= 2.0;
    }
    
    for (size_t i = (numInitialSpecies-1); i < numTaxa-1; ++i) {
        if ( lnProbTimes == RbConstants::Double::nan || lnProbTimes == RbConstants::Double::inf || lnProbTimes == RbConstants::Double::neginf) {
            return RbConstants::Double::nan;
        }
        lnProbTimes += lnSpeciationRate(times[i]) + lnP1(times[i],T,samplingProbability);
    }
    
    // if we assume diversified sampling, we need to multiply with the probability that all missing species happened after the last speciation event
    if ( samplingStrategy == "diversified" ) {
        // We use equation (5) of Hoehna et al. "Inferring Speciation and Extinction Rates under Different Sampling Schemes"
        double lastEvent = times[times.size()-2];
        
        double p_0_T = 1.0 - pSurvival(0,T,T,1.0) * exp( rateIntegral(0,T) );
        double p_0_t = (1.0 - pSurvival(lastEvent,T,T,1.0) * exp( rateIntegral(lastEvent,T) ));
        double F_t = p_0_t / p_0_T;
        
        // get an estimate of the actual number of taxa
        double m = round(numTaxa / rho->getValue());
        lnProbTimes += (m-numTaxa) * log(F_t) + log(RbMath::choose(m,numTaxa));
    }
    
    return lnProbTimes; // + logTreeTopologyProb;
    
}


double BirthDeathProcess::lnP1(double t, double T, double r) const {
    
    // get the survival probability
    double a = log( pSurvival(t,T,T,r) );
    double b = rateIntegral(t, T);
    
    // compute the probability of observing/sampling exactly one lineage
    double p = 2.0 * a + b;
    
    return p;
    
}


bool BirthDeathProcess::matchesConstraints( void ) {
    
    const TopologyNode &root = value->getRoot();
    
    for (std::vector<Clade>::iterator it = constraints.begin(); it != constraints.end(); ++it) 
    {
        if ( !root.containsClade( *it ) ) 
        {
            return false;
        }
    }
    
    return true;
}



void BirthDeathProcess::redrawValue( void ) {
    simulateTree();
}


void BirthDeathProcess::simulateTree( void ) {
    
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // the time tree object (topology + times)
    TimeTree *psi = new TimeTree();
    
    // Draw a random topology
    Topology *tau = new Topology();
    
    // internally we treat unrooted topologies the same as rooted
    tau->setRooted( true );
    
    TopologyNode* root = new TopologyNode();
    std::vector<TopologyNode* > nodes;
    nodes.push_back(root);
    
    // recursively build the tree
    buildRandomBinaryTree(nodes);
    
    // set tip names
    for (size_t i=0; i<numTaxa; i++) 
    {
        size_t index = size_t( floor(rng->uniform01() * nodes.size()) );
        
        // get the node from the list
        TopologyNode* node = nodes.at(index);
        
        // remove the randomly drawn node from the list
        nodes.erase(nodes.begin()+index);
        
        // set name
        std::string& name = taxonNames[i];
        node->setName(name);
    }
    
    // initialize the topology by setting the root
    tau->setRoot(root);
    
    // connect the tree with the topology
    psi->setTopology( tau );
    
    // now simulate the speciation times
    // first, get the time of the origin
	double t_or = origin->getValue();
    
    // allocate the vector for the times
    std::vector<double> t = std::vector<double>(numTaxa,0.0);
	t[0] = 0.0;
    
    double samplingProbability = 1.0;
    if ( samplingStrategy == "uniform" )
        samplingProbability = rho->getValue();
    
    size_t n = numTaxa;
    if ( samplingStrategy == "diversified" ) 
    {
        n = size_t( ceil(numTaxa / rho->getValue()) );
    }
    
    // draw a time for each speciation event condition on the time of the process
	for (size_t i = 1; i < n; ++i) 
    {
		t[i] = t_or - simSpeciation(t_or, samplingProbability);
	}
    
    std::sort(t.begin(),t.end());
    
    nodes.clear();
    nodes.push_back( root );
    attachTimes(psi, nodes, 0, t, t_or);
    for (size_t i = 0; i < numTaxa; ++i) 
    {
        TopologyNode& node = tau->getTipNode(i);
        psi->setAge( node.getIndex(), 0.0 );
    }
    
    // finally store the new value
    value = psi;
    
}



void BirthDeathProcess::swapParameter(const DagNode *oldP, const DagNode *newP) {
    
    if ( oldP == rho ) 
    {
        rho = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if ( oldP == origin ) 
    {
        origin = static_cast<const TypedDagNode<double>* >( newP );
    }

}
