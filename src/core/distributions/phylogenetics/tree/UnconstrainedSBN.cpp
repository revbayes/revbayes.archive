#include <boost/foreach.hpp>
#include "Clade.h"
#include "DistributionGamma.h"
#include "DistributionLognormal.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "TopologyNode.h"
#include "UnconstrainedSBN.h"

#include <algorithm>
#include <cmath>

using namespace RevBayesCore;

UnconstrainedSBN::UnconstrainedSBN(const SBNParameters parameters, bool rooted) : TypedDistribution<Tree>( new Tree() ),
    parameters( parameters ),
	  rooted( rooted ),
    taxa( parameters.getTaxa() )
{

    // Class SBNParameters handles parameterization of these edge_length_distributions
    // Here we simply use those parameters
    // Parameters are set either by calling a learn___() function or reading in an SBN
    simulateTree();

}


UnconstrainedSBN::~UnconstrainedSBN()
{
    // the tree will be deleted automatically by the base class

}


UnconstrainedSBN* UnconstrainedSBN::clone( void ) const
{

    return new UnconstrainedSBN( *this );
}

double UnconstrainedSBN::computeLnProbability( void )
{
    double lnProbability = 0.0;

    // Here we compute the probability of the tree according to the SBN
    // If it is rooted, this is a single call to computeLnProbabilityGivenRoot
    // If not, we must marginalize over the root location
    if ( rooted )
    {
      lnProbability = computeLnProbabilityGivenRoot();
    }
    else
    {
      TopologyNode &initial_root = value->getRoot();

      // sum over rooting locations
      // TODO: with appropriate message passing we could calculate all this in one pass through the tree
      for (size_t ri=0; ri < value->getNumberOfNodes(); ++ri)
      {
        value->reroot(value->getNode(ri),false);
        lnProbability = logSum(lnProbability,computeLnProbabilityGivenRoot());
      }

      // Put root back where we found it
      value->reroot(initial_root,false);
    }

    return lnProbability;
}

double UnconstrainedSBN::computeLnProbabilityGivenRoot( void )
{
    double lnProbability = 0.0;

    // first compute root split probability
    const TopologyNode &root = value->getRoot();
    const std::vector<TopologyNode*>& root_children = root.getChildren();

    if ( root_children.size() != 2 )
    {
      return( RbConstants::Double::nan );
    }

    Subsplit root_split = value->getRootSubsplit(taxa);

    lnProbability += parameters.computeRootSplitProbability(root_split);

    // get all parent-child subsplit pairs, calculate their probabilities
    // TODO: We could do this more efficiently by travesing the tree, since we would not have to find the subsplit every node belongs to every time
    std::vector<std::pair<Subsplit,Subsplit> > parent_child_subsplits = value->getAllSubsplitParentChildPairs(taxa);

    std::pair<Subsplit,Subsplit> parent_child_pair;

    BOOST_FOREACH(parent_child_pair, parent_child_subsplits) {
      lnProbability += parameters.computeSubsplitTransitionProbability(parent_child_pair.first, parent_child_pair.second);
    }


    std::map<std::pair<Subsplit,Subsplit>,std::pair<double,double> > edge_length_params = parameters.getEdgeLengthDistributionParameters();

    // Get branch lengths
    const std::vector<TopologyNode*> tree_nodes = value->getNodes();
    for (size_t i=0; i<tree_nodes.size(); ++i)
    {
      if (!tree_nodes[i]->isRoot())
      {
        Subsplit this_split = tree_nodes[i]->getSubsplit(taxa);
        Subsplit this_parent = tree_nodes[i]->getParent().getSubsplit(taxa);

        std::pair<Subsplit,Subsplit> this_parent_child;
        this_parent_child.first = this_parent;
        this_parent_child.second = this_split;

        std::pair<double,double> these_params = edge_length_params[this_parent_child];

        lnProbability += RbStatistics::Lognormal::pdf(these_params.first, these_params.second, tree_nodes[i]->getBranchLength());

      }
    }

    if ( !(value->isBinary()) )
    {
      return RbConstants::Double::nan;
    }
    return lnProbability;
}


double UnconstrainedSBN::logSum( double x1, double x2 )
{
    double offset = std::max(x1,x2);

    x1 -= offset;
    x2 -= offset;

    double exp_x1 = exp(x1);
    double exp_x2 = exp(x2);

    return log(exp_x1 + exp_x2) + offset;
}

void UnconstrainedSBN::redrawValue( void )
{
    simulateTree();
}

void UnconstrainedSBN::setValue(RevBayesCore::Tree *v, bool force)
{

    // delegate to super class
    TypedDistribution<Tree>::setValue( v, force );

}


