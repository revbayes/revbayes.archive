#ifndef RateMatrix_DoubletEpistaticGTR_H
#define RateMatrix_DoubletEpistaticGTR_H

#include "TimeReversibleRateMatrix.h"
#include <complex>
#include <vector>


namespace RevBayesCore {

    class EigenSystem;
    class TransitionProbabilityMatrix;



    /**
     * @brief Codon rate matrix class with different rates for synonymous and non-synonymous substitutions.
     *
     * This class implements the codon rate matrix with different dN/ dS rates.
     * The JC has no parameter but can be applied to any number of states.
     * The resulting rate matrix is computed by:
     *
     *        0: codon changes in more than one codon position (or stop codons)
     *        1: synonymous transition
     *        2: synonymous transversion
     *        3: non-synonymous transition
     *        4: non-synonymous transversion
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2015-07-15, version 1.0
     */
    class RateMatrix_DoubletEpistaticGTR : public TimeReversibleRateMatrix {

    public:
        RateMatrix_DoubletEpistaticGTR(void);                                                                                               //!< Construct rate matrix with n states
        RateMatrix_DoubletEpistaticGTR(const RateMatrix_DoubletEpistaticGTR& m);                                                                                //!< Copy constructor
        virtual                             ~RateMatrix_DoubletEpistaticGTR(void);                                                              //!< Destructor

        // overloaded operators
        RateMatrix_DoubletEpistaticGTR&             operator=(const RateMatrix_DoubletEpistaticGTR& r);

        // RateMatrix functions
        virtual RateMatrix_DoubletEpistaticGTR&     assign(const Assignable &m);                                                                                            //!< Assign operation that can be called on a base class instance.
        void                                                    calculateTransitionProbabilities(double startAge, double endAge, double rate, TransitionProbabilityMatrix& P) const;    //!< Calculate the transition matrix
        RateMatrix_DoubletEpistaticGTR*             clone(void) const;
        void                                                    setDoubletFrequencies(const std::vector<double> &f);                             //!< Set the nucleotide frequencies
        void                                                    setNucleotideExchangeRates(const std::vector<double> &e);
        void                                                    setD(double d);
        void                                                    update(void);
        static size_t                                           N1[];
        static size_t                                           N2[];
        static size_t                                           is_canonical[];

    private:
        void                                                    calculateCijk(void);                                                                //!< Do precalculations on eigenvectors and their inverse
        void                                                    computeOffDiagonal( void );
        void                                                    tiProbsEigens(double t, TransitionProbabilityMatrix& P) const;                      //!< Calculate transition probabilities for real case
        void                                                    tiProbsComplexEigens(double t, TransitionProbabilityMatrix& P) const;               //!< Calculate transition probabilities for complex case
        void                                                    updateEigenSystem(void);                                                            //!< Update the system of eigenvalues and eigenvectors

        EigenSystem*                                            eigen_system;                                                                       //!< Holds the eigen system
        std::vector<double>                                     c_ijk;                                                                              //!< Vector of precalculated product of eigenvectors and their inverse
        std::vector<std::complex<double> >                      cc_ijk;                                                                             //!< Vector of precalculated product of eigenvectors and thier inverse for complex case

        std::vector<double>                                     nucleotide_exchange_rates;
        double                                                  d;
        std::vector<double>                                     doublet_freqs;                                                                              //!< Vector of precalculated product of eigenvectors and their inverse

    };

}

#endif
