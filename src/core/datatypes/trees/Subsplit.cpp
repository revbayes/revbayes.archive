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
    is_fake()
{

}

/**
 * Constructor from single taxon (as a clade)
 */
Subsplit::Subsplit( const std::vector<Taxon> &c1, const std::vector<Taxon> &n ) :
    bitset(),
    is_fake( true )
    {
        // TODO: having a single taxon instead of a vector of size 1 may be faster

        // Check that clade contains only single taxon
        if ( c1.size() > 1 )
        {
          throw(RbException("Cannot create fake subsplit from clade of multiple species."));
        }

        bool found = false;

        // bitset representation
        RbBitSet clade_1_bitset = RbBitSet(n.size(),false);
        for (size_t i=0; i<n.size(); ++i)
        {
          if (n[i] == c1[0])
          {
            clade_1_bitset.set(i);
            found = true;
            break;
          }
        }

        if (!found)
        {
          throw(RbException("Attempt to create subsplit with taxon not in vector of taxa."));
        }

        bitset.first = clade_1_bitset;
        bitset.second = clade_1_bitset;
    }


/**
 * Constructor from clade objects.
 */
Subsplit::Subsplit( const std::vector<Taxon> &c1, const std::vector<Taxon> &c2, const std::vector<Taxon> &n ) :
    bitset(),
    is_fake( false )
{

    // bitset representations and check that X and Y are disjoint
    bool disjoint = true;

    std::vector<Taxon> c1_unset = c1;
    std::vector<Taxon> c2_unset = c2;
    RbBitSet clade_1_bitset = RbBitSet(n.size(),false);
    RbBitSet clade_2_bitset = RbBitSet(n.size(),false);

    for (size_t i=0; i<n.size(); ++i)
    {
      for (std::vector<Taxon>::iterator jt=c1_unset.begin(); jt!=c1_unset.end(); jt++)
      {
        if (n[i] == (*jt))
        {
          clade_1_bitset.set(i);
          c1_unset.erase(jt);
          break;
        }
      }
      for (std::vector<Taxon>::iterator jt=c2_unset.begin(); jt!=c2_unset.end(); jt++)
      {
        if (n[i] == (*jt))
        {
          clade_2_bitset.set(i);
          c2_unset.erase(jt);
          break;
        }
      }
      if (clade_1_bitset.isSet(i) && clade_2_bitset.isSet(i))
      {
        disjoint = false;
      }
      if (c1_unset.size() == 0 && c2_unset.size() == 0)
      {
        break;
      }
    }

    if ( c1_unset.size() != 0 || c2_unset.size() != 0 )
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
      bitset.first = clade_1_bitset;
      bitset.second = clade_2_bitset;
    }
    else
    {
      bitset.first = clade_2_bitset;
      bitset.second = clade_1_bitset;
    }

    // Check if we made a fake subsplit
    if ( clade_1_bitset.getNumberSetBits() == 1 && clade_2_bitset.getNumberSetBits() == 1 && clade_1_bitset.getFirstSetBit() == clade_2_bitset.getFirstSetBit() )
    {
      is_fake = true;
    }
}

Subsplit& Subsplit::operator=(const Subsplit &s)
{
  if (this != &s)
  {
    bitset  = s.bitset;
    is_fake = s.is_fake;
  }

  return *this;

}
/**
 * Equals operator, we outsource comparison to std::pair<RbBitSet,RbBitSet>.
 * Only if both clades in both subsplits are equal are two Subsplits are equal.
 */
bool Subsplit::operator==(const Subsplit &s) const
{
  return ( s.getBitset() == bitset );
}


/**
 * Not equals operator that uses the equals operator.
 */
bool Subsplit::operator!=(const Subsplit &s) const
{
  return ( s.getBitset() != bitset );
}

/**
 * Less than operator.
 */
bool Subsplit::operator<(const Subsplit &s) const
{
  return ( s.getBitset() < bitset );
}

/**
 * Less than or equal to operator
 */
bool Subsplit::operator<=(const Subsplit &s) const
{
  return ( s.getBitset() <= bitset );
}

