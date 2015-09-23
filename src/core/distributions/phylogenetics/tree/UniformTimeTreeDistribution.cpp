#include "Clade.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathCombinatorialFunctions.h"
#include "StochasticNode.h"
#include "TopologyNode.h"
#include "UniformTimeTreeDistribution.h"

#include <algorithm>
#include <cmath>

using namespace RevBayesCore;

UniformTimeTreeDistribution::UniformTimeTreeDistribution(
                                                            const TypedDagNode<double>*         originT,
                                                            const std::vector<std::string>&     taxaNames
                                                         )
    :   TypedDistribution<Tree>( new Tree() ),
        originTime( originT ),
        taxonNames( taxaNames )
{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( originTime );
    
    numTaxa = taxonNames.size();
    
    simulateTree();
    
}


UniformTimeTreeDistribution::~UniformTimeTreeDistribution()
{
    
}


/**
 * Recursive call to attach ordered interior node times to the time tree psi. Call it initially with the
 * root of the tree.
 */
void UniformTimeTreeDistribution::attachTimes(
                                                Tree*                           psi,
                                                std::vector<TopologyNode *>&    nodes,
                                                size_t                          index,
                                                const std::vector<double>&      interiorNodeTimes,
                                                double                          originTime
                                              )
{
    
    if (index < numTaxa-1)
    {
        // Get the rng
        RandomNumberGenerator* rng = GLOBAL_RNG;
        
        // Randomly draw one node from the list of nodes
        size_t node_index = static_cast<size_t>( floor(rng->uniform01()*nodes.size()) );
        
        // Get the node from the list
        TopologyNode* parent = nodes.at(node_index);
        psi->getNode( parent->getIndex() ).setAge( originTime - interiorNodeTimes[index] );
        
        // Remove the randomly drawn node from the list
        nodes.erase(nodes.begin()+long(node_index));
        
        // Add the left child if an interior node
        TopologyNode* leftChild = &parent->getChild(0);
        if ( !leftChild->isTip() )
        {
            nodes.push_back(leftChild);
        }

        // Add the right child if an interior node
        TopologyNode* rightChild = &parent->getChild(1);
        if ( !rightChild->isTip() )
        {
            nodes.push_back(rightChild);
        }
        
        // Recursive call to this function
        attachTimes(psi, nodes, index+1, interiorNodeTimes, originTime);
    }
}


/** Build random binary tree to size numTaxa. The result is a draw from the uniform distribution on histories. */
void UniformTimeTreeDistribution::buildRandomBinaryHistory(std::vector<TopologyNode*> &tips) {
    
    if (tips.size() < numTaxa)
    {
        // Get the rng
        RandomNumberGenerator* rng = GLOBAL_RNG;

        // Randomly draw one node from the list of tips
        size_t index = static_cast<size_t>( floor(rng->uniform01()*tips.size()) );

        // Get the node from the list
        TopologyNode* parent = tips.at(index);
        
        // Remove the randomly drawn node from the list
        tips.erase(tips.begin()+long(index));
        
        // Add a left child
        TopologyNode* leftChild = new TopologyNode(0);
        parent->addChild(leftChild);
        leftChild->setParent(parent);
        tips.push_back(leftChild);
        
        // Add a right child
        TopologyNode* rightChild = new TopologyNode(0);
        parent->addChild(rightChild);
        rightChild->setParent(parent);
        tips.push_back(rightChild);
        
        // Recursive call to this function
        buildRandomBinaryHistory(tips);
    }
}


/* Clone function */
UniformTimeTreeDistribution* UniformTimeTreeDistribution::clone( void ) const {
    
    return new UniformTimeTreeDistribution( *this );
}


/* Compute probability */
double UniformTimeTreeDistribution::computeLnProbability( void ) {
    
    // Variable declarations and initialization
    double lnProb = 0.0;
    double originT = originTime->getValue();
    
    // we need to check as well that all ages are smaller than the origin
    // this can simply be checked if the root age is smaller than the origin
    if ( originT < value->getRoot().getAge() ) 
    {
        return RbConstants::Double::neginf;
    }
    
    // Take the uniform draws into account
    lnProb = (numTaxa - 2) * log( 1.0 / originT );

    // Take the ordering effect into account
    lnProb += RbMath::lnFactorial( int(numTaxa - 2) );

    // We return now; apparently we are not responsible for the topology probability
    return lnProb;
}


