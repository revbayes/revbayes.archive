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


/**
 * Constructor.
 *
 * The constructor connects the parameters of the birth-death process (DAG structure)
 * and initializes the probability density by computing the combinatorial constant of the tree structure.
 *
 * \param[in]    o         Origin or time of the process.
 * \param[in]    r         Sampling probability of a species at present.
 * \param[in]    ss        The sampling strategy (uniform/diversified).
 * \param[in]    cdt       The condition of the process (time/survival/nTaxa)
 * \param[in]    nTaxa     Number of taxa (used for initialization during simulation).
 * \param[in]    tn        Taxon names used during initialization.
 * \param[in]    c         Clade constraints.
 */
BirthDeathProcess::BirthDeathProcess(const TypedDagNode<double> *o, const TypedDagNode<double> *r, 
                                     const std::string& ss, const std::string &cdt, unsigned int nTaxa, 
                                     const std::vector<std::string> &tn, const std::vector<Clade> &c) : TypedDistribution<TimeTree>( new TimeTree() ), 
        condition( cdt ),
        constraints( c ),
        origin( o ),
        rho( r ),
        numTaxa( nTaxa ),
        samplingStrategy( ss ),
        taxonNames( tn )
{
    
    addParameter( origin );
    addParameter( rho );
    
    // the combinatorial factor for the probability of a labelled history is
    // 2^{n-1} / ( n! * (n-1)! )
    // but since the probability of the divergence times contains the factor (n-1)! we simply stor
    // 2^{n-1} / n!
    double lnFact = 0.0;
    for (size_t i = 2; i <= numTaxa; i++) 
    {
        lnFact += std::log(i);
    }
    logTreeTopologyProb = (numTaxa - 1) * RbConstants::LN2 - lnFact ;
        
}



/**
 * Randomly attach the times to a tree topology.
 * This function works by randomly picking a node from the set of tips,
 * setting its time to times[index], increment the index,
 * adding the two children (if they are not actual tips) to the set of tips,
 * and recursively calling this function again.
 *
 * \param[in]     psi        The tree topology (needed to call setAge).
 * \param[in]     tips       The vector of tips
 * \param[in]     index   
 * \param[in]     times         
 * \param[in]     T      
 */