void UnconstrainedSBN::simulateTree( void )
{
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;

    // the tree object
    // Tree *psi = value;
    Tree *psi = new Tree();

    // internally we treat unrooted topologies the same as rooted
    psi->setRooted( rooted );

    // For drawing branch lengths
    std::map<std::pair<Subsplit,Subsplit>,std::pair<double,double> > edge_length_params = parameters.getEdgeLengthDistributionParameters();

    // create the tip nodes
    std::vector<TopologyNode*> tip_nodes;
    for (size_t i=0; i<taxa.size(); ++i)
    {
        // create the i-th taxon
        TopologyNode* node = new TopologyNode( taxa[i], i );
        tip_nodes.push_back(node);
    }

    // List of active tree nodes/subsplits
    // We pair them such that each tree node corresponds to the subsplit it defines
    std::vector<std::pair<Subsplit,TopologyNode*> > active;

    // Root split
    double u = rng->uniform01();
    TopologyNode* root = new TopologyNode();
    root->setNodeType(false, true, false);
    Subsplit root_split = parameters.drawRootSplit();
    active.push_back(std::make_pair(root_split,root));

    // All other subplits
    while (active.size() > 0)
    {
      // Get a node/subsplit to work on, remove that from list
      std::pair<Subsplit,TopologyNode*> this_parent = active.back();
      active.pop_back();

      Subsplit this_parent_subsplit = this_parent.first;
      TopologyNode* this_parent_node = this_parent.second;

      TopologyNode* Y_child_node;
      TopologyNode* Z_child_node;

      // Choose subsplit of Y
      Subsplit Y_child = parameters.drawSubsplitForY(this_parent_subsplit);
      if ( Y_child.isFake() )
      {
        // This is a tip, we don't add it to the active pile
        Y_child_node = tip_nodes[Y_child.getYBitset().getFirstSetBit()];
      }
      else
      {
        // This is an internal node
        Y_child_node = new TopologyNode();
        active.push_back(std::make_pair(Y_child,Y_child_node));
      }

      // Add branchlength to Y
      std::pair<Subsplit,Subsplit> this_parent_child;
      this_parent_child.first = this_parent_subsplit;
      this_parent_child.second = Y_child;

      std::pair<double,double> these_params = edge_length_params[this_parent_child];

      double brlen  = RbStatistics::Lognormal::rv(these_params.first, these_params.second, *rng);
      Y_child_node->setBranchLength(brlen,false);

      // Choose subsplit of Z
      Subsplit Z_child = parameters.drawSubsplitForZ(this_parent_subsplit);
      if ( Z_child.isFake() )
      {
        // This is a tip, we don't add it to the active pile
        Z_child_node = tip_nodes[Z_child.getYBitset().getFirstSetBit()];
      }
      else
      {
        // This is an internal node
        Z_child_node = new TopologyNode();
        active.push_back(std::make_pair(Z_child,Z_child_node));
      }

      // Add branchlength to Z
      this_parent_child.first = this_parent_subsplit;
      this_parent_child.second = Z_child;

      these_params = edge_length_params[this_parent_child];
      brlen = RbStatistics::Lognormal::rv(these_params.first, these_params.second, *rng);
      Z_child_node->setBranchLength(brlen,false);

      // Attach nodes to eachother
      this_parent_node->addChild(Y_child_node);
      this_parent_node->addChild(Z_child_node);
      Y_child_node->setParent(this_parent_node);
      Z_child_node->setParent(this_parent_node);

    }

    psi->setRoot(root, true);

    // TODO: with the selection of taxa for tips using bitsets, taxon names are paired to indices, the following may be extraneous
    // re-couple tip node names with tip indices
    // this is necessary because otherwise tip names get scrambled across replicates
    for (size_t i=0; i<taxa.size(); i++)
    {
    	psi->getTipNodeWithName(taxa[i].getName()).setIndex(i);
    }

    psi->orderNodesByIndex();

    // initialize the topology by setting the root
    // psi->setRoot(root, true);

    // finally store the new value
    delete value;
    value = psi;

}


/** Swap a parameter of the distribution */
void UnconstrainedSBN::swapParameterInternal( const DagNode *oldP, const DagNode *newP )
{
  // SBNs have their parameters set once and they do not change afterwards
}

// /**
//  * We check here if all the constraints are satisfied.
//  * These are hard constraints, that is, the clades must be monophyletic.
//  *
//  * \return     True if the constraints are matched, false otherwise.
//  */
// bool UnconstrainedSBN::matchesConstraints( void )
// {
//
//     if ( constraints.empty() == true )
//     {
// 		return true;
// 	}
//     else
//     {
//
// 		const TopologyNode &root = value->getRoot();
// 		for (std::vector<Clade>::iterator it = constraints.begin(); it != constraints.end(); ++it)
// 		{
// 			if ( root.containsClade( *it, true ) == false )
// 			{
// 				return false;
// 			}
// 		}
//
// 		return true;
// 	}
//
// }
