#ifndef StoppingRule_H
#define StoppingRule_H

#include "Cloneable.h"

#include <vector>

namespace RevBayesCore {
    
    /**
     * @brief The stopping rule interface for the MonteCarloAnalysis.
     *
     * A stopping rule checks whether to stop or to continue a MonteCarloAnalysis.
     * This interface specifies the pure virtual functions for stopping rules.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since Version 1.0, 2015-01-11
     *
     */
    class StoppingRule : public Cloneable {
        
    public:
        virtual                                            ~StoppingRule(void) {}                                         //!< Virtual destructor
        
        
        // public methods
        virtual StoppingRule*                               clone(void) const = 0;                                      //!< Clone function. This is similar to the copy constructor but useful in inheritance.
        virtual bool                                        stop(void) = 0;                                             //!< Should we stop now?
        
    protected:
        StoppingRule() {}
        
        
    };
    
    // Global functions using the class
//    std::ostream&                       operator<<(std::ostream& o, const StoppingRule& x);                             //!< Overloaded output operator
    
}

#endif
