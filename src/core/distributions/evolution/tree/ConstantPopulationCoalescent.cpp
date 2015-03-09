
#include "Clade.h"
#include "ConstantPopulationCoalescent.h"
#include "DistributionExponential.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathCombinatorialFunctions.h"
#include "TopologyNode.h"
#include "Topology.h"

#include <algorithm>
#include <cmath>

using namespace RevBayesCore;

ConstantPopulationCoalescent::ConstantPopulationCoalescent(const TypedDagNode<double>       *N,
                                                           const std::vector<Taxon>         &tn,
                                                           const std::vector<Clade>         &c) :
    TypedDistribution<TimeTree>( NULL ),
    constraints( c ),
    Ne( N ),
    numTaxa( tn.size() ),
    taxa( tn )
{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( Ne );
    
    double lnFact = RbMath::lnFactorial( int(numTaxa) );
    
    logTreeTopologyProb = (numTaxa - 1) * RbConstants::LN2 - 2.0 * lnFact - std::log( numTaxa ) ;
    
    simulateTree();
}



ConstantPopulationCoalescent::~ConstantPopulationCoalescent()
{
    
}



void ConstantPopulationCoalescent::attachTimes(TimeTree *psi, std::vector<TopologyNode *> &tips, size_t index, const std::vector<double> &times) {
    
    if (index < numTaxa-1) 
    {
        // Get the rng
        RandomNumberGenerator* rng = GLOBAL_RNG;
        
        // randomly draw one node from the list of tips
        size_t tip_index = static_cast<size_t>( floor(rng->uniform01()*tips.size()) );
        
        // get the node from the list
        TopologyNode* parent = tips.at(tip_index);
        psi->setAge( parent->getIndex(), times[numTaxa - index - 2] );
        
        // remove the randomly drawn node from the list
        tips.erase(tips.begin()+long(tip_index));
        
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
        attachTimes(psi, tips, index+1, times);
    }
    
}


void ConstantPopulationCoalescent::buildRandomBinaryTree(std::vector<TopologyNode*> &tips)
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


ConstantPopulationCoalescent* ConstantPopulationCoalescent::clone( void ) const
{
    
    return new ConstantPopulationCoalescent( *this );
}


double ConstantPopulationCoalescent::computeLnProbability( void )
{
    
    // variable declarations and initialization
    double lnProbTimes = 0;
    
    
    if ( !matchesConstraints() ) 
    {
        return RbConstants::Double::neginf;
    }
    
    
    // retrieved the speciation times
    std::vector<double> ages;
    for (size_t i = 0; i < value->getNumberOfInteriorNodes()+1; ++i) 
    {
        const TopologyNode& n = value->getInteriorNode( i );
        double a = n.getAge();
        ages.push_back(a);
    }
    // sort the vector of times in ascending order
    std::sort(ages.begin(), ages.end());
            
    
    for (size_t i = 0; i < ages.size(); ++i) 
    {
        size_t j = numTaxa - i;
        double theta = 1.0 / (2.0*Ne->getValue());
        double nPairs = j * (j-1) / 2.0;
        lnProbTimes += log( nPairs * 2.0 / theta ) - nPairs * 2.0 / theta * ages[i] ;
    }
        
    return lnProbTimes + logTreeTopologyProb;
    
}


bool ConstantPopulationCoalescent::matchesConstraints( void )
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



void ConstantPopulationCoalescent::redrawValue( void )
{
    
    simulateTree();
    
}





void ConstantPopulationCoalescent::simulateTree( void )
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
        nodes.erase(nodes.begin()+long(index));
        
        // set name
        const std::string& name = taxa[i].getName();
        node->setName(name);
        node->setSpeciesName(taxa[i].getSpeciesName());
    }
    
    // initialize the topology by setting the root
    tau->setRoot(root);
    
    // connect the tree with the topology
    psi->setTopology( tau, true );
    
    // now simulate the ages
    
    // allocate the vector for the times
    std::vector<double> coalescentTimes = std::vector<double>(numTaxa-1,0.0);
    
    double theta = 1.0 / (2.0*Ne->getValue());
    // draw a time for each speciation event condition on the time of the process
	for (size_t i = 0; i < numTaxa; ++i) 
    {
        double prevCoalescentTime = 0.0;
        if ( i > 0 ) 
        {
            prevCoalescentTime = coalescentTimes[i-1];
        }
        
        size_t j = numTaxa - i;
        double nPairs = j * (j-1) / 2.0;
        double lambda = nPairs * theta / 2.0;
        double u = RbStatistics::Exponential::rv( lambda, *rng);
		coalescentTimes[i] = prevCoalescentTime + u;
	}
        
    nodes.clear();
    nodes.push_back( root );
    attachTimes(psi, nodes, 0, coalescentTimes);
//    psi->setAge( root->getIndex(), coalescentTimes[coalescentTimes.size()-1] );
    for (size_t i = 0; i < numTaxa; ++i)
    {
        TopologyNode& node = tau->getTipNode(i);
        psi->setAge( node.getIndex(), 0.0 );
    }
    
    // finally store the new value
    delete value;
    value = psi;
    
}


/** Swap a parameter of the distribution */
void ConstantPopulationCoalescent::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    if (oldP == Ne)
    {
        Ne = static_cast<const TypedDagNode<double>* >( newP );
    }
}
