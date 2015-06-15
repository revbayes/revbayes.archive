#ifndef MonteCarloAnalysis_H
#define MonteCarloAnalysis_H

#include "Cloneable.h"
#include "MonteCarloSampler.h"
#include "RbVector.h"
#include "StoppingRule.h"

#include <vector>

namespace RevBayesCore {
    
    /**
     * @brief Monte Carlo analysis running and managing the MonteCarloSampler objects.
     *
     * The Monte Carlo Analysis object is mostly used to run independent MonteCarloSamplers
     * and check for convergence between them.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since Version 1.0, 2015-01-11
     *
     */
    class MonteCarloAnalysis : public Cloneable {
        
    public:
        MonteCarloAnalysis(MonteCarloSampler *m, size_t r);
        MonteCarloAnalysis(const MonteCarloAnalysis &m);
        virtual                                            ~MonteCarloAnalysis(void);                                   //!< Virtual destructor
        
        MonteCarloAnalysis&                                 operator=(const MonteCarloAnalysis &a);
        
        // public methods
        MonteCarloAnalysis*                                 clone(void) const;                                          //!< Clone function. This is similar to the copy constructor but useful in inheritance.
        void                                                burnin(size_t g, size_t ti);
        size_t                                              getCurrentGeneration(void) const;                           //!< Get the current generations number
        void                                                printPerformanceSummary(void) const;
        void                                                run(size_t k, RbVector<StoppingRule> r);
        void                                                runPriorSampler(size_t k, RbVector<StoppingRule> r);

    protected:
        
        size_t                                              activePID;
        size_t                                              numProcesses;
        size_t                                              pid;
        bool                                                processActive;
        size_t                                              replicates;
        std::vector<MonteCarloSampler*>                     runs;
        
    };
    
    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const MonteCarloAnalysis& x);                       //!< Overloaded output operator
    
}

#endif
