#ifndef PowerPosteriorAnalysis_H
#define PowerPosteriorAnalysis_H

#include "Cloneable.h"
#include "RbVector.h"

namespace RevBayesCore {
    
    class MonteCarloSampler;
    
    /**
     * @brief Power posterior analysis class.
     *
     * A power posterior analysis runs an analysis for a vector of powers
     * where the likelihood during each analysis run is raised to the given power.
     * The likelihood values and the current powers are stored in a file.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since Version 1.0, 2012-06-17
     *
     */
    class PowerPosteriorAnalysis : public Cloneable {
        
    public:
        PowerPosteriorAnalysis(MonteCarloSampler *m, const std::string &fn);
        PowerPosteriorAnalysis(const PowerPosteriorAnalysis &a);
        virtual                                ~PowerPosteriorAnalysis(void);                               //!< Virtual destructor
        
        PowerPosteriorAnalysis&                 operator=(const PowerPosteriorAnalysis &a);
        
        // public methods
        PowerPosteriorAnalysis*                 clone(void) const;
        void                                    burnin(size_t g, size_t ti);
        void                                    runAll(size_t g);
        void                                    runStone(size_t idx, size_t g);
        void                                    summarizeStones(void);
        void                                    setPowers(const std::vector<double> &p);
        void                                    setSampleFreq(size_t sf);
        
    private:
        
        // members
        size_t                                  activePID;
        std::string                             filename;
        size_t                                  numProcesses;
        size_t                                  pid;
        std::vector<double>                     powers;
        bool                                    processActive;
        MonteCarloSampler*                      sampler;
        size_t                                  sampleFreq;

    };
    
}

#endif
