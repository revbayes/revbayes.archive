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

#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "SBNParameters.h"

#include <cmath>
#include <iomanip>


using namespace RevBayesCore;


/** Construct rate matrix with n states */
SBNParameters::SBNParameters( void ) :
  num_taxa(),
  taxa(),
  root_splits(),
  subsplit_cpds()
{
  // Nothing to do
}

// /** Construct rate matrix with n states */
// SBNParameters::SBNParameters( const SBNParameters &tpm ) :
//     num_states( tpm.num_states ),
//     nElements( tpm.nElements )
// {
//
//     theMatrix = new double[ nElements ];
//     for ( size_t i = 0; i < nElements; ++i)
//     {
//         theMatrix[i] = tpm.theMatrix[i];
//     }
//
// }
//
//
// SBNParameters::~SBNParameters()
// {
//
//     delete [] theMatrix;
//
// }
//
//
// /** Construct rate matrix with n states */
// SBNParameters& SBNParameters::operator=( const SBNParameters &sbn ) const
// {
//
//     if ( this != &tpm )
//     {
//         nElements = tpm.nElements;
//         num_states = tpm.num_states;
//
//         delete [] theMatrix;
//         theMatrix = new double[ nElements ];
//         for ( size_t i = 0; i < nElements; ++i)
//         {
//             theMatrix[i] = tpm.theMatrix[i];
//         }
//     }
//
//     return *this;
// }

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
  size_t fsb = s.getBitset().first.getFirstSetBit();

  double u = GLOBAL_RNG->uniform01();
  size_t index;

  // This is like drawing a root split, but we must ensure we only draw from subsplits of Y
  for (size_t i=0; i<my_children.size(); ++i)
  {
    // This is a subsplit of Y if one of its splits has the same first set bit as Y
    // my_children[i].first is a Subsplit, with its bitset.first being the bitset representation of its clade Y
    if ( my_children[i].first.getBitset().first.getFirstSetBit() == fsb || my_children[i].first.getBitset().second.getFirstSetBit() )
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
  size_t fsb = s.getBitset().second.getFirstSetBit();

  double u = GLOBAL_RNG->uniform01();
  size_t index;

  // This is like drawing a root split, but we must ensure we only draw from subsplits of Y
  for (size_t i=0; i<my_children.size(); ++i)
  {
    // This is a subsplit of Y if one of its splits has the same first set bit as Y
    // my_children[i].first is a Subsplit, with its bitset.first being the bitset representation of its clade Y
    if ( my_children[i].first.getBitset().first.getFirstSetBit() == fsb || my_children[i].first.getBitset().second.getFirstSetBit() )
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

//
//
// // std::ostream& RevBayesCore::operator<<(std::ostream& o, const SBNParameters& x) {
// //
// //     std::streamsize previousPrecision = o.precision();
// //     std::ios_base::fmtflags previousFlags = o.flags();
// //
// //     o << "[ ";
// //     o << std::fixed;
// //     o << std::setprecision(4);
// //
// //     // print the RbMatrix with each column of equal width and each column centered on the decimal
// //     for (size_t i=0; i < x.getNumberOfStates(); i++)
// //     {
// //         if (i == 0)
// //             o << "[ ";
// //         else
// //             o << "  ";
// //
// //         for (size_t j = 0; j < x.getNumberOfStates(); ++j)
// //         {
// //             if (j != 0)
// //                 o << ", ";
// //             o << x[i][j];
// //         }
// //         o <<  " ]";
// //
// //         if (i == x.size()-1)
// //             o << " ]";
// //         else
// //             o << " ,\n";
// //
// //     }
// //
// //     o.setf(previousFlags);
// //     o.precision(previousPrecision);
// //
// //     return o;
// // }
