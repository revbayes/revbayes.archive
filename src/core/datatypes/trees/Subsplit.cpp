#include "Subsplit.h"
#include "RbVectorUtilities.h"

#include <algorithm>
#include <iostream>
#include <sstream>


using namespace RevBayesCore;


/**
 * Default constructor required by the revlanguage code.
 */
Subsplit::Subsplit( void ) :
    bitset(),
    clade_y(),
    clade_z(),
    is_fake()
{

}

/**
 * Constructor from single taxon (as a clade)
 */
Subsplit::Subsplit( const Clade &c1, const std::vector<Taxon> &n ) :
    bitset(),
    clade_y(),
    clade_z(),
    is_fake( true )
{
    // Check that clade contains only single taxon
    if ( c1.size() > 1 )
    {
      throw(RbException("Cannot create fake subsplit from clade of multiple species."));
    }

    // We default to putting the taxon in Clade Y for fake subsplits
    clade_y = c1;

    // bitset representation
    std::vector<Taxon> unset_taxa_1 = c1.getTaxa();
    RbBitSet clade_1_bitset = RbBitSet(n.size(),false);

    for (size_t i=0; i<n.size(); ++i)
    {
      for (std::vector<Taxon>::iterator jt=unset_taxa_1.begin(); jt!=unset_taxa_1.end(); jt++)
      {
        if (n[i] == (*jt))
        {
          clade_1_bitset.set(i);
          unset_taxa_1.erase(jt);
          break;
        }
      }
      if ( unset_taxa_1.size() == 0 )
      {
        break;
      }
    }

    if (unset_taxa_1.size() != 0)
    {
      throw(RbException("Attempt to create subsplit with taxon not in vector of taxa."));
    }

    bitset.first = clade_1_bitset;
    bitset.second = clade_1_bitset;
}


/**
 * Constructor from clade objects.
 */
Subsplit::Subsplit( const Clade &c1, const Clade &c2, const std::vector<Taxon> &n ) :
    bitset(),
    clade_y(),
    clade_z(),
    is_fake( false )
{

    // bitset representations and check that X and Y are disjoint
    bool disjoint = true;

    std::vector<Taxon> unset_taxa_1 = c1.getTaxa();
    std::vector<Taxon> unset_taxa_2 = c2.getTaxa();
    RbBitSet clade_1_bitset = RbBitSet(n.size(),false);
    RbBitSet clade_2_bitset = RbBitSet(n.size(),false);

    for (size_t i=0; i<n.size(); ++i)
    {
      for (std::vector<Taxon>::iterator jt=unset_taxa_1.begin(); jt!=unset_taxa_1.end(); jt++)
      {
        if (n[i] == (*jt))
        {
          clade_1_bitset.set(i);
          unset_taxa_1.erase(jt);
          break;
        }
      }
      for (std::vector<Taxon>::iterator jt=unset_taxa_2.begin(); jt!=unset_taxa_2.end(); jt++)
      {
        if (n[i] == (*jt))
        {
          clade_2_bitset.set(i);
          unset_taxa_2.erase(jt);
          break;
        }
      }
      if (clade_1_bitset.isSet(i) && clade_2_bitset.isSet(i))
      {
        disjoint = false;
      }
      if (unset_taxa_1.size() == 0 && unset_taxa_2.size() == 0)
      {
        break;
      }
    }

    if ( unset_taxa_1.size() != 0 || unset_taxa_2.size() != 0 )
    {
      throw(RbException("Attempt to create subsplit with taxon not in vector of taxa."));
    }

    if (!disjoint)
    {
      throw(RbException("Cannot create subsplit from non-disjoint clades."));
    }

    // Order clades
    if ( clade_1_bitset < clade_2_bitset )
    {
      clade_y = c1;
      clade_z = c2;
      bitset.first = clade_1_bitset;
      bitset.second = clade_2_bitset;
    }
    else
    {
      clade_y = c2;
      clade_z = c1;
      bitset.first = clade_2_bitset;
      bitset.second = clade_1_bitset;
    }
}

/**
 * Constructor from a bitset pair and taxon vector,
 * For use internally, to allow more efficient representations
 * of subsplits without lots of clade objects.
 * Can be used to construct fake or real subsplit.
 */
