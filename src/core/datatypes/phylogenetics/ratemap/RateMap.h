#ifndef RateMap_H
#define RateMap_H

#include "CharacterEvent.h"
#include "CharacterState.h"
#include "Cloneable.h"
#include "RateGenerator.h"
#include "RateMatrix.h"
#include "RbVector.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"
#include <vector>

namespace RevBayesCore {
    
    class RateMap : public Cloneable {
        
    public:
        RateMap(const RateMap& m);                                                              //!< Copy constructor
        RateMap(size_t ns, size_t nc);                                                          //!< Construct rate matrix with n states
        RateMap&                            operator=(const RateMap& r);
        
        virtual                            ~RateMap(void);                                                                 //!< Destructor
        
        // public methods
        size_t                              getNumberOfStates(void) const;                      //!< Return the number of states
        size_t                              getNumberOfCharacters(void) const;                  //!< Return the number of characters

        // virtual public methods
        virtual void                        calculateTransitionProbabilities(const TopologyNode& node, TransitionProbabilityMatrix& P) const;
        virtual void                        calculateTransitionProbabilities(const TopologyNode& node, TransitionProbabilityMatrix& P, size_t charIdx) const;
        virtual RateMap*                    clone(void) const;
        virtual double                      getRate(const TopologyNode& node, std::vector<CharacterEvent*> from, CharacterEvent* to, unsigned* counts, double age=0.0) const;
        virtual double                      getSiteRate(const TopologyNode& node, CharacterEvent* from, CharacterEvent* to, double age=0.0) const;
        virtual double                      getSiteRate(const TopologyNode& node, size_t from, size_t to, size_t charIdx=0, double age=0.0) const;
        virtual double                      getSumOfRates(const TopologyNode& node, std::vector<CharacterEvent*> from, double age=0.0) const;
        virtual double                      getSumOfRates(const TopologyNode& node, std::vector<CharacterEvent*> from, unsigned* counts, double age=0.0) const;
        virtual void                        updateMap(void);
        
        double                              getHomogeneousClockRate(void) const;
        void                                setHomogeneousClockRate(double d);
        const std::vector<double>&          getHeterogeneousClockRates(void) const;
        void                                setHeterogeneousClockRates(const std::vector<double>& r);
        const RateGenerator*                getHomogeneousRateMatrix(void) const;
        void                                setHomogeneousRateMatrix(const RateGenerator* r);
        const RbVector<RateGenerator>&      getHeterogeneousRateMatrices(void) const;
        void                                setHeterogeneousRateMatrices(const RbVector<RateGenerator>& r);
        const std::vector<double>&          getRootFrequencies(void) const;
        void                                setRootFrequencies(const RevBayesCore::RbVector<double>& r);


        
    protected:
        // protected members available for derived classes
        double                              homogeneous_clock_rate;
        std::vector<double>                 heterogeneous_clock_rates;
        RateGenerator*                      homogeneous_rate_matrix;
        RbVector<RateGenerator>             heterogeneous_rate_matrices;
        std::vector<double>                 root_frequencies;
        
        size_t                              num_states;                                          //!< The number of character states
        size_t                              numCharacters;                                      //!< The number of characters
        bool                                needs_update;
        bool                                branchHeterogeneousRateMatrices;
        bool                                branch_heterogeneous_clock_rates;
        
        
        
    };
    
    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const RateMap& x);                                           //!< Overloaded output operator
    
}

#endif
