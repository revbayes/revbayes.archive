/**
 * @file
 * This file contains the declaration of SBNParameters, which is
 * class that holds the parameters for an SBN, and methods for use
 * with SBNs.
 *
 * @brief Implementation of SBNParameters
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include <boost/foreach.hpp>
#include "DistributionGamma.h"
#include "DistributionLognormal.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "Split.h"
#include "SBNParameters.h"

#include <cmath>
#include <iomanip>


using namespace RevBayesCore;


/** Construct empty SBN parameters */
SBNParameters::SBNParameters( void ) :
  edge_length_distribution_parameters(),
  num_taxa(),
  taxa(),
  root_splits(),
  subsplit_cpds()
{
  // Nothing to do
}

/** Construct empty SBN parameters from taxa */
SBNParameters::SBNParameters( std::vector<Taxon> taxa ) :
  edge_length_distribution_parameters(),
  num_taxa( taxa.size() ),
  taxa( taxa ),
  root_splits(),
  subsplit_cpds()
{
  // Nothing to do
}

/** Construct rate matrix with n states */
SBNParameters::SBNParameters( const SBNParameters &sbn ) :
  edge_length_distribution_parameters(sbn.edge_length_distribution_parameters),
  num_taxa(sbn.num_taxa),
  taxa(sbn.taxa),
  root_splits(sbn.root_splits),
  subsplit_cpds(sbn.subsplit_cpds)
{

}

SBNParameters::~SBNParameters()
{

}


/** Construct rate matrix with n states */
SBNParameters& SBNParameters::operator=( const SBNParameters &sbn )
{

    if ( this != &sbn )
    {
      edge_length_distribution_parameters = sbn.edge_length_distribution_parameters;
      num_taxa                  = sbn.num_taxa;
      taxa                      = sbn.taxa;
      root_splits               = sbn.root_splits;
      subsplit_cpds             = sbn.subsplit_cpds;
    }

    return *this;
}

// std::map<RbBitSet,std::pair<double,double> >& SBNParameters::getEdgeLengthDistributionParameters(void)
std::map<RbBitSet,std::pair<double,double> >& SBNParameters::getEdgeLengthDistributionParameters(void)
{
  return edge_length_distribution_parameters;
}

const size_t SBNParameters::getNumTaxa(void) const
{
  return num_taxa;
}

std::vector<std::pair<Subsplit,double> >& SBNParameters::getRootSplits(void)
{
  return root_splits;
}

std::map<Subsplit,std::vector<std::pair<Subsplit,double> > >& SBNParameters::getSubsplitCPDs(void)
{
  return subsplit_cpds;
}

std::vector<Taxon>& SBNParameters::getTaxa(void)
{
  return taxa;
}

const std::vector<Taxon>& SBNParameters::getTaxa(void) const
{
  return taxa;
}

/* Computes the probability of seeing a particular root split given an SBN */
double SBNParameters::computeRootSplitProbability( const Subsplit &root_split ) const
{
  double log_prob = RbConstants::Double::neginf;
  for (size_t i=0; i<root_splits.size(); ++i)
  {
    if ( root_split == root_splits[i].first)
    {
      log_prob = log(root_splits[i].second);
    }
  }
  return log_prob;
}

/* Computes the probability of seeing a particular parent-child subsplit pair given an SBN */
double SBNParameters::computeSubsplitTransitionProbability( const Subsplit &parent, const Subsplit &child ) const
{

  // Find all potential children of parent
  const std::vector<std::pair<Subsplit,double> > &all_children = subsplit_cpds.at(parent);

  double log_prob = RbConstants::Double::neginf;

  for (size_t i=0; i<all_children.size(); ++i)
  {
    if ( child == all_children[i].first)
    {
      log_prob = log(all_children[i].second);
    }
  }
  return log_prob;

}

/* Draw a root split from an SBN */
Subsplit SBNParameters::drawRootSplit( void ) const
{

  double u = GLOBAL_RNG->uniform01();
  size_t index;
  for (size_t i=0; i<root_splits.size(); ++i)
  {
    if (u < root_splits[i].second)
    {
      index = i;
      break;
    }
    u -= root_splits[i].second;
  }

  return root_splits[index].first;
}

/* Draws a subsplit for subsplit S's clade Y*/
Subsplit SBNParameters::drawSubsplitForY( const Subsplit &s ) const
{

  // Find all potential children of s
  std::vector<std::pair<Subsplit,double> > my_children = subsplit_cpds.at(s);

  // Find a distinguishing feature of clade Y in subsplit s
  // Since Y and Z are disjoint, we can use the first set bit in Y
  size_t fsb = s.getYBitset().getFirstSetBit();

  double u = GLOBAL_RNG->uniform01();
  size_t index;

  // This is like drawing a root split, but we must ensure we only draw from subsplits of Y
  for (size_t i=0; i<my_children.size(); ++i)
  {
    // This is a subsplit of Y if one of its splits has the same first set bit as Y
    // my_children[i].first is a Subsplit, with its bitset.first being the bitset representation of its clade Y
    if ( my_children[i].first.getYBitset().getFirstSetBit() == fsb || my_children[i].first.getZBitset().getFirstSetBit() == fsb )
    {
      if (u < my_children[i].second)
      {
        index = i;
        break;
      }
      u -= my_children[i].second;
    }
  }

  return my_children[index].first;
}

