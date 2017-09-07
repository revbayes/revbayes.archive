#ifndef __rb_mlandis__RateMatrix_FreeK__
#define __rb_mlandis__RateMatrix_FreeK__

#include "GeneralRateMatrix.h"

#include <complex>
#include <vector>


namespace RevBayesCore {
    
    class TransitionProbabilityMatrix;
    
    
    /**
     * @brief Free rate matrix class.
     *
     * This class implements the free rate matrix .
     * The resulting rate matrix is computed by:
     *
     *      |   -            r[1]      r[2]    ...    r[k]    |
     *      |                                                 |
     *      |  r[k+1]         -       r[k+2]   ...   r[2k]    |
     * Q =  |                                                 |
     *      |  r[(k-2)k+1]          ...      -     r[(k-1)k]  |
     *      |                                                 |
     *      |  r[(k-1)k+1]          ...                -      |
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Michael Landis)
     * @since 2014-07-04, version 1.0
     */
    class RateMatrix_FreeK : public GeneralRateMatrix {
        
    public:
        RateMatrix_FreeK(size_t k);                                                                                               //!< Construct rate matrix with n states
        RateMatrix_FreeK(size_t k, bool r);
        RateMatrix_FreeK(size_t k, bool r, std::string method);
        RateMatrix_FreeK(const RateMatrix_FreeK& m);                                                                                //!< Copy constructor
        virtual                         ~RateMatrix_FreeK(void);                                                              //!< Destructor
        
        // overloaded operators
        RateMatrix_FreeK&                   operator=(const RateMatrix_FreeK& r);
        
        // RateMatrix functions
        void                                calculateTransitionProbabilities(double startAge, double endAge, double rate, TransitionProbabilityMatrix& P) const;   //!< Calculate the transition matrix
        RateMatrix_FreeK*                   clone(void) const;
        void                                fillRateMatrix(void);
        void                                update(void);
        
    private:
        void                                calculateCijk(void);                                                                //!< Do precalculations on eigenvectors and their inverse
        void                                tiProbsEigens(double t, TransitionProbabilityMatrix& P) const;                      //!< Calculate transition probabilities for real case
        void                                tiProbsComplexEigens(double t, TransitionProbabilityMatrix& P) const;               //!< Calculate transition probabilities for complex case
        void                                tiProbsUniformization(double t, TransitionProbabilityMatrix& P) const;              //!< Calculate transition probabilities for uniformization
        void                                updateEigenSystem(void);                                                            //!< Update the system of eigenvalues and eigenvectors
        void                                updateUniformization(void);                                                         //!< Update the system for uniformization
        void                                expandUniformization(int truncation) const;
        
        bool                                rescale;
        bool                                useScalingAndSquaring;
        bool                                useUniformization;
        bool                                useEigen;
        
        // members for uniformization
        MatrixReal                          singleStepMatrix;
        std::vector<MatrixReal>*            matrixProducts;
        double                              maxRate;
        
        // the eigensystem
        EigenSystem*                        theEigenSystem;                                                                     //!< Holds the eigen system
        std::vector<double>                 c_ijk;                                                                              //!< Vector of precalculated product of eigenvectors and their inverse
        std::vector<std::complex<double> >  cc_ijk;                                                                             //!< Vector of precalculated product of eigenvectors and thier inverse for complex case
        
        void                                exponentiateMatrixByScalingAndSquaring(double t,  TransitionProbabilityMatrix& p) const;
        inline void                         multiplyMatrices(TransitionProbabilityMatrix& p,  TransitionProbabilityMatrix& q,  TransitionProbabilityMatrix& r) const;
    };
    
}

#endif /* defined(__rb_mlandis__RateMatrix_FreeK__) */