void BirthDeathProcess::attachTimes(TimeTree *psi, std::vector<TopologyNode *> &tips, size_t index, const std::vector<double> &times, double T) 
{
    
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
    
    // no return value
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


/**
 * Compute the log-transformed probability of the current value under the current parameter values.
 *
 * \return   The log-transformed probability density.
 */
double BirthDeathProcess::computeLnProbability( void ) {
    
    // variable declarations and initialization
    double lnProbTimes = 0;
    
    // first check if the current tree matches the clade constraints
    if ( !matchesConstraints() ) 
    {
        return RbConstants::Double::neginf;
    }
    
    // present time 
    double tipTime = value->getTipNode(0).getTime();
    double org = origin->getValue();
    
    // test that the time of the process is larger or equal to the present time
    if ( tipTime > org ) 
    {
        return RbConstants::Double::neginf;
    }
    
    double presentTime = org;
    
    // retrieved the speciation times
    std::vector<double> times = divergenceTimesSinceOrigin();
    
    double samplingProbability = 1.0;
    if ( samplingStrategy == "uniform" ) 
    {
        samplingProbability = rho->getValue();
    }
    
    // what do we condition on?
    // did we condition on survival?
    if ( condition == "survival" )    
    {
        lnProbTimes = - log( pSurvival(0,presentTime,samplingProbability) );
    }
    
    // multiply the probability of a descendant of the initial species
    lnProbTimes += lnP1(0,presentTime,samplingProbability);
    
    // add the survival of a second species if we condition on the MRCA
    int numInitialSpecies = 1;
    
    // check if we condition on the root or origin
    bool conditionOnMRCA = false;
    if ( conditionOnMRCA == true ) 
    {
        ++numInitialSpecies;
        lnProbTimes *= 2.0;
    }
    
    for (size_t i = (numInitialSpecies-1); i < numTaxa-1; ++i) 
    {
        if ( lnProbTimes == RbConstants::Double::nan || 
            lnProbTimes == RbConstants::Double::inf || 
            lnProbTimes == RbConstants::Double::neginf ) 
        {
            return RbConstants::Double::nan;
        }
         
        lnProbTimes += lnSpeciationRate(times[i]) + lnP1(times[i],presentTime,samplingProbability);
    }
    
    // if we assume diversified sampling, we need to multiply with the probability that all missing species happened after the last speciation event
    if ( samplingStrategy == "diversified" ) 
    {
        // We use equation (5) of Hoehna et al. "Inferring Speciation and Extinction Rates under Different Sampling Schemes"
        double lastEvent = times[times.size()-2];
        
        double p_0_T = 1.0 - pSurvival(0,presentTime,1.0) * exp( rateIntegral(0,presentTime) );
        double p_0_t = (1.0 - pSurvival(lastEvent,presentTime,1.0) * exp( rateIntegral(lastEvent,presentTime) ));
        double F_t = p_0_t / p_0_T;
        
        // get an estimate of the actual number of taxa
        double m = round(numTaxa / rho->getValue());     
        lnProbTimes += (m-numTaxa) * log(F_t) + log(RbMath::choose(m,numTaxa));
    }
    
    return lnProbTimes + logTreeTopologyProb;
    
}


std::vector<double> BirthDeathProcess::divergenceTimesSinceOrigin( void ) const
{
    
    // get the time of the process
    double org = origin->getValue();
    double tipTime = value->getTipNode(0).getTime();
    double offset = org - tipTime;
    
    // retrieved the speciation times
    std::vector<double> times;
    for (size_t i = 0; i < value->getNumberOfInteriorNodes()+1; ++i) 
    {
        const TopologyNode& n = value->getInteriorNode( i );
        double t = n.getTime() + offset;
        times.push_back(t);
    }
    // sort the vector of times in ascending order
    std::sort(times.begin(), times.end());
    
    return times;
}


/**
 * Compute the diversity of the tree at time t.
 *
 * \param[in]    t      time at which we want to know the diversity.
 *
 * \return The diversity (number of species in the reconstructed tree).
 */
int BirthDeathProcess::diversity(double t) const
{
    std::vector<double> times = divergenceTimesSinceOrigin();
    
    for (size_t i = 0; i < times.size(); ++i) 
    {
        if ( times[i] > t ) 
        {
            return int( i + 2 );
        }
    }
    
    return int(times.size() + 2);
}


double BirthDeathProcess::lnP1(double t, double T, double r) const {
    
    // get the survival probability
    double a = log( pSurvival(t,T,r) );
    double b = rateIntegral(t, T) - log(r);
    
    // compute the probability of observing/sampling exactly one lineage
    double p = 2.0 * a + b;
    
    return p;
    
}


bool BirthDeathProcess::matchesConstraints( void ) {
    
    const TopologyNode &root = value->getRoot();
    
    for (std::vector<Clade>::iterator it = constraints.begin(); it != constraints.end(); ++it) 
    {
        if ( !root.containsClade( *it, true ) ) 
        {
            return false;
        }
    }
    
    return true;
}


/**
 * Compute the probabililty of survival (no extinction) of the process including uniform taxon sampling at the present time.
 * The probability of survival is given by
 * [1 + int_{t_low}^{t_high} ( mu(s) exp(rate(t,s)) ds ) ]^{-1}
 * and can be simplified to
 * [1 + int_{t_low}^{t_high} ( mu'(s) exp(rate'(t,s)) ds ) - (r-1)/r*exp(rate'(t_low,t_high)) ]^{-1}
 * where mu' and rate' are the diversification rate function without incomplete taxon sampling.
 * Therefore we can just call pSurvival without incomplete taxon sampling that will be computed in the derived classes,
 * and add the sampling here so that sampling will be available for all models :)
 * For more information please read Hoehna, S. 2014. The time-dependent reconstructed evolutionary process with a key-role for mass-extinction events.
 *
 * \param[in]    start      Start time of the process.
 * \param[in]    end        End/stopping time of the process.
 * \param[in]    r          Sampling probability.
 *
 * \return The probability of survival of the process.
 */
double BirthDeathProcess::pSurvival(double start, double end, double r) const
{
    double rate = rateIntegral(start, end);
    double ps = 1.0 / pSurvival(start, end);
    
    return 1.0 / (ps - (r-1.0)/r * exp(rate) );
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
    psi->setTopology( tau, true );
    
    // now simulate the speciation times
    // first, get the time of the origin
	double t_or = origin->getValue();
    
    
    double samplingProbability = 1.0;
    if ( samplingStrategy == "uniform" )
    {
        samplingProbability = rho->getValue();
    }
    
    size_t n = numTaxa;
    if ( samplingStrategy == "diversified" ) 
    {
        n = size_t( ceil(numTaxa / rho->getValue()) );
    }
    
    // draw a time for each speciation event condition on the time of the process
    std::vector<double> t = simSpeciations(n, t_or, samplingProbability);
    std::vector<double> times;
    
    // under diversified sampling we only use the first numTaxa simulated events
    if ( samplingStrategy == "diversified" ) 
    {
        times = std::vector<double>(t.begin(), t.begin()+(numTaxa-1));
    }
    else
    {
        times = t;
    }
        
    nodes.clear();
    nodes.push_back( root );
    attachTimes(psi, nodes, 0, times, t_or);
    // \todo Why are we doing this? (Sebastian)
    for (size_t i = 0; i < numTaxa; ++i) 
    {
        TopologyNode& node = tau->getTipNode(i);
        psi->setAge( node.getIndex(), 0.0 );
    }
    
    // finally store the new value
    delete value;
    value = psi;
    
}



/**
 * Swap the parameters held by this distribution.
 *
 * 
 * \param[in]    oldP      Pointer to the old parameter.
 * \param[in]    newP      Pointer to the new parameter.
 */
void BirthDeathProcess::swapParameter(const DagNode *oldP, const DagNode *newP) 
{
    
    if ( oldP == rho ) 
    {
        rho = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if ( oldP == origin ) 
    {
        origin = static_cast<const TypedDagNode<double>* >( newP );
    }

}
