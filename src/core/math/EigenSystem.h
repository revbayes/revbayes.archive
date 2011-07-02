/**
 * @file
 * This file contains the declaration of EigenSystem, which is
 * used to contain the eigen values and eigen vectors of a real matrix.
 *
 * @brief Declaration of EigenSystem
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 *
 * $Id:$
 */

#ifndef EigenSystem_H
#define EigenSystem_H

#include "MatrixComplex.h"
#include "MatrixReal.h"
#include "VectorComplex.h"
#include "VectorReal.h"



class EigenSystem {

    public:
                                    EigenSystem(const MatrixReal& m);                                      //!< Constructor from real matrix
                                   ~EigenSystem(void);                                                     //!< Destructor

        double                      getDeterminant(void);                                                  //!< return determinant
        MatrixReal&                 getEigenvectors(void) { return eigenvectors; }                         //!< return the eigenvector matrix
        MatrixReal&                 getInverseEigenvectors(void) { return inverseEigenvectors; }           //!< return the inverse eigenvector matrix
        VectorReal&                 getRealEigenvalues(void) { return realEigenvalues; }                   //!< return the real parts of the eigenvalues
        VectorComplex&              getImagEigenvalues(void) { return imaginaryEigenvalues; }              //!< return the imaginary parts of the eigenvalues
        MatrixComplex&              getComplexEigenvectors(void) { return complexEigenvectors; }           //!< return the eigenvector matrix
        MatrixComplex&              getComplexInverseEigenvectors() { return complexInverseEigenvectors; } //!< return the inverse eigenvector matrix
        bool                        getIsComplex(void) { return isComplex; } 

    private:
        size_t                      n;                                                                     //!< row and column dimension (square matrix)
        MatrixReal                  eigenvectors;                                                          //!< matrix for internal storage of eigenvectors
        MatrixReal                  inverseEigenvectors;                                                   //!< matrix for internal storage of the inverse eigenvectors
        MatrixComplex               complexEigenvectors;                                                   //!< matrix for internal storage of complex eigenvectors
        MatrixComplex               complexInverseEigenvectors;                                            //!< matrix for internal storage of the inverse of the complex eigenvectors
        VectorReal                  realEigenvalues;                                                       //!< vector for internal storage of the eigenvalues (real part)
        VectorComplex               imaginaryEigenvalues;                                                  //!< vector for internal storage of the eigenvalues (imaginary part)
        bool                        isComplex;
};

#endif
