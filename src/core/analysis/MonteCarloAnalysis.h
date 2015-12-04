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
        virtual                                            ~MonteCarloAnalysis(void);                                       //!< Virtual destructor
        
        MonteCarloAnalysis&                                 operator=(const MonteCarloAnalysis &a);
        
        // public methods
        void                                                addFileMonitorExtension(const std::string &s, bool dir);
        void                                                addMonitor(const Monitor &m);
        MonteCarloAnalysis*                                 clone(void) const;                                              //!< Clone function. This is similar to the copy constructor but useful in inheritance.
        void                                                burnin(size_t g, size_t ti, bool verbose=true);
        size_t                                              getCurrentGeneration(void) const;                               //!< Get the current generations number
        const Model&                                        getModel(void) const;
        void                                                printPerformanceSummary(void) const;
        void                                                removeMonitors(void);                                           //!< Remove all monitors
        void                                                run(size_t k, RbVector<StoppingRule> r, bool verbose=true);
        void                                                runPriorSampler(size_t k, RbVector<StoppingRule> r);
        virtual void                                        setActivePID(size_t i);                             //!< Set if the the sampler is active or inactive
        void                                                setModel(Model *m);
        virtual void                                        setNumberOfProcesses(size_t i);                     //!< Set the number of processes for this replication.

    protected:
        
        size_t                                              active_PID;
        size_t                                              num_processes;
        size_t                                              pid;
        bool                                                process_active;
        size_t                                              replicates;
        std::vector<MonteCarloSampler*>                     runs;
        
    };
    
    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const MonteCarloAnalysis& x);                       //!< Overloaded output operator
    
}

#endif
