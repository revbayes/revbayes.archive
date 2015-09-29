#ifndef __rb_mlandis__RateMatrix_FreeSymmetric__
#define __rb_mlandis__RateMatrix_FreeSymmetric__

#include "GeneralRateMatrix.h"
#include <complex>
#include <vector>


namespace RevBayesCore {
    
    class TransitionProbabilityMatrix;
    
    
    /**
     * @brief Free symmetric rate matrix class.
     *
     * This class implements the free symmetric rate matrix .
     * The resulting rate matrix is computed by:
     *
     *      |   -        r[1]    r[2]      ...         r[k-1]    |
     *      |                                                    |
     *      |  r[1]       -      r[k]      ...        r[2k-3]    |
     * Q =  |                                                    |
     *      |  r[2]         ...             -       r[(k-1)k/2]  |
     *      |                                                    |
     *      | r[k-1]        ...        r[(k-1)k/2]        -      |
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2015-09-29, version 1.0
     */
    class RateMatrix_FreeSymmetric : public GeneralRateMatrix {
        
    public:
        RateMatrix_FreeSymmetric(size_t k, bool r);                                                                             //!< Construct rate matrix with n states
        RateMatrix_FreeSymmetric(const RateMatrix_FreeSymmetric& m);                                                            //!< Copy constructor
        virtual                            ~RateMatrix_FreeSymmetric(void);                                                     //!< Destructor
        
        // overloaded operators
        RateMatrix_FreeSymmetric&           operator=(const RateMatrix_FreeSymmetric& r);
        
        // RateMatrix functions
//        double                              averageRate(void) const;
        void                                calculateTransitionProbabilities(double startAge, double endAge, double rate, TransitionProbabilityMatrix& P) const;   //!< Calculate the transition matrix
        RateMatrix_FreeSymmetric*           clone(void) const;
        void                                fillRateMatrix(void);
        void                                update(void);
        
    private:
        void                                calculateCijk(void);                                                                //!< Do precalculations on eigenvectors and their inverse
        void                                tiProbsEigens(double t, TransitionProbabilityMatrix& P) const;                      //!< Calculate transition probabilities for real case
        void                                tiProbsComplexEigens(double t, TransitionProbabilityMatrix& P) const;               //!< Calculate transition probabilities for complex case
        void                                updateEigenSystem(void);                                                            //!< Update the system of eigenvalues and eigenvectors
        
        EigenSystem*                        theEigenSystem;                                                                     //!< Holds the eigen system
        std::vector<double>                 c_ijk;                                                                              //!< Vector of precalculated product of eigenvectors and their inverse
        std::vector<std::complex<double> >  cc_ijk;                                                                             //!< Vector of precalculated product of eigenvectors and thier inverse for complex case
        bool                                rescaled;
        
    };
    
}

#endif /* defined(__rb_mlandis__RateMatrix_FreeSymmetric__) */
