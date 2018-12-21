#include "UnrootedSBN.h"
#include "Clade.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "TopologyNode.h"

#include <algorithm>
#include <cmath>

using namespace RevBayesCore;

// UnrootedSBN::UnrootedSBN(const std::vector<Taxon> &tn, const std::vector<Subsplit> &ss, const std::vector<long> &rs, const std::vector<double> &rp, const std::vector<std::vector<std::vector<long> > > &sc, const std::vector<std::vector<std::vector<double> > > &sp) : TypedDistribution<Tree>( new Tree() ),
UnrootedSBN::UnrootedSBN(const std::vector<Taxon> &tn, const std::vector<Subsplit> &ss, const std::vector<long> &rs, const std::vector<double> &rp, const RbVector<RbVector<RbVector<long> > > sc, const RbVector<RbVector<RbVector<double> > > sp) : TypedDistribution<Tree>( new Tree() ),
    num_taxa( tn.size() ),
    taxa( tn ),
	  subsplits( ss ),
    root_splits( rs ),
    root_split_probabilities( rp ),
    subsplit_children( sc ),
    subsplit_probabilities( sp ),
    subsplit_bitset_map (  ),
    root_split_probability_map (  ),
    subsplit_parent_child_probability_pair_map(  ),
	  rooted( false )
{
    // We will (probably) want a single object that contains all these things eventually, otherwise someone could change the size of root_splits and update not the probabilities and all hell would break loose
    // addParameter(root_splits);
    // addParameter(root_split_probabilities);
    // addParameter(subsplit_children);
    // addParameter(subsplit_probabilities);

    std::vector<Taxon> unordered_taxa = taxa;
    std::vector<std::string> ordered_taxa;
    for (size_t i = 0; i < unordered_taxa.size(); ++i)
    {
        ordered_taxa.push_back(unordered_taxa[i].getName());
    }

    // order taxon names
    std::sort(ordered_taxa.begin(), ordered_taxa.end());

    // Setup taxon bitset map
    std::map<Taxon, size_t> taxon_bitset_map;
    // add taxa to bitset map
    for (size_t i = 0; i < ordered_taxa.size(); ++i)
    {
        taxon_bitset_map[ordered_taxa[i]] = i;
    }

    // Setup map of subplits to indices
    std::map<Subsplit, size_t> subsplit_bitset_map;
    // add taxa to bitset map
    for (size_t i = 0; i < subsplits.size(); ++i)
    {
        subsplit_bitset_map[subsplits[i]] = i;
    }

    // Set up map of root splits to probabilities
    for (size_t i=0; i<root_splits.size(); ++i)
    {
      root_split_probability_map[subsplits[root_splits[i]]] = root_split_probabilities[i];
    }
    // Setup map from subsplit pair (by index) to transition probability
    // Also check that indices run from 0:(n-1) and not 1:n
    size_t max_index = 0;
    for (size_t i=0; i<subsplit_children.size(); ++i)
    {
      for (size_t j=0; j<2; ++j)
      {
        for (size_t k=0; k<subsplit_children[i][j].size(); ++k)
        {
          size_t this_child = subsplit_children[i][j][k];
          std::pair<size_t,size_t> parent_child = std::make_pair(i,this_child);
          subsplit_parent_child_probability_pair_map[parent_child] = subsplit_probabilities[i][j][k];
          if (this_child > max_index)
          {
            max_index = this_child;
          }
        }
      }
    }

    if (max_index != subsplits.size() - 1) {
      throw(RbException("Subsplits must be indexed starting at 0."));
    }
    // RbBitSet b( num_taxa );
    // for (size_t j = 0; j < outgroup.size(); j++)
    // {
    //     size_t k = taxon_bitset_map[ outgroup.getTaxonName(j) ];
    //
    //     b.set(k);
    // }
    //



    simulateTree();

}


