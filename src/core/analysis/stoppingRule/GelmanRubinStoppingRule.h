#ifndef GelmanRubinStoppingRule_H
#define GelmanRubinStoppingRule_H

#include "BurninEstimatorContinuous.h"
#include "StoppingRule.h"

#include <vector>

namespace RevBayesCore {
    
    /**
     * @brief The Gelman-Rubin stopping rule for convergence between multiple runs.
     *
     * This stopping rule returns true when the the variance of samples between runs 
     * is approximately as large as the variance within runs has been reached.
     * This rule is most useful if you want to check for convergence of continuous parameter
     * between runs.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since Version 1.0, 2015-03-09
     *
     */
    class GelmanRubinStoppingRule : public StoppingRule {
        
    public:
        GelmanRubinStoppingRule(double m, const std::string &fn, size_t fq, BurninEstimatorContinuous *be);
        GelmanRubinStoppingRule(const GelmanRubinStoppingRule &sr);
        virtual                                            ~GelmanRubinStoppingRule(void);                                   //!< Virtual destructor
        
        GelmanRubinStoppingRule&                            operator=(const GelmanRubinStoppingRule &st);
        
        // public methods
        bool                                                checkAtIteration(size_t g) const;                           //!< Should we check for convergence at the given iteration?
        GelmanRubinStoppingRule*                            clone(void) const;                                          //!< Clone function. This is similar to the copy constructor but useful in inheritance.
        bool                                                isConvergenceRule(void) const;                              //!< No, this is a threshold rule.
        void                                                runStarted(void);                                           //!< The run just started. Here we do not need to do anything.
        void                                                setNumberOfRuns(size_t n);                                  //!< Set how many runs/replicates there are.
        bool                                                stop(size_t g);                                             //!< Should we stop now?
        
    private:
        
        BurninEstimatorContinuous*                          burninEst;                                                  //!< The method for estimating the burnin
        size_t                                              checkFrequency;                                             //!< The frequency for checking for convergence
        std::string                                         filename;                                                   //!< The filename from which to read in the data
        double                                              R;                                                     //!< The minimum ESS threshold
        size_t                                              numReplicates;
        
    };
    
    // Global functions using the class
    std::ostream&                               operator<<(std::ostream& o, const GelmanRubinStoppingRule& x);               //!< Overloaded output operator
    
}

#endif