/* Draws a subsplit for subsplit S's clade Z*/
Subsplit SBNParameters::drawSubsplitForZ( const Subsplit &s ) const
{
  // Find all potential children of s
  std::vector<std::pair<Subsplit,double> > my_children = subsplit_cpds.at(s);

  // Find a distinguishing feature of clade Y in subsplit s
  // Since Y and Z are disjoint, we can use the first set bit in Z
  size_t fsb = s.getZBitset().getFirstSetBit();

  double u = GLOBAL_RNG->uniform01();
  size_t index;

  // This is like drawing a root split, but we must ensure we only draw from subsplits of Z
  for (size_t i=0; i<my_children.size(); ++i)
  {
    // This is a subsplit of Y if one of its splits has the same first set bit as Y
    // my_children[i].first is a Subsplit, with its bitset.first being the bitset representation of its clade Y
    if ( my_children[i].first.getYBitset().getFirstSetBit() == fsb || my_children[i].first.getZBitset().getFirstSetBit() == fsb )
    {
      if (u < my_children[i].second)
      {
        index = i;
        break;
      }
      u -= my_children[i].second;
    }
  }

  return my_children[index].first;
}

/* Checks the validity of an SBNParameters object.
   This requires checking that, for each candidate subsplit,
     1) The CPDs sum to 1 for both splitting Y and Z
     2) All candidate subsplits are compatible with their parents
     3) All subsplits are pairs of disjoint splits
   There are equivalent conditions on the root split
*/
bool SBNParameters::isValid(void) const
{
  if ( !isValidRootDistribution() )
  {
    return false;
  }

  std::pair<Subsplit,std::vector<std::pair<Subsplit,double> > > this_cpd;
  // Loop over parent subsplits
  BOOST_FOREACH(this_cpd, subsplit_cpds) {
    if ( !(isValidCPD(this_cpd.second, this_cpd.first)) )
    {
      return false;
    }
  }

  return true;
}

