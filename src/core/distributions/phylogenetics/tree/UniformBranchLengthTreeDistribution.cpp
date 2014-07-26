//
//  UniformBranchLengthTreeDistribution.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 5/17/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#include "UniformBranchLengthTreeDistribution.h"

#include "Clade.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathCombinatorialFunctions.h"
#include "TopologyNode.h"
#include "Topology.h"

#include <algorithm>
#include <cmath>

using namespace RevBayesCore;

UniformBranchLengthTreeDistribution::UniformBranchLengthTreeDistribution(const TypedDagNode<double>* maxBrLen, const std::vector<std::string>& taxaNames) :  TypedDistribution<BranchLengthTree>( new BranchLengthTree() ),
    maxBranchLength( maxBrLen ),
    taxonNames( taxaNames )
{
    
    numTaxa = taxonNames.size();
    
    simulateTree();
    
}


UniformBranchLengthTreeDistribution::UniformBranchLengthTreeDistribution(const UniformBranchLengthTreeDistribution &x) : TypedDistribution<BranchLengthTree>( x ),
    maxBranchLength( x.maxBranchLength ),
    numTaxa( x.numTaxa ),
    taxonNames( x.taxonNames )
{
}


UniformBranchLengthTreeDistribution::~UniformBranchLengthTreeDistribution()
{
    
}

/** Build random binary tree to size numTaxa. The result is a draw from the uniform distribution on histories. */
void UniformBranchLengthTreeDistribution::buildRandomBinaryHistory(std::vector<TopologyNode*> &tips) {
    
    if (tips.size() < numTaxa)
    {
        // Get the rng
        RandomNumberGenerator* rng = GLOBAL_RNG;
        
        // Randomly draw one node from the list of tips
        size_t index = static_cast<size_t>( floor(rng->uniform01()*tips.size()) );
        
        // Get the node from the list
        TopologyNode* parent = tips.at(index);
        
        // Remove the randomly drawn node from the list
        tips.erase( tips.begin()+long(index) );
        
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
UniformBranchLengthTreeDistribution* UniformBranchLengthTreeDistribution::clone( void ) const {
    
    return new UniformBranchLengthTreeDistribution( *this );
}


/* Compute probability */
double UniformBranchLengthTreeDistribution::computeLnProbability( void ) {
    
    const BranchLengthTree& tree = *this->value;
    const std::vector<TopologyNode*>& nodes = tree.getNodes();
    
    // return -inf if any branchlength exceeds maxBranchLength.value
    for (size_t i = 0; i < nodes.size(); i++)
        if (nodes[i]->getBranchLength() > maxBranchLength->getValue())
            return RbConstants::Double::neginf;
        
    return 0.0;
}


void UniformBranchLengthTreeDistribution::redrawValue( void ) {
    simulateTree();
}


/** Simulate the tree conditioned on the time of origin */
void UniformBranchLengthTreeDistribution::simulateTree( void ) {
    
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // Create the time tree object (topology + times)
    BranchLengthTree *psi = new BranchLengthTree();
    
    // Create an empty topology
    Topology *tau = new Topology();
    
    // Root the topology by setting the appropriate flag
    tau->setRooted( true );
    
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
        nodes.erase(nodes.begin()+long(index));
        
        // Set name
        std::string& name = taxonNames[i];
        node->setName(name);
    }
    
    // Initialize the topology by setting the root
    tau->setRoot(root);
    
    // Connect the tree with the topology
    psi->setTopology( tau, true );
    
    // Now simulate the speciation times counted from originTime
    size_t numBranches = tau->getNumberOfNodes() - 1;
    std::vector<double> nodeTimes;
    for ( size_t i=0; i<numBranches; i++ )
    {
        TopologyNode* nd = &tau->getNode(i);
        double t = rng->uniform01() * maxBranchLength->getValue();
        psi->setBranchLength(nd->getIndex(), t);
    }
    
    // Finally store the new value
    value = psi;
    
}


/** Get the parameters of the distribution */
std::set<const DagNode*> UniformBranchLengthTreeDistribution::getParameters( void ) const
{
    std::set<const DagNode*> parameters;
    
    parameters.insert( maxBranchLength );
    
    parameters.erase( NULL );
    return parameters;
}


/** Swap a parameter of the distribution */
void UniformBranchLengthTreeDistribution::swapParameter( const DagNode *oldP, const DagNode *newP )
{
    if (oldP == maxBranchLength)
    {
        maxBranchLength = static_cast<const TypedDagNode<double>* >( newP );
    }
}