UnrootedSBN::~UnrootedSBN()
{
    // the tree will be deleted automatically by the base class

}


UnrootedSBN* UnrootedSBN::clone( void ) const
{

    return new UnrootedSBN( *this );
}

double UnrootedSBN::computeLnProbability( void )
{
    TopologyNode &initial_root = value->getRoot();

    // sum over rooting locations
    // TODO: with appropriate message passing we could calculate all this in one pass through the tree
    double lnProbability = 0.0;
    for (size_t ri=0; ri < value->getNumberOfNodes(); ++ri)
    {
      value->reroot(value->getNode(ri),false);
      lnProbability += computeLnProbabilityGivenRoot();
    }

    // Put root back where we found it
    value->reroot(initial_root,false);

    return lnProbability;
}

double UnrootedSBN::computeLnProbabilityGivenRoot( void )
{
    double lnProbability = 0.0;

    // first compute root split probability
    const TopologyNode &root = value->getRoot();
    const std::vector<TopologyNode*>& root_children = root.getChildren();

    if ( root_children.size() != 2 )
    {
      return( RbConstants::Double::neginf );
    }

    Subsplit root_split = Subsplit(root_children[0]->getClade(),root_children[1]->getClade());
    if ( subsplit_bitset_map.count(root_split) == 0 )
    {
      // Root split is invalid, does not exist in our subsplits
      return( RbConstants::Double::neginf );
    }

    lnProbability += root_split_probability_map[root_split];

    // loop over all internal nodes, calculate probabilities
    // TODO: We could do this more efficiently by travesing the tree, since we would not have to find the subsplit every node belongs to every time
    for (size_t i=0; i < value->getNumberOfInteriorNodes(); ++i)
    {
      const TopologyNode &this_node = value->getNode(i);
      if (!this_node.isRoot())
      {
        const TopologyNode &this_node_parent = this_node.getParent();

        // Turn this node into a subsplit
        const std::vector<TopologyNode*>& these_children = this_node.getChildren();

        if ( these_children.size() != 2 )
        {
          return( RbConstants::Double::neginf );
        }

        Subsplit this_split = Subsplit(these_children[0]->getClade(),these_children[1]->getClade());
        if ( subsplit_bitset_map.count(this_split) == 0 )
        {
          // Subsplit is invalid, does not exist in our subsplits
          return( RbConstants::Double::neginf );
        }

        // Turn parent node into subsplit
        const std::vector<TopologyNode*>& parent_children = this_node_parent.getChildren();

        if ( parent_children.size() != 2 )
        {
          return( RbConstants::Double::neginf );
        }

        Subsplit parent_split = Subsplit(parent_children[0]->getClade(),parent_children[1]->getClade());
        if ( subsplit_bitset_map.count(this_split) == 0 )
        {
          // Subsplit is invalid, does not exist in our subsplits
          return( RbConstants::Double::neginf );
        }

        std::pair<size_t,size_t> parent_child = std::make_pair(subsplit_bitset_map[parent_split],subsplit_bitset_map[this_split]);

        lnProbability += subsplit_parent_child_probability_pair_map[parent_child];
      }
    }

    return lnProbability;
}


void UnrootedSBN::redrawValue( void )
{
    simulateTree();
}


void UnrootedSBN::setValue(RevBayesCore::Tree *v, bool force)
{

    // delegate to super class
    TypedDistribution<Tree>::setValue( v, force );

    // // Check that this isn't an artifact of arbitrary outgroup choice + clamping
    // if ( rooted == false && outgroup_provided == false )
    // {
    //     std::vector<Taxon> taxa;
    //     v->getRoot().getChild( 0 ).getTaxa( taxa );
    //     outgroup = Clade( taxa );
    //
    // }

}


