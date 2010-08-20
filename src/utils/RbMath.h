#ifndef RbMath_H
#define RbMath_H

#ifndef PI
#	define PI 3.141592653589793
#endif

#include "MatrixReal.h"
#include <vector>

namespace RbMath {

		void                        backSubstitutionRow(MatrixReal& u, std::vector<double>& b);              //!< Xxx 
		double                      beta(double a, double b);                                                //!< Xxx 
		void                        computeLandU(MatrixReal& aMat, MatrixReal& lMat, MatrixReal& uMat);      //!< Xxx 
		int                         expMatrixPade(MatrixReal& A, MatrixReal& F, int qValue);                 //!< Xxx 
		double                      factorial(int x);                                                        //!< Calculate the factorial 
		int                         findPadeQValue(const double& tolerance);                                 //!< Xxx 
		void                        forwardSubstitutionRow(MatrixReal& L, std::vector<double>& b);           //!< Xxx 
		double                      gamma(double x);                                                         //!< Calculate the Gamma function 
		void                        gaussianElimination(MatrixReal& a, MatrixReal& bMat, MatrixReal& xMat);  //!< Xxx 
		double                      hypotenuse(double a, double b);                                          //!< Xxx 
		double                      incompleteBeta(double a, double b, double x);                            //!< Xxx 
		double                      incompleteGamma(double x, double alpha, double scale);                   //!< Xxx 
        bool                        isEqualTo(double x, double y, double tolerance);                         //!< Is x equal to y within a certain tolerance 
		double                      lnGamma(double a);                                                       //!< Calculate the log of the Gamma function 
		double                      lnFactorial(int n);                                                      //!< Calculate the log factorial 
        void                        normalize(std::vector<double>& x, double sum);                           //!< Normalize a vector so that its sum is sum
		double                      rbEpsilon(void);                                                         //!< Xxx 
		int                         transposeMatrix(const MatrixReal& a, MatrixReal& t);                     //!< Xxx 
}

#endif
