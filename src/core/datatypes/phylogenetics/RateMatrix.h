/**
 * @file
 * This file contains the declaration of RateMatrix, which is
 * class that holds a rate matrix for a continuous-time Markov model.
 *
 * @brief Declaration of RateMatrix
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef RateMatrix_H
#define RateMatrix_H

#include "MemberObject.h"
#include <complex>
#include <vector>

class ArgumentRule;
class DAGNode;
class EigenSystem;
class MatrixReal;
class Simplex;
class TransitionProbabilityMatrix;
class VectorReal;
class VectorString;



const std::string RateMatrix_name = "Rate Matrix";

class RateMatrix : public MemberObject {

    public:
                                            RateMatrix(void);                                                                 //!< Default constructor (never call this except from the workspace once)
                                            RateMatrix(const RateMatrix& m);                                                  //!< Copy constructor
                                            RateMatrix(size_t n);                                                             //!< Construct rate matrix with n states
                                           ~RateMatrix(void);                                                                 //!< Destructor
        RbPtr<VectorReal>                   operator[](size_t i);                                                             //!< Subscript operator
        RbPtr<const VectorReal>             operator[](size_t i) const;                                                       //!< Subscript operator (const)
    
        // Basic utility functions
        RateMatrix*                         clone(void) const;                                                                  //!< Clone object
        const VectorString&                 getClass(void) const;                                                               //!< Get class vector
        const TypeSpec&                     getTypeSpec(void) const;                                                            //!< Get language type of the object   
        void                                printValue(std::ostream& o) const;                                                  //!< Print value for user
        std::string                         richInfo(void) const;                                                               //!< Complete info

        // Member variable rules
        RbPtr<const MemberRules>            getMemberRules(void) const;                                                         //!< Get member rules

        // Member method inits
        RbPtr<const MethodTable>            getMethods(void) const;                                                             //!< Get methods
        RbPtr<RbLanguageObject>             executeOperation(const std::string& name, const RbPtr<Environment>& args);                       //!< Map method call to internal functions
    
        // RateMatrix functions
        double                              averageRate(void) const;                                                            //!< Calculate the average rate
        void                                calculateStationaryFrequencies(void);                                               //!< Calculate the stationary frequencies for the rate matrix
        void                                calculateTransitionProbabilities(double t, RbPtr<TransitionProbabilityMatrix> P) const; //!< Calculate the transition probabilities for the rate matrix
        bool                                getAreEigensDirty(void) { return areEigensDirty; }                                  //!< Returns whether the eigensystem is in need of recalculation
        bool                                getIsTimeReversible(void);                                                          //!< Return whether the rate matrix is time reversible
        size_t                              getNumberOfStates(void) const { return numStates; }                                 //!< Return the number of states
        void                                rescaleToAverageRate(const double r);                                               //!< Rescale the rate matrix such that the average rate is "r"
        void                                setDiagonal(void);                                                                  //!< Set the diagonal such that each row sums to zero
        void                                setIsTimeReversible(const bool tf);                                                 //!< Directly set whether the rate matrix is time reversible
        void                                setStationaryFrequencies(const std::vector<double>& f);                             //!< Directly set the stationary frequencies
        void                                updateEigenSystem(void);                                                            //!< Update the system of eigenvalues and eigenvectors

    private:
        void                                calculateCijk(void);                                                                //!< Do precalculations on eigenvectors and their inverse
        bool                                checkTimeReversibity(double tolerance);                                             //!< Checks if the rate matrix is time reversible
        void                                tiProbsEigens(const double t, RbPtr<TransitionProbabilityMatrix> P) const;          //!< Calculate transition probabilities for real case
        void                                tiProbsComplexEigens(const double t, RbPtr<TransitionProbabilityMatrix> P) const;   //!< Calculate transition probabilities for complex case

        bool                                areEigensDirty;                                                                     //!< Does the eigensystem need to be recalculated
        bool                                reversibilityChecked;                                                               //!< Flag indicating if time reversibility has been checked
        bool                                isReversible;                                                                       //!< Is the matrix time reversible
        size_t                              numStates;                                                                          //!< The number of character states
        RbPtr<MatrixReal>                   theRateMatrix;                                                                      //!< Holds the rate matrix
        RbPtr<Simplex>                      theStationaryFreqs;                                                                 //!< Holds the stationary frequencies
        RbPtr<EigenSystem>                  theEigenSystem;                                                                     //!< Holds the eigen system
        std::vector<double>                 c_ijk;                                                                              //!< Vector of precalculated product of eigenvectors and their inverse
        std::vector<std::complex<double> >  cc_ijk;                                                                             //!< Vector of precalculated product of eigenvectors and thier inverse for complex case
    
        static const TypeSpec               typeSpec;
};

#endif

