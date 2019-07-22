#ifndef RateMatrix_ExperimentalCodonModel_H
#define RateMatrix_ExperimentalCodonModel_H

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
     * The underlying mutation model is HKY, the selection model is from Bloom(2014) and Hilton et al (2017).
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
     * @author The RevBayes Development Core Team
     * @since 2015-07-15, version 1.0
     */
    class RateMatrix_ExperimentalCodonModel : public TimeReversibleRateMatrix {

    public:
        RateMatrix_ExperimentalCodonModel(void);                                                                                               //!< Construct rate matrix with n states
        RateMatrix_ExperimentalCodonModel(const RateMatrix_ExperimentalCodonModel& m);                                                                                //!< Copy constructor
        virtual                             ~RateMatrix_ExperimentalCodonModel(void);                                                              //!< Destructor

        // overloaded operators
        RateMatrix_ExperimentalCodonModel&             operator=(const RateMatrix_ExperimentalCodonModel& r);

        // RateMatrix functions
        virtual RateMatrix_ExperimentalCodonModel&     assign(const Assignable &m);                                                                                            //!< Assign operation that can be called on a base class instance.
        void                                                    calculateTransitionProbabilities(double startAge, double endAge, double rate, TransitionProbabilityMatrix& P) const;    //!< Calculate the transition matrix
        RateMatrix_ExperimentalCodonModel*             clone(void) const;
        void                                                    setAminoAcidPreferences(const std::vector<double> &p);
        void                                                    setBeta(double b);
        void                                                    setKappa(double k);
        void                                                    setOmega(double o);
        void                                                    setNucleotideFrequencies(const std::vector<double> &f);                             //!< Set the nucleotide frequencies
        void                                                    update(void);
        void                                                    rescaleToOne(double mu);
    private:
        void                                                    calculateCijk(void);                                                                //!< Do precalculations on eigenvectors and their inverse
        void                                                    computeOffDiagonal( void );
        void                                                    tiProbsEigens(double t, TransitionProbabilityMatrix& P) const;                      //!< Calculate transition probabilities for real case
        void                                                    tiProbsComplexEigens(double t, TransitionProbabilityMatrix& P) const;               //!< Calculate transition probabilities for complex case
        void                                                    updateEigenSystem(void);                                                            //!< Update the system of eigenvalues and eigenvectors

        EigenSystem*                                            eigen_system;                                                                       //!< Holds the eigen system
        std::vector<double>                                     c_ijk;                                                                              //!< Vector of precalculated product of eigenvectors and their inverse
        std::vector<std::complex<double> >                      cc_ijk;                                                                             //!< Vector of precalculated product of eigenvectors and thier inverse for complex case

        double                                                  kappa;
        double                                                  omega;
        double                                                  beta;
        std::vector<double>                                     nucleotide_freqs;                                                                              //!< Vector of precalculated product of eigenvectors and their inverse
        std::vector<double>                                     amino_acid_preferences;                                                                              //!< Vector of precalculated product of eigenvectors and their inverse

    };

}

#endif
