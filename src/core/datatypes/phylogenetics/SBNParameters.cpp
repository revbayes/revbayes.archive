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

std::map<std::pair<Subsplit,Subsplit>,std::pair<double,double> >& SBNParameters::getEdgeLengthDistributionParameters(void)
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
  double prob = RbConstants::Double::neginf;
  for (size_t i=0; i<root_splits.size(); ++i)
  {
    if ( root_split == root_splits[i].first)
    {
      prob = root_splits[i].second;
    }
  }
  return log(prob);
}

/* Computes the probability of seeing a particular parent-child subsplit pair given an SBN */
double SBNParameters::computeSubsplitTransitionProbability( const Subsplit &parent, const Subsplit &child ) const
{

  // Find all potential children of parent
  const std::vector<std::pair<Subsplit,double> > &all_children = subsplit_cpds.at(parent);

  double prob = RbConstants::Double::neginf;

  for (size_t i=0; i<all_children.size(); ++i)
  {
    if ( child == all_children[i].first)
    {
      prob = all_children[i].second;
    }
  }
  return log(prob);

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

void SBNParameters::incrementParentChildCounts(std::map<std::pair<Subsplit,Subsplit>,double> &parent_child_counts, Tree& tree, double &weight)
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

void SBNParameters::incrementRootSplitCounts(std::map<Subsplit,double>& root_split_counts, Tree& tree, double &weight)
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
    }
    else if ( cpd[i].first.getYBitset().getFirstSetBit() == fsb_z || cpd[i].first.getZBitset().getFirstSetBit() == fsb_z )
    {
      sum_z +=  cpd[i].second;
    }
    else {
      throw(RbException("Found incompatible subsplit when learning SBN."));
    }
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

  double tol = 0.0001;
  if ( fabs(sum_y - 1.0) > tol || fabs(sum_z - 1.0) > tol )
  {
    std::cout << "Unnormalized or improperly normalized CPD for parent subsplit " << parent << std::endl;
    std::cout << "Sum of CPDs for descendants of Y is " << std::fixed << std::setprecision(90) << sum_y << ". Sum of CPDs for descendants of Z is " << sum_z << std::endl;
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
  }

  double tol = 0.0001;
  if ( fabs(sum_root - 1.0) > tol )
  {
    std::cout << "Root splits are unnormalized or improperly normalized" << std::endl;
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
  // For unrooted trees, the weight will be less than 1, and may vary (in the EM algorithm)
  double weight = 1.0;

  std::map<std::pair<Subsplit,Subsplit>,std::vector<double> > branch_length_observations;

  // Loop over all trees
  // for each, get all root splits and subsplit parent-child relationships
  // then consolidate into our master list
  for (size_t i=0; i<trees.size(); ++i)
  {
    incrementRootSplitCounts(root_split_counts, trees[i], weight);
    incrementParentChildCounts(parent_child_counts, trees[i], weight);

    // Get branch lengths
    const std::vector<TopologyNode*> tree_nodes = trees[i].getNodes();
    for (size_t i=0; i<tree_nodes.size(); ++i)
    {
      if (!tree_nodes[i]->isRoot())
      {
        Subsplit this_split = tree_nodes[i]->getSubsplit(taxa);
        Subsplit this_parent = tree_nodes[i]->getParent().getSubsplit(taxa);

        std::pair<Subsplit,Subsplit> this_parent_child;
        this_parent_child.first = this_parent;
        this_parent_child.second = this_split;

        (branch_length_observations[this_parent_child]).push_back(tree_nodes[i]->getBranchLength());

      }
    }


  }

  // Turn root split counts into a distribution on the root split
  makeRootSplits(root_split_counts);

  // Turn parent-child subsplit counts into CPDs
  makeCPDs(parent_child_counts);

  // Turn branch length observations into lognormal distributions
  std::pair<std::pair<Subsplit,Subsplit>,std::vector<double>> parent_child_edge_set;
  BOOST_FOREACH(parent_child_edge_set, branch_length_observations) {
    if (parent_child_edge_set.second.size() > 2)
    {
      // Get mean/sd of log of observations
      double log_mean;
      for (size_t i=0; i<parent_child_edge_set.second.size(); ++i)
      {
        log_mean += log(parent_child_edge_set.second[i]);
      }
      log_mean /= parent_child_edge_set.second.size();

      double log_sd;
      for (size_t i=0; i<parent_child_edge_set.second.size(); ++i)
      {
        log_sd += pow(log(parent_child_edge_set.second[i])-log_mean,2.0);
      }
      log_sd /= parent_child_edge_set.second.size();
      log_sd = sqrt(log_sd);

      // Approximate edge-length distribution using lognormal, use MLE parameters
      std::pair<double,double> these_params;
      these_params.first = log_mean;
      these_params.second = log_sd;

      edge_length_distribution_parameters[parent_child_edge_set.first] = these_params;

    }
    else
    {
      // Basically no information on edge length distribution
      // Approximate edge-length distribution using a lognormal that looks like the standard MrBayes prior
      std::pair<double,double> these_params;
      these_params.first = -2.8;
      these_params.second = 1.0;

      edge_length_distribution_parameters[parent_child_edge_set.first] = these_params;
    }
  }

  if ( !isValid() )
  {
    throw(RbException("learnRootedUnconstrainedSBN produced an invalid SBNParameters object."));
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
