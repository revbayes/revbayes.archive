
#include "UniformTopologyDistribution.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "Topology.h"
#include "TopologyNode.h"

#include <algorithm>
#include <cmath>

using namespace RevBayesCore;

UniformTopologyDistribution::UniformTopologyDistribution(size_t nTaxa, const std::vector<std::string> &tn) : TypedDistribution<Topology>( new Topology() ),
    numTaxa( nTaxa ),
    taxonNames( tn ),
    logTreeTopologyProb( RbConstants::Double::nan )
{
    
    double lnFact = 0.0;
    for (size_t i = 2; i < numTaxa; i++)
    {
        lnFact += std::log(i);
    }
    
    logTreeTopologyProb = (numTaxa - 1) * RbConstants::LN2 - 2.0 * lnFact - std::log( numTaxa ) ;
    
    simulateTree();
    
}


UniformTopologyDistribution::~UniformTopologyDistribution()
{
    // the tree will be deleted automatically by the base class
    
}


void UniformTopologyDistribution::buildRandomBinaryTree(std::vector<TopologyNode*> &tips) {
    
    if (tips.size() < numTaxa) {
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


UniformTopologyDistribution* UniformTopologyDistribution::clone( void ) const {
    return new UniformTopologyDistribution( *this );
}


double UniformTopologyDistribution::computeLnProbability( void ) {
    
    return logTreeTopologyProb;
    
}


void UniformTopologyDistribution::redrawValue( void ) {
    simulateTree();
}


void UniformTopologyDistribution::simulateTree( void ) {
    
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    TopologyNode* root = new TopologyNode();
    std::vector<TopologyNode* > nodes;
//    nodes.push_back(root);
        
    
    // add a left child
    TopologyNode* leftChild = new TopologyNode(0);
    root->addChild(leftChild);
    leftChild->setParent(root);
    nodes.push_back(leftChild);
    
    // add a middle child
    TopologyNode* middleChild = new TopologyNode(0);
    root->addChild(middleChild);
    middleChild->setParent(root);
    nodes.push_back(middleChild);
    
    // add a right child
    TopologyNode* rightChild = new TopologyNode(0);
    root->addChild(rightChild);
    rightChild->setParent(root);
    nodes.push_back(rightChild);
    
    // recursively build the tree
    buildRandomBinaryTree(nodes);
    
    // set tip names
    for (size_t i=0; i<numTaxa; i++) {
        size_t index = size_t( floor(rng->uniform01() * nodes.size()) );
        
        // get the node from the list
        TopologyNode* node = nodes.at(index);
        
        // remove the randomly drawn node from the list
        nodes.erase(nodes.begin()+long(index));
        
        // set name
        std::string& name = taxonNames[i];
        node->setName(name);
    }
    
    value->setRoot( root );
    
}



/** Get the parameters of the distribution */
std::set<const DagNode*> UniformTopologyDistribution::getParameters( void ) const
{
    std::set<const DagNode*> parameters;
    
    parameters.erase( NULL );
    return parameters;
}


/** Swap a parameter of the distribution */
void UniformTopologyDistribution::swapParameter( const DagNode *oldP, const DagNode *newP )
{
}
