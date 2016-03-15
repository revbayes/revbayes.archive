#ifndef RateGenerator_H
#define RateGenerator_H

#include "Assignable.h"
#include "Cloneable.h"
#include "CharacterEvent.h"
#include "MatrixReal.h"
#include <vector>

namespace RevBayesCore {
    
    class TransitionProbabilityMatrix;
    
    class RateGenerator : public Cloneable, public Assignable {
        
    public:
        virtual                             ~RateGenerator(void);
        
        virtual bool                        operator==(const RateGenerator &rm) const { return this == &rm; }
        virtual bool                        operator!=(const RateGenerator &rm) const { return !operator==(rm); }
        virtual bool                        operator<(const RateGenerator &rm) const { return this < &rm; }
        virtual bool                        operator<=(const RateGenerator &rm) const { return operator<(rm) || operator==(rm); }

        // pure virtual methods
        virtual RateGenerator&              assign(const Assignable &m) = 0;
        virtual void                        calculateTransitionProbabilities(double startAge, double endAge, double rate, TransitionProbabilityMatrix& P) const = 0;   //!< Calculate the transition matrixmatrix
        virtual RateGenerator*              clone(void) const = 0;
        virtual double                      getRate(size_t from, size_t to, double age, double rate) const = 0;                         //!< Calculate the rate from state i to state j over the given time interval scaled by a rate
        virtual double                      getSumOfRates(std::vector<CharacterEvent*> from, double rate=1.0, double age=0.0) const;
        virtual double                      getSumOfRates(std::vector<CharacterEvent*> from, const std::vector<size_t> &counts, double rate=1.0, double age=0.0) const;

        // virtual methods that may need to overwritten
        virtual void                        update(void) {};
        
        // public methods
        void                                calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const;           //!< Calculate the transition probabilities for the rate matrix
        size_t                              getNumberOfStates(void) const;                                                              //!< Return the number of states
        size_t                              size(void) const;                                                                           //!< Get the size of the rate matrix, which is the same as the number of states
        
    protected:
        // prevent instantiation
        RateGenerator(size_t n);                                                                                                        //!< Construct rate matrix with n states
        
        // protected members available for derived classes
        size_t                              numStates;                                                                                  //!< The number of character states

        
    };
    
    std::ostream&                       operator<<(std::ostream& o, const RateGenerator& x);                                           //!< Overloaded output operator

};
#endif
