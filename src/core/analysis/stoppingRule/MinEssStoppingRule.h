#ifndef MinEssStoppingRule_H
#define MinEssStoppingRule_H

#include "StoppingRule.h"

#include <vector>

namespace RevBayesCore {
    
    /**
     * @brief The min-ESS stopping rule.
     *
     * This stopping rule returns true when the minimum effective sample size (ESS) has been reached.
     * This rule is most useful if you want to guarantee that you have sufficiently many (effective) samples.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since Version 1.0, 2015-01-12
     *
     */
    class MinEssStoppingRule : public StoppingRule {
        
    public:
        MinEssStoppingRule(double m, size_t f);
        virtual                                            ~MinEssStoppingRule(void);                                   //!< Virtual destructor
        
        
        // public methods
        bool                                                checkAtIteration(size_t g) const;                           //!< Should we check for convergence at the given iteration?
        MinEssStoppingRule*                                 clone(void) const;                                          //!< Clone function. This is similar to the copy constructor but useful in inheritance.
        bool                                                isConvergenceRule(void) const;                              //!< No, this is a threshold rule.
        void                                                runStarted(void);                                           //!< The run just started. Here we do not need to do anything.
        bool                                                stop(size_t g);                                             //!< Should we stop now?
        
    private:
        
        size_t                                              checkFrequency;                                             //!< The frequency for checking for convergence
        double                                              minEss;                                                     //!< The minimum ESS threshold
        
    };
    
    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const MinEssStoppingRule& x);                 //!< Overloaded output operator
    
}

#endif