// Counts all subsplits in an unrooted tree (handles all virtual rooting)
void SBNParameters::countAllSubsplits(Tree& tree, std::map<std::pair<Subsplit,Subsplit>,double>& parent_child_counts, std::map<Subsplit,double>& root_split_counts, std::map<Subsplit,double>& q, bool doSA)
{
  // Prep for tip to root pass
  std::string order = "postorder";
  tree.orderNodesForTraversal(order);
  const std::vector<TopologyNode*> &postorder_nodes = tree.getNodes();

  double one_over_n_branches = 1.0 / (2.0 * tree.getNumberOfNodes() - 3.0); // 1 over the number of branches in an unrooted tree

  // For storing subsplits
  std::vector<Subsplit> per_node_subsplit = std::vector<Subsplit>(tree.getNumberOfNodes(),Subsplit());
  // For storing sum of rooting probabilities from tip through this branch.
  std::vector<double> ttr = std::vector<double>(tree.getNumberOfNodes(),0.0);

  // Tip to root pass, here we do two things
  // 1) Get all node's subsplits (we will need these repeatedly)
  //      We do not make a subsplit for the root as the root is a trifurcation in an unrooted tree
  // 2) accumulate cumulative rooting probabilities and add to root split counters
  //      On each edge (the edge subtending the node we're visiting) we need the probability of rooting to the split this edge induces
  //      We do not loop over the root for now because it is a special case: we already account for rooting on all edges, but ttr[root] is ill-defined
  for (std::vector<TopologyNode*>::const_iterator it = postorder_nodes.begin(); it != (postorder_nodes.end()-1); ++it)
  {
    size_t index = (*it)->getIndex();

    if ( (*it)->isTip() )
    {
      // 1)
      per_node_subsplit[index] = (*it)->getSubsplit(taxa);

      // 2)
      Subsplit root_on_edge = per_node_subsplit[index].rootSplitFromClade();
      double this_root_q = doSA ? one_over_n_branches : q[root_on_edge];
      ttr[index] = this_root_q;
      incrementRootSplitCount(root_split_counts,root_on_edge,this_root_q);
    }
    else
    {
      std::vector<int> children = (*it)->getChildrenIndices();

      // 1)
      RbBitSet clade_1 = per_node_subsplit[children[0]].asCladeBitset();
      RbBitSet clade_2 = per_node_subsplit[children[1]].asCladeBitset();
      per_node_subsplit[index] = Subsplit(clade_1,clade_2);

      // 2)
      Subsplit root_on_edge = per_node_subsplit[index].rootSplitFromClade();
      double this_root_q = doSA ? one_over_n_branches : q[root_on_edge];
      ttr[index] = this_root_q + ttr[children[0]] + ttr[children[1]];
      incrementRootSplitCount(root_split_counts,root_on_edge,this_root_q);
    }
  }

  // Root to tip pass (this is where the fun starts)
  order = "preorder";
  tree.orderNodesForTraversal(order);
  const std::vector<TopologyNode*> &preorder_nodes = tree.getNodes();

  // Loop over edges of tree (exploit equivalency between an edge and the node that edge subtends)
  // The root has no edge so there is nothing to do for the root, so we skip it
  for (std::vector<TopologyNode*>::const_iterator it = preorder_nodes.begin()+1; it != preorder_nodes.end(); ++it)
  {
    // std::cout << ">>>working on a root/internal/tip node " << ((*it)->isRoot()) << "/" << ((*it)->isInternal()) << "/" << ((*it)->isTip()) << std::endl;
    // std::cout << ">The node's subsplit is " << per_node_subsplit[(*it)->getIndex()] << std::endl;
    // std::cout << ">The node's parent's subsplit is " << per_node_subsplit[(*it)->getParent().getIndex()] << std::endl;

    size_t index = (*it)->getIndex();

    // Edges descending from root need to be handled differently
    if ( (*it)->getParent().isRoot() )
    {
      // Get subsplits for other two descendants of root
      std::vector<int> root_children_indices = tree.getRoot().getChildrenIndices();

      std::vector<int> sibling_indices;

      for (size_t i=0; i<3; ++i)
      {
        if (index != root_children_indices[i])
        {
          sibling_indices.push_back(root_children_indices[i]);
        }
      }

      // Get all cases for virtual rooting of this edge (including current rooting)
      std::vector<std::pair<Subsplit,Subsplit> > cases = per_node_subsplit[index].doVirtualRootingRootParent(per_node_subsplit[sibling_indices[0]],per_node_subsplit[sibling_indices[1]],per_node_subsplit[index]);

      // Subsplit root_on_edge = per_node_subsplit[index].rootSplitFromClade();

      // Case 1
      double weight = ttr[root_children_indices[0]];
      incrementParentChildCount(parent_child_counts,cases[0],weight);
// std::cout << "did case 1" << std::endl;

      // Case 2
      weight = ttr[root_children_indices[0]];
      incrementParentChildCount(parent_child_counts,cases[1],weight);
// std::cout << "did case 2" << std::endl;

      // Case 3
      // weight = doSA ? one_over_n_branches : q[root_on_edge];
      weight = doSA ? one_over_n_branches : q[cases[5].first];
      incrementParentChildCount(parent_child_counts,cases[2],weight);
// std::cout << "did case 3" << std::endl;

      if ( !((*it)->isTip()) )
      {
        std::vector<int> children_indices = (*it)->getChildrenIndices();
        bool child_0_is_y = per_node_subsplit[index].isChildOfY(per_node_subsplit[children_indices[0]]);

        // Case 4
        weight = ttr[children_indices[child_0_is_y ? 0 : 1]];
        incrementParentChildCount(parent_child_counts,cases[3],weight);
// std::cout << "did case 4" << std::endl;

        // Case 5
        weight = ttr[children_indices[child_0_is_y ? 1 : 0]];
        incrementParentChildCount(parent_child_counts,cases[4],weight);
// std::cout << "did case 5" << std::endl;
      }

      // Case 6
      // weight = doSA ? one_over_n_branches : q[root_on_edge];
      weight = doSA ? one_over_n_branches : q[cases[5].first];
      incrementParentChildCount(parent_child_counts,cases[5],weight);
// std::cout << "did case 6" << std::endl;
    }
    else
    {
      // Define parent-child pair for current rooting (parent first, child second)
      std::pair<Subsplit,Subsplit> this_parent_child;
      this_parent_child.first = per_node_subsplit[(*it)->getParent().getIndex()];
      this_parent_child.second = per_node_subsplit[index];

      // Get all cases for virtual rooting of this edge (including current rooting)
      std::vector<std::pair<Subsplit,Subsplit> > cases = per_node_subsplit[index].doVirtualRootingNonRootParent(this_parent_child.first,this_parent_child.second);

      // Subsplit root_on_edge = per_node_subsplit[index].rootSplitFromClade();

      // Case 1
      double weight = 1 - ttr[(*it)->getParent().getIndex()];
      incrementParentChildCount(parent_child_counts,cases[0],weight);
// std::cout << "did case 1" << std::endl;

      // Case 2
      std::vector<int> my_parents_children = (*it)->getParent().getChildrenIndices();
      size_t sibling = 0;
      if (index == my_parents_children[0])
      {
        sibling = 1;
      }

      weight = ttr[sibling];
      incrementParentChildCount(parent_child_counts,cases[1],weight);
// std::cout << "did case 2" << std::endl;

      // Case 3
      // weight = doSA ? one_over_n_branches : q[root_on_edge];
      weight = doSA ? one_over_n_branches : q[cases[5].first];
      incrementParentChildCount(parent_child_counts,cases[2],weight);
// std::cout << "did case 3" << std::endl;

      if ( !((*it)->isTip()) )
      {
        std::vector<int> children_indices = (*it)->getChildrenIndices();
        bool child_0_is_y = per_node_subsplit[index].isChildOfY(per_node_subsplit[children_indices[0]]);

        // Case 4
        weight = ttr[children_indices[child_0_is_y ? 0 : 1]];
        incrementParentChildCount(parent_child_counts,cases[3],weight);
// std::cout << "did case 4" << std::endl;

        // Case 5
        weight = ttr[children_indices[child_0_is_y ? 1 : 0]];
        incrementParentChildCount(parent_child_counts,cases[4],weight);
// std::cout << "did case 5" << std::endl;
      }

      // Case 6
      // weight = doSA ? one_over_n_branches : q[root_on_edge];
      weight = doSA ? one_over_n_branches : q[cases[5].first];
      incrementParentChildCount(parent_child_counts,cases[5],weight);
// std::cout << "did case 6" << std::endl;

    }

  }

}

