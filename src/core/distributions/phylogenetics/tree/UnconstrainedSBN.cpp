#include "UnconstrainedSBN.h"
#include "Clade.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "TopologyNode.h"

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
      return( RbConstants::Double::neginf );
    }

    Subsplit root_split = Subsplit(root_children[0]->getClade(),root_children[1]->getClade(),taxa);

    lnProbability += parameters.computeRootSplitProbability(root_split);

    // loop over all internal nodes, calculate probabilities
    // TODO: We could do this more efficiently by travesing the tree, since we would not have to find the subsplit every node belongs to every time
    for (size_t i=0; i < value->getNumberOfInteriorNodes(); ++i)
    {
      const TopologyNode &this_node = value->getNode(i);
      if (!this_node.isRoot())
      {
        // Turn this node into a subsplit
        const std::vector<TopologyNode*>& these_children = this_node.getChildren();

        if ( these_children.size() != 2 )
        {
          return( RbConstants::Double::nan );
        }

        Subsplit this_split = Subsplit(these_children[0]->getClade(),these_children[1]->getClade(), taxa);

        // Turn parent node into subsplit
        const TopologyNode &this_node_parent = this_node.getParent();

        const std::vector<TopologyNode*>& parent_children = this_node_parent.getChildren();

        if ( parent_children.size() != 2 )
        {
          return( RbConstants::Double::nan );
        }

        Subsplit parent_split = Subsplit(parent_children[0]->getClade(),parent_children[1]->getClade(),taxa);

        lnProbability += parameters.computeSubsplitTransitionProbability(parent_split, this_split);

      }
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

    // // create the tip nodes
    // std::vector<TopologyNode*> nodes;
    // for (size_t i=0; i<num_taxa; ++i)
    // {
    //
    //     // create the i-th taxon
    //     TopologyNode* node = new TopologyNode( taxa[i], i );
    //     nodes.push_back(node);
    // }

    // List of active tree nodes/subsplits
    // We pair them such that each tree node corresponds to the subsplit it defines
    std::vector<std::pair<Subsplit,TopologyNode*> > active;

    size_t index = taxa.size()+1;
    size_t tip_index = 0;

    // Root split
    double u = rng->uniform01();
    // TopologyNode* root = new TopologyNode( taxa.size() );
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
        Clade tip = Y_child.asClade();
        Y_child_node = new TopologyNode( tip_index++ );
        // Y_child_node = new TopologyNode();
        Y_child_node->setTaxon(tip.getTaxa()[0]);
        // Y_child_node->setNodeType(true, false, false);
        Y_child_node->setName(tip.getTaxa()[0].getName());
      }
      else
      {
        // This is an internal node
        // Y_child_node = new TopologyNode( index++ );
        Y_child_node = new TopologyNode();
        // Y_child_node->setNodeType(false, false, true);
        active.push_back(std::make_pair(Y_child,Y_child_node));
      }

      // Choose subsplit of Z
      Subsplit Z_child = parameters.drawSubsplitForZ(this_parent_subsplit);
      if ( Z_child.isFake() )
      {
        // This is a tip, we don't add it to the active pile
        Clade tip = Z_child.asClade();
        Z_child_node = new TopologyNode( tip_index++ );
        // Z_child_node = new TopologyNode();
        // Z_child_node->setNodeType(true, false, false);
        Z_child_node->setTaxon(tip.getTaxa()[0]);
        Z_child_node->setName(tip.getTaxa()[0].getName());
      }
      else
      {
        // This is an internal node
        // Z_child_node = new TopologyNode( index++ );
        Z_child_node = new TopologyNode();
        // Z_child_node->setNodeType(false, false, true);
        active.push_back(std::make_pair(Z_child,Z_child_node));
      }

      this_parent_node->addChild(Y_child_node);
      this_parent_node->addChild(Z_child_node);
      Y_child_node->setParent(this_parent_node);
      Z_child_node->setParent(this_parent_node);

    }

    psi->setRoot(root, true);

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
