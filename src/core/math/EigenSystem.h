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

#include "Complex.h"
#include "Matrix.h"
#include "Real.h"
#include "RbVector.h"



class EigenSystem {

    public:
                                    EigenSystem(Matrix<Real>* m);                                                                   //!< Constructor from rate matrix
                                    EigenSystem(const EigenSystem& e);                                                              //!< Copy constructor
                                   ~EigenSystem(void);                                                                              //!< Destructor
    
        EigenSystem*                clone(void) const;

        double                      getDeterminant(void);                                                                           //!< Return determinant
        const Matrix<Real>&         getEigenvectors(void) const { return eigenvectors; }                                            //!< Return the eigenvector matrix
        const Matrix<Real>&         getInverseEigenvectors(void) const { return inverseEigenvectors; }                              //!< Return the inverse eigenvector matrix
        const RbVector<Real>&       getRealEigenvalues(void) const { return realEigenvalues; }                                      //!< Return the real parts of the eigenvalues
        const RbVector<Real>&       getImagEigenvalues(void) const { return imaginaryEigenvalues; }                                 //!< Return the imaginary parts of the eigenvalues
        const Matrix<Complex>&      getComplexEigenvectors(void) const { return complexEigenvectors; }                              //!< Return the eigenvector matrix
        const Matrix<Complex>&      getComplexInverseEigenvectors() const { return complexInverseEigenvectors; }                    //!< Return the inverse eigenvector matrix
        bool                        isComplex(void) const { return complex; } 
        void                        setRateMatrixPtr(Matrix<Real>* qp) { qPtr = qp; }
        bool                        update(void);                                                                                   //!< Update the eigen system for the matrix q;

    private:
        // functions used to calculate eigenvalues and eigenvectors @TODO Note, we should change most of these and move them to RbMath functions
        void                        balance(Matrix<Real>& A, std::vector<double>& scale, int* low, int* high);                      //!< balances a matrix
        void                        balback(int low, int high, std::vector<double>& scale, Matrix<Real>& eivec);                    //!< reverses the balancing
        bool                        checkForComplexEigenvalues(void);                                                               //!< returns 'true' if there are complex eigenvalues
        void                        complexLUBackSubstitution(Matrix<Complex>& a, int* indx, RbVector<Complex>& b);                 //!< back-substitutes a complex LU-decomposed matrix
        bool                        complexLUDecompose(Matrix<Complex>& a, double* vv, int* indx, double* pd);                      //!< calculates the LU-decomposition of a complex matrix
        void                        elmhes(int low, int high, Matrix<Real>& a, std::vector<int>& perm);                             //!< reduces matrix to upper Hessenberg form
        void                        elmtrans(int low, int high, Matrix<Real>& a, std::vector<int>& perm, Matrix<Real>& h);          //!< copies the Hessenberg matrix
        int                         hqr2(int low, int high, Matrix<Real>& h, RbVector<Real>& wr, RbVector<Real>& wi, Matrix<Real>& eivec); //!< computes eigenvalues and eigenvectors
        void                        initializeComplexEigenvectors(void);                                                            //!< sets up the complex eigenvector matrix
        int                         invertMatrix(Matrix<Real>& a, Matrix<Real>& aInv);                                              //!< inverts a matrix
        int                         invertComplexMatrix(Matrix<Complex>& a, Matrix<Complex>& aInv);                                 //!< inverts a complex matrix
        void                        luBackSubstitution (Matrix<Real>& a, int *indx, double *b);                                     //!< back-substitutes an LU-decomposed matrix
        int                         luDecompose(Matrix<Real>& a, double *vv, int *indx, double *pd);                                //!< calculates the LU-decomposition of a matrix

        int                         n;                                                                                              //!< Row and column dimension (square matrix)
        Matrix<Real>*               qPtr;                                                                                           //!< A pointer to the rate matrix for this system of eigen values and vectors
        Matrix<Real>                eigenvectors;                                                                                   //!< Matrix for internal storage of eigenvectors
        Matrix<Real>                inverseEigenvectors;                                                                            //!< Matrix for internal storage of the inverse eigenvectors
        Matrix<Complex>             complexEigenvectors;                                                                            //!< Matrix for internal storage of complex eigenvectors
        Matrix<Complex>             complexInverseEigenvectors;                                                                     //!< Matrix for internal storage of the inverse of the complex eigenvectors
        RbVector<Real>              realEigenvalues;                                                                                //!< Vector for internal storage of the eigenvalues (real part)
        RbVector<Real>              imaginaryEigenvalues;                                                                           //!< Vector for internal storage of the eigenvalues (imaginary part)
        bool                        complex;                                                                                        //!< Do we have imaginary eigenvectors
};

#endif