void UniformTimeTreeDistribution::redrawValue( void ) {
    simulateTree();
}


/** Simulate the tree conditioned on the time of origin */
void UniformTimeTreeDistribution::simulateTree( void ) {
    
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;

    // Create the time tree object (topology + times)
    Tree *psi = new Tree();

    // Root the topology by setting the appropriate flag
    psi->setRooted( true );
    
    // Create the root node and a vector of nodes
    TopologyNode* root = new TopologyNode();
    std::vector<TopologyNode* > nodes;
    nodes.push_back(root);
    
    // Draw a random tree history
    buildRandomBinaryHistory(nodes);
    
    // Set the tip names
    for (size_t i=0; i<numTaxa; i++) {
        size_t index = size_t( floor(rng->uniform01() * nodes.size()) );
        
        // Get the node from the list
        TopologyNode* node = nodes.at(index);
        
        // Remove the randomly drawn node from the list
        nodes.erase(nodes.begin()+long(index) );
        
        // Set name
        std::string& name = taxonNames[i];
        node->setName(name);
    }
    
    // Initialize the topology by setting the root
    psi->setRoot(root);
    
    // Now simulate the speciation times counted from originTime
    std::vector<double> intNodeTimes;
    double              t_or = originTime->getValue();
    intNodeTimes.push_back( 0.0 );  // For time of mrca
    for ( size_t i=0; i<numTaxa-2; i++ )
    {
        double t = rng->uniform01() * t_or;
        intNodeTimes.push_back( t );
    }
    
    // Sort the speciation times from 0.0 (root node) to the largest value
    std::sort( intNodeTimes.begin(), intNodeTimes.end() );

    // Attach times
    nodes.clear();
    nodes.push_back( root );
    attachTimes(psi, nodes, 0, intNodeTimes, t_or);
    for (size_t i = 0; i < numTaxa; ++i) {
        TopologyNode& node = psi->getTipNode(i);
        psi->getNode( node.getIndex() ).setAge( 0.0 );
    }
    
    // Finally store the new value
    value = psi;
    
}

void UniformTimeTreeDistribution::getAffected(std::set<DagNode *> &affected, RevBayesCore::DagNode *affecter)
{
    
    if ( affecter == originTime)
    {
        dagNode->getAffectedNodes( affected );
    }
    
}

/**
 * Keep the current value and reset some internal flags. Nothing to do here.
 */
void UniformTimeTreeDistribution::keepSpecialization(DagNode *affecter)
{
    
    if ( affecter == originTime )
    {
        dagNode->keepAffected();
    }
    
}

/**
 * Restore the current value and reset some internal flags.
 * If the root age variable has been restored, then we need to change the root age of the tree too.
 */
void UniformTimeTreeDistribution::restoreSpecialization(DagNode *affecter)
{
    
    if ( affecter == originTime )
    {
        value->getNode( value->getRoot().getIndex() ).setAge( originTime->getValue() );
        dagNode->restoreAffected();
    }
    
}

/** Swap a parameter of the distribution */
void UniformTimeTreeDistribution::swapParameterInternal( const DagNode *oldP, const DagNode *newP )
{
    if (oldP == originTime)
    {
        originTime = static_cast<const TypedDagNode<double>* >( newP );
    }
}

/**
 * Touch the current value and reset some internal flags.
 * If the root age variable has been restored, then we need to change the root age of the tree too.
 */
void UniformTimeTreeDistribution::touchSpecialization(DagNode *affecter, bool touchAll)
{
    
    if ( affecter == originTime )
    {
        value->getNode( value->getRoot().getIndex() ).setAge( originTime->getValue() );
        dagNode->touchAffected();
    }
    
}
