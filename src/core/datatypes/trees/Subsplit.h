#ifndef Subsplit_H
#define Subsplit_H

#include <map>
#include <set>
#include <string>
#include <vector>

#include "Clade.h"
#include "RbBitSet.h"
#include "RbException.h"
#include "Taxon.h"

namespace RevBayesCore {

    /**
     * Object describing subsplits.
     *
     * A subsplit is simply a pair of clades.
     *
     * @copyright Copyright 2009-
     * @author Andrew Magee
     * @since 2018-11-23, version 1.0
     */
    class Subsplit  {

    public:
      // TODO: initializations are bonkers
      //       1) clades from trees requires getting taxa then making clades, use vector<Taxon>
      //       2) we never use any of the bitset initializers
      //       3) storing taxa is wasteful
      //       4) update RevLanguage subsplit to handle this, or get rid of it?? eh, it's probably cheaper to keep'er
                                                    Subsplit(void);                                                            //! Default constructor: empty Subsplit
                                                    Subsplit(const std::vector<Taxon> &c1, const std::vector<Taxon> &n );                                                //!< Default constructor: fake Subsplit
                                                    Subsplit(const std::vector<Taxon> &c1, const std::vector<Taxon> &c2, const std::vector<Taxon> &n );                               //!< Default constructor: real Subsplit

        virtual                                    ~Subsplit(void) {}

        // overloaded operators
        Subsplit&                                   operator=(const Subsplit &t);
        bool                                        operator==(const Subsplit &t) const;
        bool                                        operator!=(const Subsplit &t) const;
        bool                                        operator<(const Subsplit &t) const;
        bool                                        operator<=(const Subsplit &t) const;
        bool                                        operator>(const Subsplit &t) const;
        bool                                        operator>=(const Subsplit &t) const;


        // Basic utility functions
        Subsplit*                                      clone(void) const;                                          //!< Clone object

        // public methods
        Clade                                       asClade(const std::vector<Taxon> &taxa) const;                                        //!< Get clades Y and Z as a single clade
        RbBitSet                                    asCladeBitset(void) const;                                  //!< Convert this value into a bitset representation for a clade.
        std::pair<RbBitSet,RbBitSet>                getBitset(void) const;                                      //!< Get the paired bitset representation of this subsplit as a clade
        Clade                                       getY(const std::vector<Taxon>& taxa) const;                                           //!< Get clade Y
        RbBitSet                                    getYBitset(void) const;                                     //!< Get clade Y as bitset
        Clade                                       getZ(const std::vector<Taxon>& taxa) const;                                           //!< Get clade Z
        RbBitSet                                    getZBitset(void) const;                                     //!< Get clade Y as bitset
        bool                                        isChildOfY(const Subsplit &s) const;                      //!< Is argument subsplit compatible with this one?
        bool                                        isChildOfZ(const Subsplit &s) const;                      //!< Is argument subsplit compatible with this one?
        bool                                        isCompatible(const Subsplit &s) const;                      //!< Is argument subsplit compatible with this one?
        bool                                        isFake() const;                                             //!< Is this a fake subsplit (aka a single tip)?
        size_t                                      size(void) const;                                           //!< Get the number of taxa.
        bool                                        splitsAreDisjoint(void) const;
        std::string                                 toString(void) const;                                       //!< Convert this value into a string.

        // public TopologyNode functions

    private:

        // members
        std::pair<RbBitSet,RbBitSet>                bitset;
        bool                                        is_fake;
    };

    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const Subsplit& x);                             //!< Overloaded output operator

}

#endif