void SBNParameters::fitBranchLengthDistributions(std::vector<Tree> &trees)
{
  std::map<RbBitSet,std::vector<double> > branch_length_observations;

  // Loop over all trees
  for (size_t i=0; i<trees.size(); ++i)
  {
    // Get branch lengths
    //unrooted trees have basal polytomies, so without fear we can take a node's clade, turn it into the split the edge below it represents, and add to that edge's observations
    const std::vector<TopologyNode*> tree_nodes = trees[i].getNodes();
    for (size_t i=0; i<tree_nodes.size(); ++i)
    {
      if ( (!tree_nodes[i]->isRoot()) )
      {
        Subsplit this_subsplit = tree_nodes[i]->getSubsplit(taxa);
        RbBitSet this_split = this_subsplit.asSplitBitset();

        // // Polarize split if needed (fewer than half the bits should be set)
        // if ((this_split.getNumberSetBits() > size_t(std::floor(this_split.size()/2.0))) )
        // {
        //   this_split = ~this_split;
        // }
        // // If even, and half bits are set, make sure first bit is 1
        // else if ( size_t(this_split.size() % 2) == 0 && (this_split.getNumberSetBits() == size_t(std::floor(this_split.size()/2.0))) && this_split[0] == 0)
        // {
        //   this_split = ~this_split;
        // }

        (branch_length_observations[this_split]).push_back(tree_nodes[i]->getBranchLength());
      }
    }
  }

  // // Turn branch length observations into empirical distributions
  // std::pair<RbBitSet,std::vector<double> > clade_edge_observations;
  // BOOST_FOREACH(clade_edge_observations, branch_length_observations) {
  //
  //   if (clade_edge_observations.second.size() > 1)
  //   {
  //
  //     sort(clade_edge_observations.second.begin(),clade_edge_observations.second.end());
  //     double obs_min = clade_edge_observations.second[0];
  //     double obs_max = clade_edge_observations.second[clade_edge_observations.second.size()-1];
  //
  //     // This works because if there are 2 data points it gives us a single interval, otherwise we'd need exception handling
  //     int n_intervals = floor(sqrt(clade_edge_observations.second.size()));
  //
  //     double bin_width = (obs_max - obs_min)/n_intervals;
  //
  //     // Initilize vector of breaks
  //     std::vector<double> breaks;
  //     for (size_t i=0; i<n_intervals; ++i)
  //     {
  //       breaks.push_back(obs_min + i * bin_width);
  //     }
  //     breaks.push_back(obs_max);
  //
  //     // Initialize probs to 0 in case there are empty intervals
  //     std::vector<double> probs = std::vector<double>(n_intervals,0.0);
  //
  //     // The contribution of a single data point is 1/n to the probability mass of a cell
  //     double one_over_n = 1.0 / clade_edge_observations.second.size();
  //
  //     // Loop over all data points, count number in each bin and add probability
  //     size_t current_left = 0;
  //     size_t current_right = 1;
  //     for (size_t i=0; i<(clade_edge_observations.second.size()-1); ++i) // Leave out the last data point, handle later
  //     {
  //       // Point is in bounds, add to the interval
  //       if (clade_edge_observations.second[i] < breaks[current_right])
  //       {
  //         probs[current_left] += one_over_n;
  //       }
  //       // Point is out of bounds, find the interval it's in and add it
  //       else {
  //         while (clade_edge_observations.second[i] >= breaks[current_right])
  //         {
  //           current_right += 1;
  //         }
  //         current_left = current_right - 1;
  //         probs[current_left] += one_over_n;
  //       }
  //     }
  //
  //     // We haven't yet counted the last data point
  //     probs[n_intervals-1] += one_over_n;
  //
  //     double s = 0.0;
  //     for (size_t i=0; i<probs.size(); ++i)
  //     {
  //       s += probs[i];
  //     }
  //     if (s > 1.0001 || s < 0.9999) {
  //       std::cout << "sum of probs is " << s << std::endl;
  //     }
  //
  //     std::pair<std::vector<double>,std::vector<double> > these_params;
  //     these_params.first = probs;
  //     these_params.second = breaks;
  //
  //     edge_length_distribution_parameters[clade_edge_observations.first] = these_params;
  //   }
  //   else
  //   {
  //     std::vector<double> probs = std::vector<double>(1,1.0);
  //     std::vector<double> breaks;
  //     breaks.push_back(clade_edge_observations.second[0]*0.95);
  //     breaks.push_back(clade_edge_observations.second[0]*1.05);
  //
  //     std::pair<std::vector<double>,std::vector<double> > these_params;
  //     these_params.first = probs;
  //     these_params.second = breaks;
  //     edge_length_distribution_parameters[clade_edge_observations.first] = these_params;
  //   }
  // }


  // Turn branch length observations into lognormal distributions
  std::pair<RbBitSet,std::vector<double> > clade_edge_observations;
  BOOST_FOREACH(clade_edge_observations, branch_length_observations) {
    // std::cout << "Learning branch distribution for clade " << clade_edge_observations.first << ", observations are:" << std::endl;
    if (clade_edge_observations.second.size() > 2)
    {
      // TODO: if we are going to keep using lognormal via MLE, there are more efficient ways to get the logmean and logsd
      // Get mean/sd of log of observations
      double log_mean;
      for (size_t i=0; i<clade_edge_observations.second.size(); ++i)
      {
        // Branch lengths x such that c++ returns log(x) = -inf are possible, replace with smallest representable number instead
        double log_x = log(clade_edge_observations.second[i]);
        log_mean += log_x == RbConstants::Double::neginf ? RbConstants::Double::min : log_x;
// std::cout << log_x << ",";
      }
// std::cout << std::endl;
      log_mean /= clade_edge_observations.second.size();

      double log_sd;
      for (size_t i=0; i<clade_edge_observations.second.size(); ++i)
      {
        double log_x = log(clade_edge_observations.second[i]);
        log_sd += log_x == RbConstants::Double::neginf ? pow(RbConstants::Double::min - log_mean,2.0) : pow(log_x - log_mean,2.0);
      }
      log_sd /= clade_edge_observations.second.size();
      log_sd = sqrt(log_sd);

      // Approximate edge-length distribution using lognormal, use MLE parameters
      std::pair<double,double> these_params;
      these_params.first = log_mean;
      these_params.second = log_sd;

      edge_length_distribution_parameters[clade_edge_observations.first] = these_params;

    }
    else
    {
      // Basically no information on edge length distribution
      // Approximate edge-length distribution using a lognormal resembling an exponential(10)
      std::pair<double,double> these_params;
      these_params.first = -2.8;
      these_params.second = 1.0;

      edge_length_distribution_parameters[clade_edge_observations.first] = these_params;
    }
  }

  // // Turn branch length observations into gamma distributions
  // std::pair<RbBitSet,std::vector<double> > clade_edge_observations;
  // BOOST_FOREACH(clade_edge_observations, branch_length_observations) {
  //   if (clade_edge_observations.second.size() > 2)
  //   {
  //     // Get mean/var of log of observations
  //     double mean;
  //     for (size_t i=0; i<clade_edge_observations.second.size(); ++i)
  //     {
  //       mean += clade_edge_observations.second[i];
  //     }
  //     mean /= clade_edge_observations.second.size();
  //
  //     double var;
  //     for (size_t i=0; i<clade_edge_observations.second.size(); ++i)
  //     {
  //       var += pow(clade_edge_observations.second[i] - mean,2.0);
  //     }
  //     var /= clade_edge_observations.second.size();
  //
  //     // Approximate edge-length distribution using lognormal, use MLE parameters
  //     std::pair<double,double> these_params;
  //     these_params.second = mean/var;
  //     these_params.first = mean * these_params.second;
  //
  //     edge_length_distribution_parameters[clade_edge_observations.first] = these_params;
  //
  //   }
  //   else
  //   {
  //     // Basically no information on edge length distribution
  //     // Approximate edge-length distribution using an exponential(10)
  //     std::pair<double,double> these_params;
  //     these_params.first = 1.0;
  //     these_params.second = 10.0;
  //
  //     edge_length_distribution_parameters[clade_edge_observations.first] = these_params;
  //   }
  // }

}

