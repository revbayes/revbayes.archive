
#include "Clade.h"
#include "ConstantBirthDeathProcess.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathCombinatorialFunctions.h"
#include "TopologyNode.h"
#include "Topology.h"

#include <algorithm>
#include <cmath>

using namespace RevBayesCore;

ConstantBirthDeathProcess::ConstantBirthDeathProcess(const TypedDagNode<double> *s, const TypedDagNode<double> *e, const TypedDagNode<double> *r,
                                                     const TypedDagNode< std::vector<double> >* met, const TypedDagNode< std::vector<double> >* mep, 
                                                     const std::string& ss, const std::string &cdt, unsigned int nTaxa, const std::vector<std::string> &tn, 
                                                     const std::vector<Clade> &c) : TypedDistribution<TimeTree>( new TimeTree() ), 
speciation( s ), extinction( e ), rho( r ), massExtinctionTimes( met ), massExtinctionSurvivalProrbabilities( mep ), samplingStrategy( ss ), condition( cdt ), numTaxa( nTaxa ), taxonNames( tn ), constraints( c ) {

    addParameter( speciation );
    addParameter( extinction );
    addParameter( rho );
    addParameter( massExtinctionTimes );
    addParameter( massExtinctionSurvivalProrbabilities );
    
    double lnFact = 0.0;
    for (size_t i = 2; i < numTaxa; i++) {
        lnFact += std::log(i);
    }
    
    logTreeTopologyProb = (numTaxa - 1) * RbConstants::LN2 - 2.0 * lnFact - std::log( numTaxa ) ;
    
    simulateTree();

}



ConstantBirthDeathProcess::ConstantBirthDeathProcess(const ConstantBirthDeathProcess &v) : TypedDistribution<TimeTree>( v ), speciation( v.speciation ), extinction( v.extinction ), samplingStrategy( v.samplingStrategy ), condition( v.condition ), numTaxa( v.numTaxa ), taxonNames( v.taxonNames ), logTreeTopologyProb( v.logTreeTopologyProb ) {
    // parameters are automatically copied
}


ConstantBirthDeathProcess::~ConstantBirthDeathProcess() {
    
}



void ConstantBirthDeathProcess::attachTimes(TimeTree *psi, std::vector<TopologyNode *> &tips, int index, const std::vector<double> &times, double T) {
    
    if (index < numTaxa-1) {
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
        if ( !leftChild->isTip() ) {
            tips.push_back(leftChild);
        }
        
        // add a right child
        TopologyNode* rightChild = &parent->getChild(1);
        if ( !rightChild->isTip() ) {
            tips.push_back(rightChild);
        }
        
        // recursive call to this function
        attachTimes(psi, tips, index+1, times, T);
    }
}


void ConstantBirthDeathProcess::buildRandomBinaryTree(std::vector<TopologyNode*> &tips) {
    
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
        buildRandomBinaryTree(tips);
    }
}


ConstantBirthDeathProcess* ConstantBirthDeathProcess::clone( void ) const {
    return new ConstantBirthDeathProcess( *this );
}


