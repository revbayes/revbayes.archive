#ifndef RateGeneratorSequence_H
#define RateGeneratorSequence_H

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
    
    class RateGeneratorSequence : public Cloneable {
        
    public:
        RateGeneratorSequence(size_t ns, size_t nc);                                                          //!< Construct rate matrix with n states
        
        virtual                            ~RateGeneratorSequence(void);                                                                 //!< Destructor
        
        // public methods
        size_t                              getNumberOfStates(void) const;                      //!< Return the number of states
        size_t                              getNumberOfCharacters(void) const;                  //!< Return the number of characters
        size_t                              size(void) const;                                                                           //!< Get the size of the rate matrix, which is the same as the number of states

        // virtual public methods
        virtual void                        calculateTransitionProbabilities(TransitionProbabilityMatrix& P, double age=0.0) const;   //!< Calculate the transition probabilities for the rate matrix
        virtual void                        calculateTransitionProbabilities(TransitionProbabilityMatrix& P, double startAge, double endAge, double rate) const = 0;   //!< Calculate the transition matrixmatrix
        virtual RateGeneratorSequence*                    clone(void) const = 0;
//        virtual double                      getRate(std::vector<CharacterEvent*> from, CharacterEvent* to, unsigned* counts, double rate=1.0, double age=0.0) const = 0;
        virtual double                      getRate(std::vector<CharacterEvent*> from, CharacterEvent* to, double rate=1.0, double age=0.0) const = 0;
        virtual double                      getRate(size_t from, size_t to, double rate=1.0, double age=0.0) const = 0;
        virtual std::vector<double>         getStationaryFrequencies(void) const = 0;
        virtual double                      getSumOfRates(std::vector<CharacterEvent*> from, double rate=1.0, double age=0.0) const = 0;
        virtual double                      getSumOfRates(std::vector<CharacterEvent*> from, std::vector<size_t> counts, double rate=1.0, double age=0.0) const = 0;
        virtual void                        updateMap(void);
        
    protected:
        // protected members available for derived classes
        size_t                              numStates;                                          //!< The number of character states
        size_t                              numCharacters;                                      //!< The number of characters
        bool                                needsUpdate;
        
        
        
    };
    
    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const RateGeneratorSequence& x);                                           //!< Overloaded output operator
    
}

#endif