void SBNParameters::makeCPDs(std::map<std::pair<Subsplit,Subsplit>,double>& parent_child_counts)
{

  // Put parent-child splits in correct format and place
  std::pair<std::pair<Subsplit,Subsplit>,double> this_parent_child;

  BOOST_FOREACH(this_parent_child, parent_child_counts) {
    Subsplit this_parent = this_parent_child.first.first;
    Subsplit this_child = this_parent_child.first.second;
    double this_prob = this_parent_child.second;

    if ( !(this_parent.isCompatible(this_child)) )
    {
      std::cout << "Invalid s|t" << std::endl;
      std::cout << "  s = " << this_child << std::endl;
      std::cout << "  t = " << this_parent << std::endl;
      throw(RbException("Found impossible parent-child subsplit pair in makeCPDs."));
    }

    std::pair<Subsplit,double> this_cpd;
    this_cpd.first = this_child;
    this_cpd.second = this_prob;

    (subsplit_cpds[this_parent]).push_back(this_cpd);
  }

  // Normalize CPDs
  std::pair<Subsplit,std::vector<std::pair<Subsplit,double> > > parent_cpd_pair;

  // Loop over parent subsplits
  BOOST_FOREACH(parent_cpd_pair, subsplit_cpds) {
    Subsplit parent = parent_cpd_pair.first; // The parent subsplit
    std::vector<std::pair<Subsplit,double> > my_children = parent_cpd_pair.second; // The children of this parent

    for (size_t i=0; i<my_children.size(); ++i)
    {
      if ( !(parent.isCompatible(my_children[i].first)) )
      {
        std::cout << "Found incompatible parent-child subsplit pair:" << parent << "->" << my_children[i].first << std::endl;
        throw(RbException("Found incompatible subsplit in makeCPDs."));
      }
    }

    normalizeCPDForSubsplit(my_children, parent);

  }

}

