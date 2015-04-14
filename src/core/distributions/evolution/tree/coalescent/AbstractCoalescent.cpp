#include "AbstractCoalescent.h"
#include "Clade.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbMathCombinatorialFunctions.h"
#include "StochasticNode.h"
#include "Taxon.h"
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
 * \param[in]    cdt       The condition of the process (time/survival/nTaxa)
 * \param[in]    nTaxa     Number of taxa (used for initialization during simulation).
 * \param[in]    tn        Taxon names used during initialization.
 * \param[in]    c         Clade constraints.
 */
AbstractCoalescent::AbstractCoalescent(const std::vector<Taxon> &tn, const std::vector<Clade> &c) : TypedDistribution<TimeTree>( new TimeTree() ),
    constraints( c ),
    numTaxa( tn.size() ),
    taxa( tn )
{
    
    // the combinatorial factor for the probability of a labelled history is
    // 2^{n-1} / ( n! * (n-1)! )
    // but since the probability of the divergence times contains the factor (n-1)! we simply store
    // 2^{n-1} / n!
    
    double lnFact = RbMath::lnFactorial( int(numTaxa) );
    logTreeTopologyProb = (numTaxa - 1) * RbConstants::LN2 - 2.0 * lnFact - std::log( numTaxa );
// (This is the tree prob for the birth-death)    logTreeTopologyProb = (numTaxa - 1) * RbConstants::LN2 - lnFact ;
    
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
void AbstractCoalescent::attachAges(TimeTree *psi, std::vector<TopologyNode *> &tips, size_t index, const std::vector<double> &ages)
{
    
    if (index < ages.size() )
    {
        // Get the rng
        RandomNumberGenerator* rng = GLOBAL_RNG;
        
        // randomly draw one node from the list of tips
        size_t tip_index = size_t( floor(rng->uniform01()*tips.size()) );
        
        // get the node from the list
        TopologyNode* parent = tips.at(tip_index);
        psi->setAge( parent->getIndex(), ages[index] );
        
        // remove the randomly drawn node from the list
        tips.erase(tips.begin()+ long(tip_index) );
        
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
        attachAges(psi, tips, index+1, ages);
    }
    
    // no return value
}


void AbstractCoalescent::buildRandomBinaryTree(std::vector<TopologyNode*> &tips)
{
    
    if (tips.size() < numTaxa)
    {
        // Get the rng
        RandomNumberGenerator* rng = GLOBAL_RNG;
        
        // randomly draw one node from the list of tips
        size_t index = static_cast<size_t>( floor(rng->uniform01()*tips.size()) );
        
        // get the node from the list
        TopologyNode* parent = tips.at(index);
        
        // remove the randomly drawn node from the list
        tips.erase(tips.begin()+long(index));
        
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
 */
double AbstractCoalescent::computeLnProbability( void )
{
    
    // variable declarations and initialization
    double lnProbTimes = 0;
    
    // first check if the current tree matches the clade constraints
    if ( !matchesConstraints() )
    {
        return RbConstants::Double::neginf;
    }
    
    // check that all children are younger than there parents
    const std::vector<TopologyNode*>& nodes = value->getNodes();
    for (std::vector<TopologyNode*>::const_iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        if ( !(*it)->isRoot() )
        {
            if ( (*it)->getAge() >= (*it)->getParent().getAge() )
            {
                return RbConstants::Double::neginf;
            }
        }
    }
    
    // multiply the probability of a descendant of the initial species
    lnProbTimes += computeLnProbabilityTimes();
    
    return lnProbTimes + logTreeTopologyProb;
    
}



/**
 * We check here if all the constraints are satisfied.
 * These are hard constraints, that is, the clades must be monophyletic.
 *
 * \return     True if the constraints are matched, false otherwise.
 */
bool AbstractCoalescent::matchesConstraints( void )
{
    
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
 * Redraw the current value. We delegate this to the simulate method.
 */
void AbstractCoalescent::redrawValue( void )
{
    
    simulateTree();
    
}


/**
 *
 */
void AbstractCoalescent::simulateTree( void )
{
    
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
        nodes.erase( nodes.begin()+long(index) );
        
        // set name
        const std::string& name = taxa[i].getName();
        node->setName(name);
        node->setSpeciesName(taxa[i].getSpeciesName());
    }
    
    // initialize the topology by setting the root
    tau->setRoot(root);
    
    // connect the tree with the topology
    psi->setTopology( tau, true );
    
    // now simulate the speciation times
    size_t numInitialSpecies = 1;
    
    nodes.clear();
    
    if ( numInitialSpecies < numTaxa)
    {
        // draw a time for each speciation event condition on the time of the process
        std::vector<double> times = simulateCoalescentTime(numTaxa-numInitialSpecies);
        
        // add a left child
        TopologyNode* leftChild = &root->getChild(0);
        if ( !leftChild->isTip() )
        {
            nodes.push_back(leftChild);
        }
            
        // add a right child
        TopologyNode* rightChild = &root->getChild(1);
        if ( !rightChild->isTip() )
        {
            nodes.push_back(rightChild);
        }
        
        attachAges(psi, nodes, 0, times);
        
    }
    
    // \todo Why are we doing this? (Sebastian)
    for (size_t i = 0; i < numTaxa; ++i)
    {
        TopologyNode& node = tau->getTipNode(i);
        psi->setAge( node.getIndex(), 0.0 );
    }
    
    // reset the listeners
    const std::set<TreeChangeEventListener*> l = value->getTreeChangeEventHandler().getListeners();
    
    for (std::set<TreeChangeEventListener*>::const_iterator it = l.begin(); it != l.end(); ++it)
    {
        value->getTreeChangeEventHandler().removeListener( *it );
        psi->getTreeChangeEventHandler().addListener( *it );
    }
    
    // finally store the new value
    delete value;
    value = psi;
    
}