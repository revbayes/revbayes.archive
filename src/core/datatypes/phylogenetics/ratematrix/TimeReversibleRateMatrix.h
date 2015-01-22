#ifndef TimeReversibleRateMatrix_H
#define TimeReversibleRateMatrix_H

#include "MatrixReal.h"
#include "AbstractRateMatrix.h"
#include <complex>
#include <vector>


namespace RevBayesCore {
    
    class TimeReversibleRateMatrix : public AbstractRateMatrix {
        
    public:
        ~TimeReversibleRateMatrix(void);                                                                                                //!< Destructor
        
        
        // pure virtual method
        virtual void                        calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const = 0;       //!< Calculate the transition probabilities for the rate matrix
        virtual TimeReversibleRateMatrix*   clone(void) const = 0;
        virtual void                        updateMatrix(void) = 0;                                                                     //!< Update the rate entries of the matrix (is needed if stationarity freqs or similar have changed)

        // public methods
        virtual TimeReversibleRateMatrix&   assign(const Assignable &m);
        double                              averageRate(void) const;                                                                    //!< Calculate the average rate
        void                                computeOffDiagonal(void);
        const std::vector<double>&          getExchangeabilityRates(void) const;
        const std::vector<double>&          getStationaryFrequencies(void) const;                                                       //!< Return the stationary frequencies
        bool                                isTimeReversible(void);                                                                     //!< Return whether the rate matrix is time reversible
        void                                setExchangeabilityRates(const std::vector<double> &er);
        void                                setStationaryFrequencies(const std::vector<double>& f);                                     //!< Directly set the stationary frequencies
        
        
    protected:        
        TimeReversibleRateMatrix(size_t n);                                                                                             //!< Construct rate matrix with n states
    
        std::vector<double>                 exchangeabilityRates;
        std::vector<double>                 stationaryFreqs;                                                                            //!< Holds the stationary frequencies
        
    };
        
}

#endif