/*
  This function first puts the root split counts into a root splits probability map,
    then it normalizes them into a probability distribution
*/
void SBNParameters::makeRootSplits(std::map<Subsplit,double>& root_split_counts)
{
  // Put root splits in correct format and place
  std::pair<Subsplit,double> this_root;
  BOOST_FOREACH(this_root, root_split_counts) {
    root_splits.push_back(this_root);
  }

  // Normalize root splits
  double sum_root = 0.0;
  for (size_t i=0; i<root_splits.size(); ++i)
  {
    sum_root += root_splits[i].second;
  }

  for (size_t i=0; i<root_splits.size(); ++i)
  {
    root_splits[i].second /= sum_root;
  }

}

void SBNParameters::normalizeCPDForSubsplit(std::vector<std::pair<Subsplit,double> >& cpd, Subsplit& parent)
{

  double sum_y = 0.0; // sum of counts for child of parent subsplit's clade Y
  double sum_z = 0.0; // sum of counts for child of parent subsplit's clade Z

  // Find a distinguishing feature of clade Y in subsplit s
  // Since Y and Z are disjoint, we can use the first set bits in Y and Z
  size_t fsb_y = parent.getYBitset().getFirstSetBit();
  size_t fsb_z = parent.getZBitset().getFirstSetBit();

  // In unrooted counting, we lose dummy subsplits, we add them in here
  bool y_is_tip = parent.getYBitset().getNumberSetBits() == 1 ? true : false;
  bool z_is_tip = parent.getZBitset().getNumberSetBits() == 1 ? true : false;

  size_t n_children_of_y = 0;
  size_t n_children_of_z = 0;

  for (size_t i=0; i<cpd.size(); ++i) // Loop over the children of this parent, get sum for normalizing
  {
    if ( !(parent.isCompatible(cpd[i].first)) )
    {
      std::cout << "Found incompatible parent-child subsplit pair." << std::endl;
      std::cout << "  parent: " << parent << std::endl;
      std::cout << "  child:  " << cpd[i].first << std::endl;
      throw(RbException("Found incompatible subsplit in normalizeCPDForSubsplit."));
    }

    // This is a subsplit of parent's clade Y if one of its splits has the same first set bit as Y
    // cpd[i].first is a Subsplit, with its bitset.first being the bitset representation of its clade Y
    if ( cpd[i].first.getYBitset().getFirstSetBit() == fsb_y || cpd[i].first.getZBitset().getFirstSetBit() == fsb_y )
    {
      sum_y +=  cpd[i].second;
      ++n_children_of_y;
    }
    else if ( cpd[i].first.getYBitset().getFirstSetBit() == fsb_z || cpd[i].first.getZBitset().getFirstSetBit() == fsb_z )
    {
      sum_z +=  cpd[i].second;
      ++n_children_of_z;
    }
    else {
      throw(RbException("Found incompatible subsplit when learning SBN."));
    }
  }

  // If y or z are tips, we will now insert probability-1 dummy splits for these tips
  if ( n_children_of_y == 0 && y_is_tip )
  {
    RbBitSet tip = parent.getYBitset();
    Subsplit dummy_subsplit = Subsplit(tip,tip);

    std::pair<Subsplit,double> dummy_cpd;
    dummy_cpd.first = dummy_subsplit;
    dummy_cpd.second = 1.0;

    (subsplit_cpds[parent]).push_back(dummy_cpd);
    sum_y = 1.0;
  }

  if ( n_children_of_z == 0 && z_is_tip )
  {
    RbBitSet tip = parent.getZBitset();
    Subsplit dummy_subsplit = Subsplit(tip,tip);

    std::pair<Subsplit,double> dummy_cpd;
    dummy_cpd.first = dummy_subsplit;
    dummy_cpd.second = 1.0;

    (subsplit_cpds[parent]).push_back(dummy_cpd);
    sum_z = 1.0;
  }

  for (size_t i=0; i<cpd.size(); ++i) // Loop over the children of this parent, normalize
  {
    // This is a subsplit of X's clade Y if one of its splits has the same first set bit as Y
    // cpd[i].first is a Subsplit, with its bitset.first being the bitset representation of its clade Y
    if ( cpd[i].first.getYBitset().getFirstSetBit() == fsb_y || cpd[i].first.getZBitset().getFirstSetBit() == fsb_y )
    {
      (subsplit_cpds[parent][i]).second /= sum_y;
    }
    else
    {
      (subsplit_cpds[parent][i]).second /= sum_z;
    }
  }

}