Subsplit::Subsplit( const std::pair<RbBitSet,RbBitSet> &b, const std::vector<Taxon> &n ) :
    bitset(),
    clade_y(),
    clade_z(),
    is_fake()
{
  // Clade 1 from bitset
  Clade c1 = Clade(b.first, n);

  // Check if we should be making a fake split
  // Fake split bitsets are a pair of identical bitsets with only a single 1 (as they are singleton clades == taxa)
  if ( b.first == b.second) {
    if ( b.first.getNumberSetBits() == 1 && b.second.getNumberSetBits() == 1 ) {
      is_fake = true;
      clade_y = c1;
    }
    else
    {
      throw(RbException("Cannot create fake subsplit from clade of multiple species."));
    }
  }
  else
  {
    Clade c2 = Clade(b.second, n);

    // Check that X and Y are disjoint
    bool disjoint = true;
    for (size_t i = 0; i < n.size(); ++i)
    {
      if ( b.first[i] == 1 && b.second[i] == 1 ) {
        disjoint = false;
        break;
      }
    }

    if (!disjoint)
    {
      throw(RbException("Cannot create subsplit from non-disjoint clades."));
    }

    // Order clades
    if ( b.first < b.second )
    {
      clade_y = c1;
      clade_z = c2;
      bitset.first = b.first;
      bitset.second = b.second;
    }
    else
    {
      clade_y = c2;
      clade_z = c1;
      bitset.first = b.second;
      bitset.second = b.first;

    }
  }
}

/**
 * Overloaded equals operator.
 * Only if both clades in both subsplits are equal are two Subsplits are equal.
 */
bool Subsplit::operator==(const Subsplit &s) const
{
  RbBitSet s1 = asCladeBitset();
  RbBitSet s2 = s.asCladeBitset();

  return s1 == s2;
}


/**
 * Not equals operator that uses the equals operator.
 */
bool Subsplit::operator!=(const Subsplit &s) const
{
  RbBitSet s1 = asCladeBitset();
  RbBitSet s2 = s.asCladeBitset();

  return s1 != s2;
}

/**
 * Less than operator, we outsource this to a bitset comparison.
 */
bool Subsplit::operator<(const Subsplit &s) const
{
  RbBitSet s1 = asCladeBitset();
  RbBitSet s2 = s.asCladeBitset();

  return s1 < s2;
}

/**
 * Less than or equal to operator
 */
bool Subsplit::operator<=(const Subsplit &s) const
{

  return ( *this < s || *this == s );
}

/**
 * Greater than operator.
 */
bool Subsplit::operator>(const Subsplit &s) const
{
  return !( *this <= s );
}

/**
 * Greater than operator or equal to operator
 */
bool Subsplit::operator>=(const Subsplit &s) const
{
  return !( *this < s );
}


/**
 * Make subsplit into a single clade object
 *
 * \return    The clade.
 */
Clade Subsplit::asClade( void ) const
{
    Clade total;
    if (is_fake)
    {
      total.addTaxon(clade_y.getTaxon(0));
    }
    else
    {
      for (size_t i=0; i<clade_y.size(); ++i)
      {
        total.addTaxon(clade_y.getTaxon(i));
      }

      for (size_t i=0; i<clade_z.size(); ++i)
      {
        total.addTaxon(clade_z.getTaxon(i));
      }
    }
    return total;
}

/**
 * Make subsplit into a bitset for a single clade object
 *
 * \return    The clade.
 */
RbBitSet Subsplit::asCladeBitset( void ) const
{
    RbBitSet total = bitset.first | bitset.second;

    return total;
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself
 */
Subsplit* Subsplit::clone(void) const
{
    return new Subsplit(*this);
}

/**
 * Get clade Y.
 *
 * \return    Clade Y.
 */
Clade Subsplit::getY( void ) const
{
    return clade_y;
}

/**
 * Get clade Y.
 *
 * \return    Clade Z.
 */
Clade Subsplit::getZ( void ) const
{
    if (!is_fake)
    {
      return clade_z;
    }
    else
    {
      throw(RbException("Cannot access subsplit clade Z from a fake subsplit."));
    }
}

/**
 * Is subsplit compatible with this one?
 *
 * \return    true/false
 */
bool Subsplit::isCompatible(const Subsplit &s) const
{
    // A subsplit s is compatible with another subsplit t if the clades in s sum to be one of the clades of t
    Clade c = s.asClade();
    if ( c == clade_y )
    {
        return true;
    }
    else if (!is_fake && c == clade_z) // Don't compare to clade Z if there is no clade Z
    {
      return true;
    }
    return false;
}

/**
 * Is subsplit a fake subsplit that contains only a single taxon?
 *
 * \return    true/false
 */
bool Subsplit::isFake() const
{
    return is_fake;
}

/**
 * Get the number of taxa contained in this Subsplit.
 *
 * \return       Size of the taxon name vector in both clades combined.
 */
size_t Subsplit::size(void) const
{
    if (is_fake)
    {
      return(clade_y.size());
    }
    else
    {
      return clade_y.size() + clade_z.size();
    }
}


/**
 * Write the value of this Subsplit as a string.
 *
 * \return    A single string containing the entire Subsplit.
 */
std::string Subsplit::toString( void ) const
{
    std::string s;

    if (is_fake)
    {
      s = clade_y.toString() + "|" + clade_y.toString();
    }
    else
    {
      s = clade_y.toString() + "|" + clade_z.toString();
    }
    return s;
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const Subsplit& x) {

    o << x.toString();

    return o;
}
