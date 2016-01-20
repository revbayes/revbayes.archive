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
        RateMap(size_t ns, size_t nc);                                                          //!< Construct rate matrix with n states
        
        virtual                            ~RateMap(void);                                                                 //!< Destructor
        
        // public methods
        size_t                              getNumberOfStates(void) const;                      //!< Return the number of states
        size_t                              getNumberOfCharacters(void) const;                  //!< Return the number of characters

        // virtual public methods
        virtual void                        calculateTransitionProbabilities(double startAge, double endAge, double rate, TransitionProbabilityMatrix& P) const = 0;   //!< Calculate the transition matrixmatrix
        virtual RateMap*                    clone(void) const = 0;
        virtual double                      getRate(std::vector<CharacterEvent*> from, CharacterEvent* to, unsigned* counts, double rate=1.0, double age=0.0) const = 0;
        virtual double                      getSumOfRates(std::vector<CharacterEvent*> from, double rate=1.0, double age=0.0) const = 0;
        virtual double                      getSumOfRates(std::vector<CharacterEvent*> from, unsigned* counts, double rate=1.0, double age=0.0) const = 0;
        virtual void                        updateMap(void);

        
    protected:
        // protected members available for derived classes
        size_t                              numStates;                                          //!< The number of character states
        size_t                              numCharacters;                                      //!< The number of characters
        bool                                needsUpdate;
        
        
        
    };
    
    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const RateMap& x);                                           //!< Overloaded output operator
    
}

#endif
