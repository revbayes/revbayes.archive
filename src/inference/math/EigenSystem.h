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
 * $Id$
 */

#ifndef EigenSystem_H
#define EigenSystem_H

#include "Matrix.h"

#include <complex>


class EigenSystem {

    public:
                                                EigenSystem(Matrix<double>* m);                                                                                     //!< Constructor from rate matrix
    
        double                                  getDeterminant(void);                                                                                               //!< Return determinant
        const Matrix<double>&                   getEigenvectors(void) const { return eigenvectors; }                                                                //!< Return the eigenvector matrix
        const Matrix<double>&                   getInverseEigenvectors(void) const { return inverseEigenvectors; }                                                  //!< Return the inverse eigenvector matrix
        const std::vector<double>&              getRealEigenvalues(void) const { return realEigenvalues; }                                                          //!< Return the real parts of the eigenvalues
        const std::vector<double>&              getImagEigenvalues(void) const { return imaginaryEigenvalues; }                                                     //!< Return the imaginary parts of the eigenvalues
        const Matrix<std::complex<double> >&    getComplexEigenvectors(void) const { return complexEigenvectors; }                                                  //!< Return the eigenvector matrix
        const Matrix<std::complex<double> >&    getComplexInverseEigenvectors() const { return complexInverseEigenvectors; }                                        //!< Return the inverse eigenvector matrix
        bool                                    isComplex(void) const { return complex; } 
        void                                    setRateMatrixPtr(Matrix<double>* qp) { qPtr = qp; }
        bool                                    update(void);                                                                                                       //!< Update the eigen system for the matrix q;

    private:
        // functions used to calculate eigenvalues and eigenvectors 
        // @TODO Note, we should change most of these and move them to RbMath functions
        void                                    balance(Matrix<double>& A, std::vector<double>& scale, int* low, int* high);                                        //!< balances a matrix
        void                                    balback(int low, int high, std::vector<double>& scale, Matrix<double>& eivec);                                      //!< reverses the balancing
        bool                                    checkForComplexEigenvalues(void);                                                                                   //!< returns 'true' if there are complex eigenvalues
        void                                    complexLUBackSubstitution(Matrix<std::complex<double> >& a, int* indx, std::vector<std::complex<double> >& b);      //!< back-substitutes a complex LU-decomposed matrix
        bool                                    complexLUDecompose(Matrix<std::complex<double> >& a, double* vv, int* indx, double* pd);                            //!< calculates the LU-decomposition of a complex matrix
        void                                    elmhes(int low, int high, Matrix<double>& a, std::vector<int>& perm);                                               //!< reduces matrix to upper Hessenberg form
        void                                    elmtrans(int low, int high, Matrix<double>& a, std::vector<int>& perm, Matrix<double>& h);                          //!< copies the Hessenberg matrix
        int                                     hqr2(int low, int high, Matrix<double>& h, std::vector<double>& wr, std::vector<double>& wi, Matrix<double>& eivec); //!< computes eigenvalues and eigenvectors
        void                                    initializeComplexEigenvectors(void);                                                                                //!< sets up the complex eigenvector matrix
        int                                     invertMatrix(Matrix<double>& a, Matrix<double>& aInv);                                                              //!< inverts a matrix
        int                                     invertComplexMatrix(Matrix<std::complex<double> >& a, Matrix<std::complex<double> >& aInv);                         //!< inverts a complex matrix
        void                                    luBackSubstitution (Matrix<double>& a, int *indx, double *b);                                                       //!< back-substitutes an LU-decomposed matrix
        int                                     luDecompose(Matrix<double>& a, double *vv, int *indx, double *pd);                                                  //!< calculates the LU-decomposition of a matrix

        size_t                                  n;                                                                                                                  //!< Row and column dimension (square matrix)
        Matrix<double>*                         qPtr;                                                                                                               //!< A pointer to the rate matrix for this system of eigen values and vectors
        Matrix<double>                          eigenvectors;                                                                                                       //!< Matrix for internal storage of eigenvectors
        Matrix<double>                          inverseEigenvectors;                                                                                                //!< Matrix for internal storage of the inverse eigenvectors
        Matrix<std::complex<double> >           complexEigenvectors;                                                                                                //!< Matrix for internal storage of complex eigenvectors
        Matrix<std::complex<double> >           complexInverseEigenvectors;                                                                                         //!< Matrix for internal storage of the inverse of the complex eigenvectors
        std::vector<double>                     realEigenvalues;                                                                                                    //!< Vector for internal storage of the eigenvalues (real part)
        std::vector<double>                     imaginaryEigenvalues;                                                                                               //!< Vector for internal storage of the eigenvalues (imaginary part)
        bool                                    complex;                                                                                                            //!< Do we have imaginary eigenvectors
};

#endif
