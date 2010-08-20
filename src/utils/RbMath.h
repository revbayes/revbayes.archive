#ifndef RbMath_H
#define RbMath_H

#ifndef PI
#	define PI 3.141592653589793
#endif

#include "RbMatrix.h"
#include <vector>

namespace RbMath {

		void                        backSubstitutionRow(RbMatrix<double> &u, std::vector<double>& b);                          /*!< Xxx */
		double                      beta(double a, double b);                                                                  /*!< Xxx */
		void                        computeLandU(RbMatrix<double> &aMat, RbMatrix<double> &lMat, RbMatrix<double> &uMat);      /*!< Xxx */
		int                         expMatrixPade(RbMatrix<double>& A, RbMatrix<double>& F, int qValue);                       /*!< Xxx */
		double                      factorial(int x);                                                                          /*!< Xxx */
		int                         findPadeQValue(const double& tolerance);                                                   /*!< Xxx */
		void                        forwardSubstitutionRow(RbMatrix<double>& L, std::vector<double>& b);                       /*!< Xxx */
		double                      gamma(double x);                                                                           /*!< Xxx */
		void                        gaussianElimination(RbMatrix<double>& a, RbMatrix<double>& bMat, RbMatrix<double>& xMat);  /*!< Xxx */
		double                      hypotenuse(double a, double b);                                                            /*!< Xxx */
		double                      incompleteBeta(double a, double b, double x);                                              /*!< Xxx */
		double                      incompleteGamma(double x, double alpha, double scale);                                     /*!< Xxx */
        bool                        isEqualTo(double x, double y, double tolerance);                                           /*!< Is x equal to y within a certain tolerance */
		double                      lnGamma(double a);                                                                         /*!< Xxx */
		double                      lnFactorial(int n);                                                                        /*!< Xxx */
		double                      rbEpsilon(void);                                                                           /*!< Xxx */
		int                         transposeMatrix(const RbMatrix<double>& a, RbMatrix<double>& t);                           /*!< Xxx */
}

#endif
