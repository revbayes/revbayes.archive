#ifndef RateMapUsingMatrix_H
#define RateMapUsingMatrix_H

#include "RateMap.h"
#include "RateMatrix.h"
#include "RbVector.h"
#include "TransitionProbabilityMatrix.h"
#include <vector>

namespace RevBayesCore {
    
    class RateMapUsingMatrix : public RateMap {
        
    public:
        RateMapUsingMatrix(const RateMapUsingMatrix& m);                                                              //!< Copy constructor
        RateMapUsingMatrix(size_t ns, size_t nc);                                                          //!< Construct rate matrix with n states
        RateMapUsingMatrix&                 operator=(const RateMapUsingMatrix& r);
        
        virtual                            ~RateMapUsingMatrix(void);                                                                 //!< Destructor
        
        
        // virtual public methods
        virtual void                        calculateTransitionProbabilities(double startAge, double endAge, double rate, TransitionProbabilityMatrix& P) const;   //!< Calculate the transition matrixmatrix
//        virtual void                        calculateTransitionProbabilities(TransitionProbabilityMatrix& P, size_t charIdx) const;
        virtual RateMapUsingMatrix*         clone(void) const;
        virtual double                      getRate(std::vector<CharacterEvent*> from, CharacterEvent* to, unsigned* counts, double rate=1.0, double age=0.0) const;
        virtual double                      getSiteRate(CharacterEvent* from, CharacterEvent* to, double rate=1.0, double age=0.0) const;
        virtual double                      getSiteRate(size_t from, size_t to, size_t charIdx=0, double rate=1.0, double age=0.0) const;
        virtual double                      getSumOfRates(std::vector<CharacterEvent*> from, double rate=1.0, double age=0.0) const;
        virtual double                      getSumOfRates(std::vector<CharacterEvent*> from, unsigned* counts, double rate=1.0, double age=0.0) const;
        
        void                                setRateMatrix(const RateGenerator *rm);
        virtual void                        updateMap(void);
        
        
    protected:
        // protected members available for derived classes
        RateGenerator*                      rateMatrix;
        
        
    };
    
    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const RateMapUsingMatrix& x);                                           //!< Overloaded output operator
    
}

#endif
