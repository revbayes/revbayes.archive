#ifndef ConditionalClade_H
#define ConditionalClade_H

#include "Clade.h"

#include <vector>
#include <string>

namespace RevBayesCore {
    
    /**
     * Object describing conditional clades.
     *
     * A conditional clade is a clade given its parent clade.
     * Thus, this object simply holds two clade: a parent and a child clade.
     * The conditional clades are used for computing the conditional clade probabilities.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-02-26, version 1.0
     */
    class ConditionalClade  {
        
    public:
        ConditionalClade(const Clade &parent, const Clade &child);                                      //!< Default constructor with both child and parent clade       
               
        const Clade&                    getChild(void) const;                                           //!< Getter for child clade
        const Clade&                    getParent(void) const;                                          //!< Getter for parent clade
        
    private:    
        
        // members
        Clade                           child;
        Clade                           parent;
        
    };
        
}

#endif