double ConstantBirthDeathProcess::computeLnProbability( void ) {
    // variable declarations and initialization
    double lnProbTimes = 0;
    
    
    if ( !matchesConstraints() ) {
        return RbConstants::Double::neginf;
    }
    
    double samplingProbability = 1.0;
    if ( samplingStrategy == "uniform" ) 
        samplingProbability = rho->getValue();
    
    // retrieved the speciation times
    std::vector<double> times;
    for (int i = 0; i < value->getNumberOfInteriorNodes()+1; ++i) {
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
    lnProbTimes += p1(0,T,samplingProbability);
        
    // add the survival of a second species if we condition on the MRCA
    int numInitialSpecies = 1;
    // check if we condition on the root or origin
    if ( times[0] == 0.0 ) {
        ++numInitialSpecies;
        lnProbTimes *= 2.0;
    }
    
    double lambda = speciation->getValue();
    for (int i = (numInitialSpecies-1); i < numTaxa-1; ++i) {
        if ( lnProbTimes == RbConstants::Double::nan || lnProbTimes == RbConstants::Double::inf || lnProbTimes == RbConstants::Double::neginf) {
            return RbConstants::Double::nan;
        }
        lnProbTimes += log(lambda) + p1(times[i],T,samplingProbability);
    }
    
    // if we assume diversified sampling, we need to multiply with the probability that all missing species happened after the last speciation event
    if ( samplingStrategy == "diversified" ) {
        // We use equation (5) of Hoehna et al. "Inferring Speciation and Extinction Rates under Different Sampling Schemes"
        double lastEvent = times[times.size()-2];
        double lambda = speciation->getValue();
        double mu = extinction->getValue();
        double div = lambda - mu;
        
        double p_0_T = 1.0 - pSurvival(0,T,T,1.0) * exp( -div * T );
        double p_0_t = (1.0 - pSurvival(lastEvent,T,T,1.0) * exp(div * (lastEvent-T)));
        double F_t = p_0_t / p_0_T;
        
        // get an estimate of the actual number of taxa
        double m = round(numTaxa / rho->getValue());
        lnProbTimes += (m-numTaxa) * log(F_t) + log(RbMath::choose(m,numTaxa));
    }
    
    return lnProbTimes; // + logTreeTopologyProb;
    
}


bool ConstantBirthDeathProcess::matchesConstraints( void ) {
    
    const TopologyNode &root = value->getRoot();
    
    for (std::vector<Clade>::iterator it = constraints.begin(); it != constraints.end(); ++it) {
        if ( !root.containsClade( *it ) ) {
            return false;
        }
    }
    
    return true;
}


double ConstantBirthDeathProcess::p1(double t, double T, double r) const {
    double a = log(pSurvival(t,T,T,r));
    double b = (speciation->getValue() - extinction->getValue()) * (t-T);

    // subtract the probability that we might not have sampled this species (or any descendant)
    b -= log(r);
    
    // add mass-extinction
    const std::vector<double> &met = massExtinctionTimes->getValue();
    const std::vector<double> &mep = massExtinctionSurvivalProrbabilities->getValue();
    for (size_t j=0; j<met.size(); ++j ) 
    {
        if ( t < met[j] && T > met[j] ) 
        {
            b -= log(mep[j]);
        }
    }
    
    double p = 2.0 * a + b;
    
    return p;

}


double ConstantBirthDeathProcess::pSurvival(double start, double end, double T, double r) const {
    
    // compute the rate
    double mu = extinction->getValue();
    double lambda = speciation->getValue();
    double rate = mu - lambda;
    
    // do the integration of int_{t_low}^{t_high} ( mu(s) exp(rate(t,s)) ds )
    // where rate(t,s) = int_{t}^{s} ( mu(x)-lambda(x) dx ) - sum_{for all t < m_i < s in massExtinctionTimes }( log(massExtinctionSurvivalProbability[i]) )
    
    // we compute the integral stepwise for each epoch between mass-extinction events
    // add mass-extinction
    double accumulatedMassExtinction = 1.0;
    double prev_time = start;
    double den = 1.0;
    const std::vector<double> &met = massExtinctionTimes->getValue();
    const std::vector<double> &mep = massExtinctionSurvivalProrbabilities->getValue();
    if ( met.size() > 0 ) 
    {
        for (size_t j=0; j<met.size(); ++j ) 
        {
            if ( start < met[j] && end > met[j] ) 
            {
                // compute the integral for this time episode until the mass-extinction event
                den += exp(-rate*start) * mu / (rate * accumulatedMassExtinction ) * ( exp(rate* met[j]) - exp(rate*prev_time));
                // store the current time so that we remember from which episode we need to integrate next
                prev_time = met[j];
                accumulatedMassExtinction *= mep[j];
                // integrate over the tiny time interval of the mass-extinction event itself and add it to the integral
                den -= (mep[j]-1) / accumulatedMassExtinction * exp( rate*(met[j] - start) );
            }
        }
    }
    
    // add the integral of the final epoch until the present time
    den += exp(-rate*start) * mu / (rate * accumulatedMassExtinction ) * ( exp(rate*end) - exp(rate*prev_time));
    
    // add sampling
    if ( (start < T) && (end >= T) )
    {
        accumulatedMassExtinction *= r;
        den -= (r-1)*exp( rate*(T-start) ) / accumulatedMassExtinction;
    }
    
    return (1.0 / den);
    
}


void ConstantBirthDeathProcess::redrawValue( void ) {
    simulateTree();
}



double ConstantBirthDeathProcess::simOrigin( void ) {
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // random draw
    double u = rng->uniform01();
    
    // get the parameters
    double lambda = speciation->getValue();
    double mu = extinction->getValue();
    double div = lambda - mu;
    
    // the expected number of taxa if we would have sampled all species
    double n = numTaxa / rho->getValue();
    
	double origin = std::log( (-lambda + mu * std::pow(u, (1.0/n) )) / (lambda * (-1 + std::pow(u,(1.0/n))))) / div;
	
    return origin;
}


double ConstantBirthDeathProcess::simSpeciation(double origin, double r) {
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
	double u = rng->uniform01();
    
    // get the parameters
    double lambda = speciation->getValue()*r;
    double mu = extinction->getValue() - speciation->getValue()*(1.0-r);
    double div = lambda - mu;
    
    double t = 1.0/div * log((lambda - mu * exp((-div)*origin) - mu * (1.0 - exp((-div) * origin)) * u )/(lambda - mu * exp((-div) * origin) - lambda * (1.0 - exp(( -div ) * origin)) * u ) );  
	
    return t;
}


void ConstantBirthDeathProcess::simulateTree( void ) {
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
    for (int i=0; i<numTaxa; i++) {
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
    // draw a sample for the origin
	double t_or = simOrigin();
    
    // allocate the vector for the times
    std::vector<double> t = std::vector<double>(numTaxa,0.0);
	t[0] = 0.0;
    
    double samplingProbability = 1.0;
    if ( samplingStrategy == "uniform" )
        samplingProbability = rho->getValue();
    
    size_t n = numTaxa;
    if ( samplingStrategy == "diversified" ) {
        n = size_t( ceil(numTaxa / rho->getValue()) );
    }
    
    // draw a time for each speciation event condition on the time of the process
	for (size_t i = 1; i < n; ++i) {
		t[i] = t_or - simSpeciation(t_or, samplingProbability);
	}
    
    std::sort(t.begin(),t.end());
    
    nodes.clear();
    nodes.push_back( root );
    attachTimes(psi, nodes, 0, t, t_or);
    for (size_t i = 0; i < numTaxa; ++i) {
        TopologyNode& n = tau->getTipNode(i);
        psi->setAge( n.getIndex(), 0.0 );
    }
    
    // finally store the new value
    value = psi;
    
}



void ConstantBirthDeathProcess::swapParameter(const DagNode *oldP, const DagNode *newP) {
    if (oldP == speciation) {
        speciation = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == extinction) {
        extinction = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == rho) {
        rho = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == massExtinctionTimes) {
        massExtinctionTimes = static_cast<const TypedDagNode<std::vector<double> >* >( newP );
    }
    else if (oldP == massExtinctionSurvivalProrbabilities) {
        massExtinctionSurvivalProrbabilities = static_cast<const TypedDagNode<std::vector<double> >* >( newP );
    }
}
