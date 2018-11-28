#include "Subsplit.h"
#include "RbVectorUtilities.h"

#include <algorithm>
#include <iostream>
#include <sstream>


using namespace RevBayesCore;


/**
 * Default constructor required by the revlanguage code.
 * We use two empty strings for this default object.
 */
Subsplit::Subsplit( void ) :
    clade_y(),
    clade_z(),
    is_fake()
{

}

/**
 * Constructor from single taxon (as a clade)
 */
Subsplit::Subsplit( const Clade &c1 ) :
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
}


/**
 * Constructor from clades.
 */
Subsplit::Subsplit( const Clade &c1, const Clade &c2 ) :
    clade_y(),
    clade_z(),
    is_fake( false )
{
    // Check that X and Y are disjoint
    bool disjoint = true;
    RbBitSet c1_bit = c1.getBitRepresentation();
    RbBitSet c2_bit = c2.getBitRepresentation();
    for (size_t i=0; i < c1_bit.size(); ++i)
    {
      if ( c1_bit.isSet(i) && c2_bit.isSet(i) ) {
        disjoint = false;
        break;
      }
    }

    if (!disjoint)
    {
      throw(RbException("Cannot create subsplit from non-disjoint clades."));
    }

    // Order clades
    if ( c1 < c2 )
    {
      clade_y = c1;
      clade_z = c2;
    }
    else
    {
      clade_y = c2;
      clade_z = c1;
    }
}


/**
 * Overloaded equals operator.
 * Only if both clades in both subsplits are equal are two Subsplits are equal.
 */
bool Subsplit::operator==(const Subsplit &s) const
{

    if ( s.getY() == clade_y && s.getZ() == clade_z )
    {
        return true;
    }

    return false;
}


/**
 * Not equals operator that uses the equals operator.
 */
bool Subsplit::operator!=(const Subsplit &s) const
{
    return !operator==( s );
}


/**
 * Less than operator so that we can sort the Subsplits.
 */
bool Subsplit::operator<(const Subsplit &s) const
{
  throw(RbException("Operator < is not defined on subsplits at this time"));
}


/**
 * Less than operator so that we can sort the Subsplits.
 */
bool Subsplit::operator<=(const Subsplit &s) const
{
  throw(RbException("Operator <= is not defined on subsplits at this time"));
}


/**
 * Less than operator so that we can sort the Subsplits.
 */
bool Subsplit::operator>(const Subsplit &s) const
{
  throw(RbException("Operator > is not defined on subsplits at this time"));
}


/**
 * Less than operator so that we can sort the Subsplits.
 */
bool Subsplit::operator>=(const Subsplit &s) const
{
  throw(RbException("Operator >= is not defined on subsplits at this time"));
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
      s = clade_y.toString() + "|" clade_y.toString();
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
