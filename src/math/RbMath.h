#ifndef RbMath_H
#define RbMath_H

#ifndef PI
#	define PI 3.141592653589793
#endif

#include "MatrixReal.h"
#include "VectorReal.h"
#include <vector>

namespace RbMath {

		void                        backSubstitutionRow(MatrixReal& u, std::vector<double>& b);                       //!< Xxx 
		double                      beta(double a, double b);                                                         //!< Xxx 
        bool                        compApproximatelyEqual(double a, double b);                                       //!< Compares two doubles for equality
        bool                        compApproximatelyEqual(double a, double b, double epsilon);                       //!< Compares two doubles for equality
        bool                        compEssentiallyEqual(double a, double b);                                         //!< Compares two doubles for equality
        bool                        compEssentiallyEqual(double a, double b, double epsilon);                         //!< Compares two doubles for equality
        bool                        compDefinitelyGreaterThan(double a, double b);                                    //!< Tests whether one number is greater than another
        bool                        compDefinitelyGreaterThan(double a, double b, double epsilon);                    //!< Tests whether one number is greater than another
        bool                        compDefinitelyLessThan(double a, double b);                                       //!< Tests whether one number is less than another
        bool                        compDefinitelyLessThan(double a, double b, double epsilon);                       //!< Tests whether one number is less than another
		void                        computeLandU(MatrixReal& aMat, MatrixReal& lMat, MatrixReal& uMat);               //!< Xxx 
		int                         expMatrixPade(MatrixReal& A, MatrixReal& F, int qValue);                          //!< Xxx 
		double                      factorial(int x);                                                                 //!< Calculate the factorial 
		int                         findPadeQValue(const double& tolerance);                                          //!< Xxx 
		void                        forwardSubstitutionRow(MatrixReal& L, std::vector<double>& b);                    //!< Xxx 
		double                      gamma(double x);                                                                  //!< Calculate the Gamma function 
		void                        gaussianElimination(MatrixReal& a, MatrixReal& bMat, MatrixReal& xMat);           //!< Xxx 
		double                      hypotenuse(double a, double b);                                                   //!< Xxx 
		double                      incompleteBeta(double a, double b, double x);                                     //!< Xxx 
		double                      incompleteGamma(double x, double alpha, double scale);                            //!< Xxx 
        void                        matrixInverse(MatrixReal& a, MatrixReal& aInv);                                   //!< Computer the inverse of a matrix
		double                      lnGamma(double a);                                                                //!< Calculate the log of the Gamma function 
		double                      lnFactorial(int n);                                                               //!< Calculate the log factorial 
        void                        normalize(std::vector<double>& x, double sum);                                    //!< Normalize a vector so that its sum is sum
		double                      rbEpsilon(void);                                                                  //!< Xxx 
		int                         transposeMatrix(const MatrixReal& a, MatrixReal& t);                              //!< Transpose a matrix 
        void                        vectorMultiplicationColByRow(const VectorReal& v1, const VectorReal& v2, MatrixReal& p);    //!< Vector multiplication col by row
        void                        vectorMultiplicationRowByCol(const VectorReal& v1, const VectorReal& v2, Real& p);          //!< Vector multiplication row by col
}

#endif