void UnrootedSBN::simulateTree( void )
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
    std::vector<std::pair<size_t,TopologyNode*> > active;

    size_t index = num_taxa;

    // Root split
    double u = rng->uniform01();
    TopologyNode* root = new TopologyNode( 0 );
    root->setNodeType(false, true, false);
    for (size_t i=0; i<root_splits.size(); ++i)
    {
      if (u < root_split_probabilities[i])
      {
        active.push_back(std::make_pair(root_splits[i],root));
        break;
      }
      u -= root_split_probabilities[i];
    }

    // All other subplits
    while (active.size() > 0)
    {
      // Get a node/subsplit to work on, remove that from list
      std::pair<size_t,TopologyNode*> this_parent = active.back();
      active.pop_back();

      size_t this_parent_subsplit = this_parent.first;
      TopologyNode* this_parent_node = this_parent.second;

      // Choose left subsplit
      u = rng->uniform01();
      size_t chosen_child;
      for (size_t i=0; i<subsplit_children[this_parent_subsplit][0].size(); ++i)
      {
        if (u < subsplit_probabilities[this_parent_subsplit][0][i])
        {
          chosen_child <- subsplit_children[this_parent_subsplit][0][i];
          break;
        }
        u -= root_split_probabilities[i];
      }

      // Before adding node, check if this is a tip, which we call a "fake" subsplit
      if (subsplits[chosen_child].isFake())
      {
        // This is a tip, we don't add it to the active pile
        Clade tip = subsplits[chosen_child].asClade();
        TopologyNode* this_node = new TopologyNode( tip.getTaxon(0), ++index );
        this_node->setNodeType(true, false, false);

      }
      else
      {
        // This is an internal node
        TopologyNode* this_node = new TopologyNode( ++index );
        this_node->setNodeType(false, false, true);
        active.push_back(std::make_pair(chosen_child,this_node));
      }

      // Choose right subsplit
      u = rng->uniform01();
      for (size_t i=0; i<subsplit_children[this_parent_subsplit][1].size(); ++i)
      {
        if (u < subsplit_probabilities[this_parent_subsplit][1][i])
        {
          chosen_child <- subsplit_children[this_parent_subsplit][1][i];
          break;
        }
        u -= root_split_probabilities[i];
      }

      // Before adding node, check if this is a tip, which we call a "fake" subsplit
      if (subsplits[chosen_child].isFake())
      {
        // This is a tip, we don't add it to the active pile
        Clade tip = subsplits[chosen_child].asClade();
        TopologyNode* this_node = new TopologyNode( tip.getTaxon(0), ++index );
        this_node->setNodeType(true, false, false);

      }
      else
      {
        // This is an internal node
        TopologyNode* this_node = new TopologyNode( ++index );
        this_node->setNodeType(false, false, true);
        active.push_back(std::make_pair(chosen_child,this_node));
      }

    }


    // Do we need to do this here??
    // // re-couple tip node names with tip indices
    // // this is necessary because otherwise tip names get scrambled across replicates
    // for (size_t i=0; i<num_taxa; i++)
    // {
    // 	psi->getTipNodeWithName(taxa[i].getName()).setIndex(i);
    // }
    //
    psi->orderNodesByIndex();

    // initialize the topology by setting the root
    psi->setRoot(root, true);

    // finally store the new value
    delete value;
    value = psi;

}


/** Swap a parameter of the distribution */
void UnrootedSBN::swapParameterInternal( const DagNode *oldP, const DagNode *newP )
{

  // if (oldP == Nes)
  // {
  //     Nes = static_cast<const TypedDagNode<RbVector<double> >* >( newP );
  // }
  // else if (oldP == intervalStarts)
  // {
  //     intervalStarts = static_cast<const TypedDagNode<RbVector<double> >* >( newP );
  // }
}

// /**
//  * We check here if all the constraints are satisfied.
//  * These are hard constraints, that is, the clades must be monophyletic.
//  *
//  * \return     True if the constraints are matched, false otherwise.
//  */
// bool UnrootedSBN::matchesConstraints( void )
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
