#ifndef RateMatrix_DECRateMatrix_H
#define RateMatrix_DECRateMatrix_H

#include "GeneralRateMatrix.h"
#include <complex>
#include <vector>
#include <map>


namespace RevBayesCore {
    
    class TransitionProbabilityMatrix;
    
    class RateMatrix_DECRateMatrix : public GeneralRateMatrix {
        
    public:
        RateMatrix_DECRateMatrix(size_t n);                                                                                         //!< Construct rate matrix with n states
        RateMatrix_DECRateMatrix(const RateMatrix_DECRateMatrix& m);                                                                //!< Copy constructor
        virtual                         ~RateMatrix_DECRateMatrix(void);                                                            //!< Destructor
        
        // overloaded operators
        RateMatrix_DECRateMatrix&           operator=(const RateMatrix_DECRateMatrix& r);
        
        // RateMatrix functions
        double                              averageRate(void) const;
        void                                calculateTransitionProbabilities(TransitionProbabilityMatrix& P, double startAge, double endAge, double rate) const;   //!< Calculate the transition matrix
        RateMatrix_DECRateMatrix*           clone(void) const;
        void                                fillRateMatrix(void);
        const RbVector<RbVector<double> >&  getDispersalRates(void) const;
        const std::vector<double>&          getExtirpationRates(void) const;                                                        //!< Return the extirpation rates
        const std::vector<double>&          getRangeSize(void) const;                                                               //!< Return the range size simplex

        void                                setDispersalRates(const RbVector<RbVector<double> >& dr);                               //!< Directly set dispersal rates
        void                                setExtirpationRates(const std::vector<double>& er);                                     //!< Directly set extirpation rates
        void                                setRangeSize(const std::vector<double>& rs);                                            //!< Directly set range size simplex

        void                                update(void);
        
    private:
        void                                calculateCijk(void);                                                                //!< Do precalculations on eigenvectors and their inverse
        void                                tiProbsEigens(double t, TransitionProbabilityMatrix& P) const;                      //!< Calculate transition probabilities for real case
        void                                tiProbsComplexEigens(double t, TransitionProbabilityMatrix& P) const;               //!< Calculate transition probabilities for complex case
        void                                updateEigenSystem(void);                                                            //!< Update the system of eigenvalues and eigenvectors
        
        
        void                                                makeBits(void);
        void                                                makeTransitions(void);
        
        std::vector<std::vector<unsigned> >                 bits;
        std::map<std::vector<unsigned>, unsigned>           inverseBits;
        std::vector<std::vector<unsigned> >                 transitions;
        std::vector<std::vector<unsigned> >                 lossOrGain;
        std::vector<std::vector<std::vector<unsigned> > >   transitionAreas;
        size_t                                              numCharacters;
        size_t                                              numStates;
        
        EigenSystem*                        theEigenSystem;                                                                     //!< Holds the eigen system
        std::vector<double>                 c_ijk;                                                                              //!< Vector of precalculated product of eigenvectors and their inverse
        std::vector<std::complex<double> >  cc_ijk;                                                                             //!< Vector of precalculated product of eigenvectors and thier inverse for complex case
        
        // members
        RbVector<RbVector<double> >            dispersalRates;
        std::vector<double>                    extirpationRates;
        std::vector<double>                    rangeSize;
    };
    
}

#endif /* defined(__revbayes_proj__RateMatrix_DECRateMatrix__) */
