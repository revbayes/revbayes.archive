#ifndef RateGeneratorSequenceUsingMatrix_H
#define RateGeneratorSequenceUsingMatrix_H

#include "RateGeneratorSequence.h"
#include "RateMatrix.h"
#include "RbVector.h"
#include "TransitionProbabilityMatrix.h"
#include <vector>

namespace RevBayesCore {
    
    class RateGeneratorSequenceUsingMatrix : public RateGeneratorSequence {
        
    public:
        RateGeneratorSequenceUsingMatrix(const RateGeneratorSequenceUsingMatrix& m);                                                              //!< Copy constructor
        RateGeneratorSequenceUsingMatrix(size_t ns, size_t nc);                                                          //!< Construct rate matrix with n states
        RateGeneratorSequenceUsingMatrix&                 operator=(const RateGeneratorSequenceUsingMatrix& r);
        
        virtual                            ~RateGeneratorSequenceUsingMatrix(void);                                                                 //!< Destructor
        
        
        // virtual public methods
        virtual void                        calculateTransitionProbabilities(TransitionProbabilityMatrix& P, double age=0.0) const;   //!< Calculate the transition probabilities for the rate matrix
        virtual void                        calculateTransitionProbabilities(TransitionProbabilityMatrix& P, double startAge, double endAge, double rate) const;   //!< Calculate the transition matrixmatrix
//        virtual void                        calculateTransitionProbabilities(TransitionProbabilityMatrix& P, size_t charIdx) const;
        virtual RateGeneratorSequenceUsingMatrix*         clone(void) const;
        virtual double                      getRate(std::vector<CharacterEvent*> from, CharacterEvent* to, unsigned* counts, double rate=1.0, double age=0.0) const;
        virtual double                      getRate(std::vector<CharacterEvent*> from, CharacterEvent* to, double rate=1.0, double age=0.0) const;
        virtual double                      getRate(size_t from, size_t to, double rate=1.0, double age=0.0) const;
        virtual double                      getSiteRate(CharacterEvent* from, CharacterEvent* to, double rate=1.0, double age=0.0) const;
        virtual double                      getSiteRate(size_t from, size_t to, size_t charIdx=0, double rate=1.0, double age=0.0) const;
        virtual std::vector<double>         getStationaryFrequencies(void) const;
        virtual double                      getSumOfRates(std::vector<CharacterEvent*> from, double rate=1.0, double age=0.0) const;
        virtual double                      getSumOfRates(std::vector<CharacterEvent*> from, std::vector<size_t> counts, double rate=1.0, double age=0.0) const;
        
        void                                setRateMatrix(const RateGenerator *rm);
        virtual void                        updateMap(void);
        
        
    protected:
        // protected members available for derived classes
        RateGenerator*                      rateMatrix;
        
        
    };
    
    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const RateGeneratorSequenceUsingMatrix& x);                                           //!< Overloaded output operator
    
}

#endif
