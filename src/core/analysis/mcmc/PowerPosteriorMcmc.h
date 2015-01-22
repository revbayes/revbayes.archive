//#ifndef PowerPosteriorMcmc_H
//#define PowerPosteriorMcmc_H
//
//#include "MonteCarloSampler.h"
//
//namespace RevBayesCore {
//    
//    /**
//     * @brief Power posterior MCMC class.
//     *
//     * A power posterior MCMC runs an MCMC for a vector of powers
//     * where the likelihood during each MCMC run is raised to the given power.
//     * The likelihood values and the current powers are stored in a file.
//     *
//     *
//     * @copyright Copyright 2009-
//     * @author The RevBayes Development Core Team (Sebastian Hoehna)
//     * @since Version 1.0, 2012-06-17
//     *
//     */
//    class PowerPosteriorMcmc : public MonteCarloSampler {
//        
//    public:
//        PowerPosteriorMcmc(const Model& m, const RbVector<Move> &moves, const std::string &fn);
//        virtual                                            ~PowerPosteriorMcmc(void);                                                          //!< Virtual destructor
//        
//        // public methods
//        PowerPosteriorMcmc*                                 clone(void) const;
//        void                                                run(size_t g);
//        
//        void                                                setPowers(const std::vector<double> &p);
//        void                                                setSampleFreq(size_t sf);
//        
//    private:
//        
//        // members
//        std::string                                         filename;
//        std::vector<double>                                 powers;
//        size_t                                              sampleFreq;
//        
//    };
//    
//}
//
//#endif