bool SBNParameters::isValidCPD(std::vector<std::pair<Subsplit,double> >& cpd, Subsplit& parent) const
{
  double sum_y = 0.0;
  double sum_z = 0.0;

  size_t fsb_y = parent.getYBitset().getFirstSetBit();
  size_t fsb_z = parent.getZBitset().getFirstSetBit();

  bool y_is_tip = parent.getYBitset().getNumberSetBits() == 1 ? true : false;
  bool z_is_tip = parent.getZBitset().getNumberSetBits() == 1 ? true : false;

  for (size_t i=0; i<cpd.size(); ++i)
  {
    Subsplit child = cpd[i].first;
    size_t child_fsb = child.asCladeBitset().getFirstSetBit();
    RbBitSet child_y = child.getYBitset();
    RbBitSet child_z = child.getZBitset();

    // Make sure child is compatible with parent
    if ( !(parent.isCompatible(child)) )
    {
      std::cout << "Found incompatible parent-child subsplit pair." << std::endl;
      std::cout << "  parent: " << parent << std::endl;
      std::cout << "  child:  " << child << std::endl;
      return false;
    }

    // Make sure children's splits are disjoint
    if ( !child.splitsAreDisjoint() )
    {
      std::cout << "Found impossible subsplit: " << child << std::endl;
      return false;
    }

    // Determine if the child is of parent's split Y or Z, add probability to appropriate sum
    if ( child_fsb == fsb_y )
    {
      sum_y += cpd[i].second;
    }
    else
    {
      sum_z += cpd[i].second;
    }
  }

  // Make sure that for all non-trivial subsplits, the sum of probabilities of children is 1 (trivial subsplits are tips and don't need CPDs)
  double tol = 0.0001;
  // if ( (fabs(sum_y - 1.0) > tol && !y_is_tip) || (fabs(sum_z - 1.0) > tol && !z_is_tip) )
  if ( (fabs(sum_y - 1.0) > tol) || (fabs(sum_z - 1.0) > tol) )
  {
    std::cout << "Unnormalized or improperly normalized CPD for parent subsplit " << parent << std::endl;
    std::cout << "Sum of CPDs for descendants of Y is " << std::fixed << std::setprecision(10) << sum_y << ". Sum of CPDs for descendants of Z is " << sum_z << std::endl;
    return false;
  }

  return true;
}

bool SBNParameters::isValidRootDistribution(void) const
{
  double sum_root = 0.0;
  for (size_t i=0; i<root_splits.size(); ++i)
  {
    sum_root += root_splits[i].second;
    RbBitSet root_y = root_splits[i].first.getYBitset();
    RbBitSet root_z = root_splits[i].first.getZBitset();

    // Check that in sum_root they have all tips
    if ( !(root_y.getNumberSetBits() + root_z.getNumberSetBits() == root_y.size()) )
    {
      return false;
    }
    // Check they're disjoint
    if ( !root_splits[i].first.splitsAreDisjoint() )
    {
      std::cout << "Found impossible root split: " << root_splits[i].first << std::endl;
        return false;
    }
    // Check they're not empty
    if ( root_y.getNumberSetBits() == 0 || root_z.getNumberSetBits() == 0 )
    {
      std::cout << "Found impossible root split: " << root_splits[i].first << ". Pr(root_split) = " << root_splits[i].second << std::endl;
        return false;
    }
  }

  double tol = 0.0001;
  if ( fabs(sum_root - 1.0) > tol )
  {
    std::cout << "Root splits are unnormalized or improperly normalized, sum of root probabilities is " << sum_root << std::endl;
    return false;
  }

  return true;
}

void SBNParameters::learnRootedUnconstrainedSBN( std::vector<Tree> &trees )
{
  // TODO: Reformat branch length processing and tree processing.
  //       We're going to want to read each tree once and leave it as a root split and a vector of subsplits (probably as a pair<root,all_subsplits>).
  //         We're also going to want makeCPDs and makeRootSplits to take these in, as well as a weight (the weight is either the variational distribution q_k^n or 1/(2n-3)) for normalizing.
  //         Each tree-read pass should also return not just the subsplits but the branch lengths observed for each subsplit.
  //       If we're going to consider online work, this will need a lot of other work

  // For counting subsplits, we could use integers but unrooted trees get fractional counts, so we'll be consistent
  std::map<Subsplit,double> root_split_counts;
  std::map<std::pair<Subsplit,Subsplit>,double> parent_child_counts;

  // The weight to assign when counting subsplits, for rooted trees the weight is 1
  double weight = 1.0;

  // Loop over all trees
  // for each, get all root splits and subsplit parent-child relationships
  // then consolidate into our master list
  for (size_t i=0; i<trees.size(); ++i)
  {
    addTreeToAllRootSplitCounts(root_split_counts, trees[i], weight);
    addTreeToAllParentChildCounts(parent_child_counts, trees[i], weight);
  }

  // Turn root split counts into a distribution on the root split
  makeRootSplits(root_split_counts);

  // Turn parent-child subsplit counts into CPDs
  makeCPDs(parent_child_counts);

  if ( !isValid() )
  {
    throw(RbException("learnRootedUnconstrainedSBN produced an invalid SBNParameters object."));
  }

}