/**
 * Greater than operator.
 */
bool Subsplit::operator>(const Subsplit &s) const
{
  return ( s.getBitset() > bitset );
}

/**
 * Greater than operator or equal to operator
 */
bool Subsplit::operator>=(const Subsplit &s) const
{
  return ( s.getBitset() >= bitset );
}


/**
 * Make subsplit into a single clade object
 *
 * \return    The clade.
 */
Clade Subsplit::asClade( const std::vector<Taxon>& taxa ) const
{

    RbBitSet total_bitset = asCladeBitset();
    Clade total = Clade(total_bitset, taxa);
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
 * Utility function to access paired bitset
 *
 * \return    The bitset pair.
 */
std::pair<RbBitSet,RbBitSet> Subsplit::getBitset( void ) const
{

    return bitset;
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
Clade Subsplit::getY( const std::vector<Taxon>& taxa ) const
{
    Clade y = Clade(bitset.first, taxa);
    return y;
}

RbBitSet Subsplit::getYBitset( void ) const
{

    return bitset.first;
}

/**
 * Get clade Z.
 *
 * \return    Clade Z.
 */
Clade Subsplit::getZ( const std::vector<Taxon>& taxa ) const
{
    if (!is_fake)
    {
      Clade z = Clade(bitset.second, taxa);
      return z;
    }
    else
    {
      throw(RbException("Cannot access subsplit clade Z from a fake subsplit."));
    }
}

RbBitSet Subsplit::getZBitset( void ) const
{

    return bitset.second;
}

/**
 * Is subsplit s a child of this subsplit's clade Y?
 * \return    true/false
 */
bool Subsplit::isChildOfY(const Subsplit &s) const
{
    return false;
}

/**
 * Is subsplit s a child of this subsplit's clade Y?
 * \return    true/false
 */
bool Subsplit::isChildOfZ(const Subsplit &s) const
{
    return false;
}

/**
 * Is subsplit compatible with this one?
 * A subsplit s is compatible with another subsplit t if the clades in s sum to be one of the clades of t
 * This requires that s has all the taxa in t's Y or Z splits, no more and no fewer
 * \return    true/false
 */
bool Subsplit::isCompatible(const Subsplit &s) const
{
    if ( s.bitset.first.size() != bitset.first.size() )
    {
      throw(RbException("Cannot compare subsplits with unequal taxon sizes."));
    }

    // Check if s is equal to t's Y split
    RbBitSet s_total = s.asCladeBitset();
    if ( s_total == bitset.first )
    {
        return true;
    }
    // Check if s is equal to t's Z split
    else if (!is_fake && s_total == bitset.second) // Don't compare to clade Z if there is no clade Z
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
      return(bitset.first.getNumberSetBits());
    }
    else
    {
      return bitset.first.getNumberSetBits() + bitset.second.getNumberSetBits();
    }
}

/**
 * Are the two splits in a subsplit disjoint (nonoverlapping)?
 *
 * \return    true/false
 */
bool Subsplit::splitsAreDisjoint() const
{
    if ( !is_fake ) // A fake split cannot and need not be disjoint
    {
      for (size_t i=0; i < bitset.first.size(); ++i)
      {
        if (bitset.first[i] && bitset.second[i])
        {
          return false;
        }
      }
    }

    return true;
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
      std::string y = "[";
      for (size_t i=0; i<bitset.first.size(); ++i)
      {
          y += ( bitset.first.isSet(i) ? "1" : "0");
      }
      y += "]";

      s = y + "|" + y;
    }
    else
    {
      std::string y = "[";
      for (size_t i=0; i<bitset.first.size(); ++i)
      {
          y += ( bitset.first.isSet(i) ? "1" : "0");
      }
      y += "]";

      std::string z = "[";
      for (size_t i=0; i<bitset.second.size(); ++i)
      {
          z += ( bitset.second.isSet(i) ? "1" : "0");
      }
      z += "]";

      s = y + "|" + z;
    }
    return s;
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const Subsplit& x) {

    o << x.toString();

    return o;
}
