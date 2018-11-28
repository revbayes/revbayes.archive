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
                                                    Subsplit(const Clade &c1 );                                                //!< Default constructor: fake Subsplit
                                                    Subsplit(const Clade &c1, const Clade &c2 );                               //!< Default constructor: real Subsplit

        virtual                                    ~Subsplit(void) {}

        // overloaded operators
        bool                                        operator==(const Subsplit &t) const;
        bool                                        operator!=(const Subsplit &t) const;
        bool                                        operator<(const Subsplit &t) const;
        bool                                        operator<=(const Subsplit &t) const;
        bool                                        operator>(const Subsplit &t) const;
        bool                                        operator>=(const Subsplit &t) const;


        // Basic utility functions
        Subsplit*                                      clone(void) const;                                          //!< Clone object

        // public methods
        Clade                                       asClade(void) const;                                        //!< Get clades Y and Z as a single clade
        Clade                                       getY(void) const;                                           //!< Get clade Y
        Clade                                       getZ(void) const;                                           //!< Get clade Z
        bool                                        isCompatible(const Subsplit &s) const;                      //!< Is argument subsplit compatible with this one?
        bool                                        isFake() const;                                             //!< Is this a fake subsplit (aka a single tip)?
        size_t                                      size(void) const;                                           //!< Get the number of taxa.
        std::string                                 toString(void) const;                                       //!< Convert this value into a string.

        // public TopologyNode functions

    private:

        // members
        Clade                                       clade_y;
        Clade                                       clade_z;
        bool                                        is_fake;

    };

    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const Subsplit& x);                             //!< Overloaded output operator

}

#endif
