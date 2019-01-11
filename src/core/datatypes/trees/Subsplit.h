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
                                                    Subsplit(void);                                                            //! Default constructor: empty Subsplit
                                                    Subsplit(const Clade &c1, const std::vector<Taxon> &n );                                                //!< Default constructor: fake Subsplit
                                                    Subsplit(const Clade &c1, const Clade &c2, const std::vector<Taxon> &n );                               //!< Default constructor: real Subsplit
                                                    Subsplit(const std::pair<RbBitSet,RbBitSet> &b, const std::vector<Taxon> &n );                               //!< constructor: real Subsplit from subsplit
                                                    Subsplit(const RbBitSet &b1, const RbBitSet &b2 );                               //!< constructor: subsplit from two splits/bitset clades, no taxa (for internal use)
                                                    Subsplit(const std::pair<RbBitSet,RbBitSet> &b );                               //!< constructor: subsplit from subsplit, no taxa (for internal use)

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
//TODO: add functions, one for telling if a subsplit is a subsplit of Y, one for Z
        // public methods
        Clade                                       asClade(void) const;                                        //!< Get clades Y and Z as a single clade
        RbBitSet                                    asCladeBitset(void) const;                                  //!< Convert this value into a bitset representation for a clade.
        std::pair<RbBitSet,RbBitSet>                getBitset(void) const;                                      //!< Get the paired bitset representation of this subsplit as a clade
        Clade                                       getY(void) const;                                           //!< Get clade Y
        RbBitSet                                    getYBitset(void) const;                                     //!< Get clade Y as bitset
        Clade                                       getZ(void) const;                                           //!< Get clade Z
        RbBitSet                                    getZBitset(void) const;                                     //!< Get clade Y as bitset
        bool                                        isCompatible(const Subsplit &s) const;                      //!< Is argument subsplit compatible with this one?
        bool                                        isFake() const;                                             //!< Is this a fake subsplit (aka a single tip)?
        size_t                                      size(void) const;                                           //!< Get the number of taxa.
        std::string                                 toString(void) const;                                       //!< Convert this value into a string.

        // public TopologyNode functions

    private:

        // members
        // Clade                                       clade_y;
        // Clade                                       clade_z;
        std::pair<RbBitSet,RbBitSet>                bitset;
        bool                                        is_fake;
//TODO: remove taxa from core subsplit, make attribute solely of RevLanguage subsplit
//         this will require making things like getX and getY take taxa as arguments
        std::vector<Taxon>                          taxa;
    };

    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const Subsplit& x);                             //!< Overloaded output operator

}

#endif