void SBNParameters::learnUnconstrainedSBNSA( std::vector<Tree> &trees )
{
  // std::cout << "hello from learnUnconstrainedSBNSA, there are this many trees" << trees.size() << std::endl;

  // To store counts
  std::map<Subsplit,double> root_split_counts;
  std::map<std::pair<Subsplit,Subsplit>,double> parent_child_counts;

  // This can stay empty, we don't need to specify q() if we override with doSA=TRUE
  std::map<Subsplit,double> q;

  // Run counting
  for (size_t i=0; i<trees.size(); ++i)
  {
// std::cout << trees[i] << std::endl;
    countAllSubsplits(trees[i], parent_child_counts, root_split_counts, q, true);
  }
// std::cout << "counted all subsplits in all trees" << std::endl;

  // Turn root split counts into a distribution on the root split
  makeRootSplits(root_split_counts);

  // Turn parent-child subsplit counts into CPDs
  makeCPDs(parent_child_counts);

  // Handle branch lengths
  fitBranchLengthDistributions(trees);

  if ( !isValid() )
  {
    throw(RbException("learnUnconstrainedSBNSA produced an invalid SBNParameters object."));
  }

}

void SBNParameters::learnUnconstrainedSBNEM( std::vector<Tree> &trees, double &alpha )
{

  // Initialize parameters to SA values
  // This will handle the branch lengths for us, we don't need to touch them again
  learnUnconstrainedSBNSA(trees);

  // // run EM, start on E-step because we have parameters from M-step
  // bool terminate = false;
  // while ( !terminate )
  // {
  //   // E-step, compute q and m
  //
  //   // M-step, compute p
  //
  // }
}

// Takes a tree in with weight (can be for multiple trees or for q(root)), keeps rooting intact, adds all parent-child subsplits found in this tree to master list of counts
void SBNParameters::addTreeToAllParentChildCounts(std::map<std::pair<Subsplit,Subsplit>,double> &parent_child_counts, Tree& tree, double &weight)
{
  std::vector<std::pair<Subsplit,Subsplit> > these_parent_child_subsplits = tree.getAllSubsplitParentChildPairs(taxa);
  for (size_t j=0; j<these_parent_child_subsplits.size(); ++j)
  {
    if ( parent_child_counts.count(these_parent_child_subsplits[j]) == 0 )
    {
      parent_child_counts[these_parent_child_subsplits[j]] = weight;
    }
    else
    {
      parent_child_counts[these_parent_child_subsplits[j]] += weight;
    }
  }
}

// Takes a tree in with weight (can be for multiple trees or for q(root)), keeps rooting intact, adds the root split to master list of counts
void SBNParameters::addTreeToAllRootSplitCounts(std::map<Subsplit,double>& root_split_counts, Tree& tree, double &weight)
{
  Subsplit this_root_split;
  this_root_split = tree.getRootSubsplit(taxa);
  if ( root_split_counts.count(this_root_split) == 0 )
  {
    root_split_counts[this_root_split] = weight;
  }
  else
  {
    root_split_counts[this_root_split] += weight;
  }
}


void SBNParameters::incrementParentChildCount(std::map<std::pair<Subsplit,Subsplit>,double> &parent_child_counts, std::pair<Subsplit,Subsplit> &this_parent_child, double &weight)
{
// std::cout << "incrementing ParentChildCount by " << weight << " for parent-child" << this_parent_child.first << " - " << this_parent_child.second << std::endl;
  if ( parent_child_counts.count(this_parent_child) == 0 )
  {
    parent_child_counts[this_parent_child] = weight;
  }
  else
  {
    parent_child_counts[this_parent_child] += weight;
  }
}

void SBNParameters::incrementRootSplitCount(std::map<Subsplit,double>& root_split_counts, Subsplit &this_root_split, double &weight)
{
// std::cout << "incrementing RootSplitCount by " << weight << " for root split" << this_root_split << std::endl;
  if ( root_split_counts.count(this_root_split) == 0 )
  {
    root_split_counts[this_root_split] = weight;
  }
  else
  {
    root_split_counts[this_root_split] += weight;
  }
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const SBNParameters& x) {

    // std::streamsize previousPrecision = o.precision();
    // std::ios_base::fmtflags previousFlags = o.flags();

    std::vector<Taxon> taxa = x.getTaxa();
    o << "SBN on taxon vector [ ";
    // o << std::fixed;
    // o << std::setprecision(4);

    o << taxa[0].getName();

    // print the RbMatrix with each column of equal width and each column centered on the decimal
    for (size_t i=1; i < x.getNumTaxa(); i++)
    {
      o << ", ";
      o << taxa[i].getName();
    }

    o << " ]";
    // o.setf(previousFlags);
    // o.precision(previousPrecision);

    return o;
}
