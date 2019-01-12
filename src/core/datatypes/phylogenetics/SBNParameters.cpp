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
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "SBNParameters.h"

#include <cmath>
#include <iomanip>


using namespace RevBayesCore;


/** Construct empty SBN parameters */
SBNParameters::SBNParameters( void ) :
  edge_length_distributions(),
  num_taxa(),
  taxa(),
  root_splits(),
  subsplit_cpds()
{
  // Nothing to do
}

/** Construct empty SBN parameters from taxa */
SBNParameters::SBNParameters( std::vector<Taxon> taxa ) :
  edge_length_distributions(),
  num_taxa( taxa.size() ),
  taxa( taxa ),
  root_splits(),
  subsplit_cpds()
{
  // Nothing to do
}

/** Construct rate matrix with n states */
SBNParameters::SBNParameters( const SBNParameters &sbn ) :
  edge_length_distributions(sbn.edge_length_distributions),
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
      edge_length_distributions = sbn.edge_length_distributions;
      num_taxa                  = sbn.num_taxa;
      taxa                      = sbn.taxa;
      root_splits               = sbn.root_splits;
      subsplit_cpds             = sbn.subsplit_cpds;
    }

    return *this;
}

std::map<std::pair<Subsplit,Subsplit>,TypedDistribution<double>* >& SBNParameters::getEdgeLengthDistributions(void)
{
  return edge_length_distributions;
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

double SBNParameters::computeRootSplitProbability( Subsplit root_split ) const
{
  double prob = RbConstants::Double::nan;
  for (size_t i=0; i<root_splits.size(); ++i)
  {
    if ( root_split == root_splits[i].first)
    {
      prob = root_splits[i].second;
    }
  }
  return prob;
}

double SBNParameters::computeSubsplitTransitionProbability( const Subsplit parent, const Subsplit child ) const
{

  // Find all potential children of parent
  std::vector<std::pair<Subsplit,double> > all_children = subsplit_cpds.at(parent);

  double prob = RbConstants::Double::nan;

  for (size_t i=0; i<all_children.size(); ++i)
  {
    if ( child == all_children[i].first)
    {
      prob = all_children[i].second;
    }
  }
  return prob;

}

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

Subsplit SBNParameters::drawSubsplitForY( Subsplit s ) const
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

Subsplit SBNParameters::drawSubsplitForZ( Subsplit s ) const
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

void SBNParameters::learnRootedUnconstrainedSBN( std::vector<Tree> &trees )
{
  // For counting subsplits, we could use integers but unrooted trees get fractional counts, so we'll be consistent
  std::map<Subsplit,double> root_split_counts;
  std::map<std::pair<Subsplit,Subsplit>,double> parent_child_counts;

  // TODO: add branch length processing
  // std::map<std::pair<Subsplit,Subsplit>,double> branch_length_observations;

  // Loop over all trees
  // for each, get all root splits and subsplit parent-child relationships
  // then consolidate into our master list
  for (size_t i=0; i<trees.size(); ++i)
  {
    Subsplit this_root_split;
    this_root_split = trees[i].getRootSubsplit();
    if ( root_split_counts.count(this_root_split) == 0 )
    {
      root_split_counts[this_root_split] = 1.0;
    }
    else
    {
      root_split_counts[this_root_split] += 1.0;
    }

    std::vector<std::pair<Subsplit,Subsplit> > these_parent_child_subsplits = trees[i].getAllSubsplitParentChildPairs();
    for (size_t j=0; j<these_parent_child_subsplits.size(); ++j)
    {
      if ( parent_child_counts.count(these_parent_child_subsplits[j]) == 0 )
      {
        parent_child_counts[these_parent_child_subsplits[j]] = 1.0;
      }
      else
      {
        parent_child_counts[these_parent_child_subsplits[j]] += 1.0;
      }
    }

  }
  // Put root splits in correct format and place
  std::pair<Subsplit,double> this_root;
  BOOST_FOREACH(this_root, root_split_counts) {
    root_splits.push_back(this_root);
  }

  // Normalize root splits
  for (size_t i=0; i<root_splits.size(); ++i)
  {
    root_splits[i].second /= trees.size();
  }

  // Put parent-child splits in correct format and place
  std::pair<std::pair<Subsplit,Subsplit>,double> this_parent_child;
  BOOST_FOREACH(this_parent_child, parent_child_counts) {
    // Subsplit this_parent = this_parent_child.first.first;
    // Subsplit this_child = this_parent_child.first.second;
    // double this_prob = this_parent_child.second;
    std::pair<Subsplit,double> this_cpd;
    // this_cpd.first = this_child;
    // this_cpd.second = this_prob;
    this_cpd.first = this_parent_child.first.second;
    this_cpd.second = this_parent_child.second;

    (subsplit_cpds[this_parent_child.first.first]).push_back(this_cpd);

  }

  // Normalize CPDs
  std::pair<Subsplit,std::vector<std::pair<Subsplit,double> > > this_cpd;

  // Loop over parent subsplits
  BOOST_FOREACH(this_cpd, subsplit_cpds) {
    Subsplit x = this_cpd.first; // The parent subsplit
    std::vector<std::pair<Subsplit,double> > my_children = this_cpd.second; // The children of this parent

    double sum_y = 0.0; // sum of counts for child of subsplit X's clade Y
    double sum_z = 0.0; // sum of counts for child of subsplit X's clade Z

    // Find a distinguishing feature of clade Y in subsplit s
    // Since Y and Z are disjoint, we can use the first set bits in Y and Z
    // Unlike in drawing subsplits, here we make sure that children are compatible with their parents
    //   this way, when drawing, we are safe, since the subsplit is guaranteed to be fair game
    size_t fsb_y = x.getYBitset().getFirstSetBit();
    size_t fsb_z = x.getZBitset().getFirstSetBit();

    for (size_t i=0; i<my_children.size(); ++i) // Loop over the children of this parent, get sum for normalizing
    {
      // This is a subsplit of X's clade Y if one of its splits has the same first set bit as Y
      // my_children[i].first is a Subsplit, with its bitset.first being the bitset representation of its clade Y
      if ( my_children[i].first.getYBitset().getFirstSetBit() == fsb_y || my_children[i].first.getZBitset().getFirstSetBit() == fsb_y )
      {
        sum_y +=  my_children[i].second;
      }
      else if ( my_children[i].first.getYBitset().getFirstSetBit() == fsb_z || my_children[i].first.getZBitset().getFirstSetBit() == fsb_z )
      {
        sum_z +=  my_children[i].second;
      }
      else {
        throw(RbException("Found incompatible subsplit when learning SBN."));
      }
    }

    for (size_t i=0; i<my_children.size(); ++i) // Loop over the children of this parent, normalize
    {
      // This is a subsplit of X's clade Y if one of its splits has the same first set bit as Y
      // my_children[i].first is a Subsplit, with its bitset.first being the bitset representation of its clade Y
      if ( my_children[i].first.getYBitset().getFirstSetBit() == fsb_y || my_children[i].first.getZBitset().getFirstSetBit() == fsb_y )
      {
        (subsplit_cpds[x][i]).second /= sum_y;
      }
      else
      {
        (subsplit_cpds[x][i]).second /= sum_z;
      }
    }
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
