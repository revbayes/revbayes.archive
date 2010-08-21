#include "RbMath.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>



// Math Functions

/*!
 * Back-substitution of Gaussian elimination
 * 
 * \brief Back-substitution
 * \param u Matrix to back substitute
 * \param b Solution vector
 * \return Returns nothing
 */
void RbMath::backSubstitutionRow(MatrixReal& u, std::vector<double>& b) {

	int n = u.getNumRows();
	b[n-1] /= u[n-1][n-1];
	for (int i=n-2; i>=0; i--) 
		{
		double dotProduct = 0.0;
		for (int j=i+1; j<n; j++)
			dotProduct += u[i][j] * b[j];
		b[i] = (b[i] - dotProduct) / u[i][i];
		}
}

/*!
 * This function calculates the beta function.
 *
 * \brief Beta function.
 * \param a is an argument. 
 * \param b is an argument. 
 * \return Returns the value of the beta function. 
 * \throws Does not throw an error.
 */
double RbMath::beta(double a, double b) {

	return ( exp(RbMath::lnGamma(a) + RbMath::lnGamma(b) - RbMath::lnGamma(a + b)) );
}

/*!
 * The following functions check whether two double-precision real
 * numbers are equal. They are described in:
 *
 * Knuth, D. E. 1981. The art of computer programming: Seminumerical
 *    algorithms, Volume 2. Addison-Wesley.
 *
 * Note that approximately equal to is more stringent than essentially
 * equal to.
 *
 * \brief Comparison functions
 * \param a First double-precision number
 * \param b Second double-precision number
 * \param epsilon How close should the numbers be
 * \return true / false
 */
bool RbMath::compApproximatelyEqual(double a, double b, double epsilon) {

    return fabs(a - b) <= ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

bool RbMath::compEssentiallyEqual(double a, double b, double epsilon) {

    return fabs(a - b) <= ( (fabs(a) > fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

bool RbMath::compDefinitelyGreaterThan(double a, double b, double epsilon) {

    return (a - b) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

bool RbMath::compDefinitelyLessThan(double a, double b, double epsilon) {

    return (b - a) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

/*!
 * This function computes the L and U decomposition of a matrix. Basically,
 * we find matrices lMat and uMat such that: lMat * uMat = aMat
 *
 * \brief Compute LU decomposition
 * \param aMat The matrix to LU decompose (destroyed)
 * \param lMat The L matrix
 * \param uMat The U matrix
 * \return Returns nothing
 */
void RbMath::computeLandU(MatrixReal& aMat, MatrixReal& lMat, MatrixReal& uMat) {

	int n = aMat.getNumRows();
	for (int j=0; j<n; j++) 
		{
		for (int k=0; k<j; k++)
			for (int i=k+1; i<j; i++)
				aMat[i][j] = aMat[i][j] - aMat[i][k] * aMat[k][j];

		for (int k=0; k<j; k++)
			for (int i=j; i<n; i++)
				aMat[i][j] = aMat[i][j] - aMat[i][k] * aMat[k][j];

		for (int m=j+1; m<n; m++)
	  		aMat[m][j] /= aMat[j][j]; 
		}

	for (int row=0; row<n; row++)
		{
		for (int col=0; col<n; col++) 
			{
			if ( row <= col ) 
				{
				uMat[row][col] = aMat[row][col];
				lMat[row][col] = (row == col ? 1.0 : 0.0);
				}
			else 
				{
				lMat[row][col] = aMat[row][col];
				uMat[row][col] = 0.0;
				}
			}
		}
}

/*!
 * This function approximates the matrix exponential, f = e^a, using
 * the Pade method, which has the advantage of error control. The error
 * is controlled by setting qValue appropriately (using the function SetQValue).
 *
 * \brief Pade approximation of Matrix exponential
 * \param a [in] Input matrix
 * \param f [out] Output matrix, e^a
 * \return Returns nothing
 * \see
 * Golub, G. H., and C. F. Van Loan. 1996. Matrix Computations, Third Edition.
 *    The Johns Hopkins University Press, Baltimore, Maryland. [algorithm 11.3.1]
 * \todo See if ldexp is faster than regular matrix division by scalar
 */
int RbMath::expMatrixPade(MatrixReal& A, MatrixReal& F, int qValue) {

	int dim = A.getNumRows();
	if (dim != A.getNumCols())
		return (1);
	
	// create identity matrices
	MatrixReal D(dim, dim, 0.0);
	MatrixReal N(dim, dim, 0.0);
	MatrixReal X(dim, dim, 0.0);
	for (int i=0; i<dim; i++)
        {
		D[i][i] = 1.0;
        N[i][i] = 1.0;
        X[i][i] = 1.0;
        }

	// create uninitialized matrix
	MatrixReal cX(dim, dim, 0.0);
	
	// We assume that we have a rate matrix where rows sum to zero
	// Then the infinity-norm is twice the maximum absolute value
	// of the diagonal cells.
	double normA = 0.0;
	for (int i=0; i<dim; i++) 
        {
		double x = fabs (A[i][i]);
		if (x > normA)
			normA = x;
        }
	normA *= 2.0;

	// Calculate 1 + floor (log2(normA))
	int y;	
	frexp(normA, &y);	// this will give us the floor(log2(normA)) part in y
	y++;

	// Get max(0,y)
	int j = 0;
	if (y > 0)
		j = y;

	// divide A by scalar 2^j
	//A /= ldexp(1.0, j);
    double myFactor = 1.0 / ldexp(1.0, j);
    A *= myFactor;
	
	double c = 1.0;
	for (int k=1; k<=qValue; k++) 
        {
		c = c * (qValue - k + 1.0) / ((2.0 * qValue - k + 1.0) * k);

		/* X = AX */
        X = A * X;

		/* N = N + cX */
		cX = c * X;
		N = N + cX;

		/* D = D + (-1)^k*cX */
		if (k % 2 == 0)
			D = D + cX;
		else
			D = D - cX;
		}

	RbMath::gaussianElimination(D, N, F);

	for (int k=0; k<j; k++)
		F = F * F;
	
	for (int i=0; i<dim; i++)
		{
		for (j=0; j<dim; j++)
			{
			if (F[i][j] < 0.0)
				F[i][j] = fabs(F[i][j]);
			}
		}
	return (0);
}

/*!
 * This function returns the factorial of x, x!
 *
 * \brief Return x!
 * \param x The x value
 * \return The factorial x!
 */
double RbMath::factorial(int x) {
	
	double fac = 1.0;
	for (int i=1; i<=x; i++)
		fac *= i;
	return (fac);
}

/*!
 * This function calculates the p and q values needed to control the error of the
 * Pade approximation for calculating the matrix exponential, P = e^{Q * v}. 
 * The error, e(p,q) is:
 *
 *    e(p,q) = 2^(3-(p+q)) * ((p!*q!) / (p+q)! * (p+q+1)!)
 *
 * Setting p = q will minimize the error for a given amount of work. This function 
 * assumes that p = q. The function takes in as a parameter the desired tolerance
 * for the accuracy of the matrix exponentiation, and returns qV = p = q, that
 * will achieve the tolerance.
 * 
 * \brief Calculate p=q needed to control error of Pade approximation
 * \param tolerance The desired tolerance
 * \return The int value giving the desired tolerance
 * \see
 * Golub, G. H., and C. F. Van Loan. 1996. Matrix Computations, Third Edition.
 *    The Johns Hopkins University Press, Baltimore, Maryland.
 */
int RbMath::findPadeQValue(const double& tolerance) {

	// Here we want to calculate
	// double x = pow(2.0, 3.0 - (0 + 0)) * MbMath::factorial(0) * MbMath::factorial(0) / (MbMath::factorial(0+0) * MbMath::factorial(0+0+1));
	// that is, the expression below for qV = 0. However, we can simplify that to
	double x = 8.0;
	int qV = 0;
	while (x > tolerance) {
		qV++;
		x = pow(2.0, 3.0 - (qV + qV)) * RbMath::factorial(qV) * RbMath::factorial(qV) / (RbMath::factorial(qV+qV) * RbMath::factorial(qV+qV+1));
	}
	return (qV);
}

/*!
 * Forward substitution of Gaussian elimination
 *
 * \brief Forward substitution
 * \param L [in/out] Matrix for forward substitution
 * \param b [in/out] Solution vector
 * \return Returns nothing
 */
void RbMath::forwardSubstitutionRow(MatrixReal& L, std::vector<double>& b) {

	int n = L.getNumRows();
	b[0] = b[0] / L[0][0];
	for (int i=1; i<n; i++) 
		{
		double dotProduct = 0.0;
		for (int j=0; j<i; j++)
            {
	      	dotProduct += L[i][j] * b[j];
            }
		b[i] = (b[i] - dotProduct) / L[i][i];
		}
}
	
/*!
 * This function calculates the gamma function for real x.
 *
 * \brief Gamma function.
 * \param x is the argument. 
 * \return Returns the value of the gamma function. 
 * \throws Does not throw an error.
 */
double RbMath::gamma(double x) {

	double c[7] = { -1.910444077728E-03, 
	                8.4171387781295E-04, 
	                -5.952379913043012E-04, 
	                7.93650793500350248E-04, 
	                -2.777777777777681622553E-03, 
	                8.333333333333333331554247E-02, 
	                5.7083835261E-03 };
	double fact;
	int i;
	int n;
	double p[8] = { -1.71618513886549492533811, 
	                2.47656508055759199108314E+01, 
	                -3.79804256470945635097577E+02, 
	                6.29331155312818442661052E+02, 
	                8.66966202790413211295064E+02, 
	                -3.14512729688483675254357E+04, 
	                -3.61444134186911729807069E+04, 
	                6.64561438202405440627855E+04 };
	double q[8] = { -3.08402300119738975254353E+01, 
	                3.15350626979604161529144E+02,
	                -1.01515636749021914166146E+03,
	                -3.10777167157231109440444E+03, 
	                2.25381184209801510330112E+04, 
	                4.75584627752788110767815E+03, 
	                -1.34659959864969306392456E+05, 
	                -1.15132259675553483497211E+05 };
	double sqrtpi = 0.9189385332046727417803297;
	double sum2;
	double value;
	double xbig = 35.040;
	double xden;
	double xminin = 1.18E-38;
	double xnum;
	double y;
	double y1;
	double ysq;
	double z;

	bool parity = false;
	fact = 1.0;
	n = 0;
	y = x;

	if ( y <= 0.0 )
		{
		/* argument negative */
		y = -x;
		y1 = ( double ) ( ( int ) ( y ) );
		value = y - y1;

		if ( value != 0.0 )
			{
			if ( y1 != ( double ) ( ( int ) ( y1 * 0.5 ) ) * 2.0 )
				parity = true;
			fact = -PI / std::sin(PI * value);
			y = y + 1.0;
			}
		else
			{
			//value = d_huge ( );
			value = HUGE_VAL;
			return value;
			}
		}
	if ( y < rbEpsilon() )
		{
		/* argument < EPS */
		if ( xminin <= y )
			{
			value = 1.0 / y;
			}
		else
			{
			//value = d_huge ( );
			value = HUGE_VAL;
			return value;
			}
		}
	else if ( y < 12.0 )
		{
		y1 = y;
		/* 0.0 < argument < 1.0 */
		if ( y < 1.0 )
			{
			z = y;
			y = y + 1.0;
			}
		/* 1.0 < argument < 12.0, reduce argument if necessary */
		else
			{
			n = int ( y ) - 1;
			y = y - ( double ) ( n );
			z = y - 1.0;
			}
		/* evaluate approximation for 1.0 < argument < 2.0 */
		xnum = 0.0;
		xden = 1.0;
		for ( i = 0; i < 8; i++ )
			{
			xnum = ( xnum + p[i] ) * z;
			xden = xden * z + q[i];
			}

		value = xnum / xden + 1.0;
		/* adjust result for case  0.0 < argument < 1.0 */
		if ( y1 < y )
			{
			value = value / y1;
			}
		/* adjust result for case  2.0 < argument < 12.0 */
		else if ( y < y1 )
			{
			for ( i = 1; i <= n; i++ )
				{
				value = value * y;
				y = y + 1.0;
				}
			}
		}
	else
		{
		/* evaluate for 12 <= argument */
		if ( y <= xbig )
			{
			ysq = y * y;
			sum2 = c[6];
			for ( i = 0; i < 6; i++ )
				{
				sum2 = sum2 / ysq + c[i];
				}
			sum2 = sum2 / y - y + sqrtpi;
			sum2 = sum2 + ( y - 0.5 ) * log ( y );
			value = exp ( sum2 );
			}
		else
			{
			//value = d_huge ( );
			value = HUGE_VAL;
			return value;
			}

		}
	/* final adjustments and return */
	if ( parity )
		{
		value = -value;
		}
	if ( fact != 1.0 )
		{
		value = fact / value;
		}

	return value;
}

/*!
 * Gaussian elimination
 *
 * \brief Gaussian elimination
 * \param a ??
 * \param bMat ??
 * \param xMat ??
 * \return Returns nothing
 */
void RbMath::gaussianElimination(MatrixReal& a, MatrixReal& bMat, MatrixReal& xMat) {

	int n = a.getNumRows();
	MatrixReal lMat(n, n, 0.0);
	MatrixReal uMat(n, n, 0.0);
	std::vector<double> bVec(n);
	RbMath::computeLandU(a, lMat, uMat);

	for (int k=0; k<n; k++) 
		{
		for (int i=0; i<n; i++)
			bVec[i] = bMat[i][k];

		/* Answer of Ly = b (which is solving for y) is copied into b. */
		forwardSubstitutionRow(lMat, bVec);

		/* Answer of Ux = y (solving for x and the y was copied into b above) 
			is also copied into b. */
		backSubstitutionRow(uMat, bVec);
		for (int i=0; i<n; i++)
			xMat[i][k] = bVec[i];
		}
}

/*!
 * This function returns the hypotenuse of a
 * triangle with the legs being a and b
 *
 * \brief Return hypotenuse
 * \param a First leg
 * \param b Second leg
 * \return Hypotenuse
 */
double RbMath::hypotenuse(double a, double b) {
	
	double r;
	if ( fabs(a) > fabs(b) ) 
		{
		r = b / a;
		r = fabs(a) * sqrt(1+r*r);
		} 
	else if ( b != 0.0 ) 
		{
		r = a / b;
		r = fabs(b) * sqrt(1+r*r);
		} 
	else 
		{
		r = 0.0;
		}
	return r;
}

/*!
 * This function returns the incomplete beta function, which is
 *
 * BI(a,b,x) = Integral(0 <= T <= X) T**(A-1) (1-T)**(B-1) dt /
 *             Integral(0 <= T <= 1) T**(A-1) (1-T)**(B-1) dt
 *
 * \brief Incomplete beta function.
 * \param a is a beta parameter. 
 * \param b is a beta parameter. 
 * \param x is the upper limit of integration. 
 * \return Returns the incomplete beta function. 
 * \throws Does not throw an error.
 * \see Majumder & Bhattacharjee. 1973. Algorithm AS63. Applied 
 *      Statistics, 22.
 */
double RbMath::incompleteBeta(double a, double b, double x) {

	double tol = 1.0E-07;

	double value;
	if ( x <= 0.0 )
		{
		value = 0.0;
		return value;
		}
	else if ( 1.0 <= x )
		{
		value = 1.0;
		return value;
		}

	/* change tail if necessary and determine S */
	double psq = a + b;

	double xx, cx, pp, qq;
	bool indx;
	if ( a < (a + b) * x )
		{
		xx = 1.0 - x;
		cx = x;
		pp = b;
		qq = a;
		indx = true;
		}
	else
		{
		xx = x;
		cx = 1.0 - x;
		pp = a;
		qq = b;
		indx = false;
		}

	double term = 1.0;
	int i = 1;
	value = 1.0;
	int ns = (int)(qq + cx * (a + b));

	/* use Soper's reduction formulas */
	double rx = xx / cx;

	double temp = qq - (double)i;
	if ( ns == 0 )
		rx = xx;

	int it = 0;
	int it_max = 1000;
	for (;;)
		{
		it++;
		if ( it_max < it )
			{
			std::cerr << "Error in incompleteBeta: Maximum number of iterations exceeded!" << std::endl;
			exit(1);
			}
		term = term * temp * rx / ( pp + ( double ) ( i ) );
		value = value + term;
		temp = fabs(term);
		if ( temp <= tol && temp <= tol * value )
			break;
		i++;
		ns--;
		if ( 0 <= ns )
			{
			temp = qq - (double)i;
			if ( ns == 0 )
				rx = xx;
			}
		else
			{
			temp = psq;
			psq = psq + 1.0;
			}
		}
		
	/* finish calculation */
	value = value * exp(pp * log(xx) + (qq - 1.0) * log(cx)) / (beta(a, b) * pp);
	if ( indx )
		value = 1.0 - value;
	return value;
}

/*!
 * This function returns the incomplete gamma ratio I(x,alpha) where x is
 * the upper limit of the integration and alpha is the shape parameter.
 *
 * \brief Incomplete gamma function.
 * \param alpha is the shape parameter of the gamma. 
 * \param x is the upper limit of integration. 
 * \return Returns -1 if in error and the incomplete gamma ratio otherwise. 
 * \throws Does not throw an error.
 * \see Bhattacharjee, G. P. 1970. The incomplete gamma integral. Applied 
 *      Statistics, 19:285-287.
 */
double RbMath::incompleteGamma(double x, double alpha, double scale) {

	double			p = alpha, g = scale,
					accurate = 1e-8, overflow = 1e30,
					rn = 0.0, a = 0.0, b = 0.0, an = 0.0, 
					gin, dif = 0.0, term = 0.0, pn[6];

	if (x == 0.0) 
		return (0.0);
	if (x < 0 || p <= 0) 
		return (-1.0);

	double factor = exp(p*log(x)-x-g);   
	if (x > 1 && x >= p) 
		goto l30;
	gin = 1.0;  
	term = 1.0;  
	rn = p;
	l20:
		rn++;
		term *= x/rn;   
		gin += term;
		if (term > accurate) 
			goto l20;
		gin *= factor/p;
		goto l50;
	l30:
		a = 1.0-p;   
		b = a+x+1.0;  
		term = 0.0;
		pn[0] = 1.0;  
		pn[1] = x;  
		pn[2] = x+1;  
		pn[3] = x*b;
		gin = pn[2]/pn[3];
	l32:
		a++;  
		b += 2.0;  
		term++;   
		an = a*term;
		for (int i=0; i<2; i++) 
			pn[i+4] = b*pn[i+2]-an*pn[i];
		if (pn[5] == 0) 
			goto l35;
		rn = pn[4]/pn[5];   
		dif = fabs(gin-rn);
		if (dif>accurate) 
			goto l34;
		if (dif<=accurate*rn) 
			goto l42;
	l34:
		gin = rn;
	l35:
		for (int i=0; i<4; i++) 
			pn[i] = pn[i+2];
		if (fabs(pn[4]) < overflow) 
			goto l32;
		for (int i=0; i<4; i++) 
			pn[i] /= overflow;
		goto l32;
	l42:
		gin = 1.0-factor*gin;
	l50:
		return (gin);
}

/* log factorial ln(n!) */
/*!
 * This function calculates the natural log of the factorial of n using
 * the Stirling approximation. 
 *
 * \brief Natural log of the factorial.
 * \param n is the number for the factorial (n!). 
 * \return Returns the natural log of the factorial of n. 
 * \throws Does not throw an error.
 */
double RbMath::lnFactorial(int n) {

	double n1 = n;  
	double r  = 1.0 / n1;
	double C0 =  0.918938533204672722;
	double C1 =  1.0/12.0;
	double C3 = -1.0/360.0;
	return (n1 + 0.5) * log(n1) - n1 + C0 + r*(C1 + r*r*C3);
}

/*!
 * This function calculates the log of the gamma function, which is equal to:
 * Gamma(alp) = {integral from 0 to infinity} t^{alp-1} e^-t dt
 * The result is accurate to 10 decimal places. Stirling's formula is used
 * for the central polynomial part of the procedure.
 *
 * \brief Natural log of the gamma function.
 * \param alp is the parameter of the gamma function. 
 * \return Returns the log of the gamma function. 
 * \throws Does not throw an error.
 * \see Pike, M. C. and I. D. Hill. 1966. Algorithm 291: Logarithm of the gamma
 *      function. Communications of the Association for Computing Machinery, 9:684.
 */
double RbMath::lnGamma(double a) {

	double x = a;
	double f = 0.0;
	double z;
	if (x < 7) 
		{
		f = 1.0;  
		z = x - 1.0;
		while (++z < 7.0)  
			f *= z;
		x = z;   
		f = -log(f);
		}
	z = 1.0 / (x*x);
	return  (f + (x-0.5)*log(x) - x + 0.918938533204673 + 
			(((-0.000595238095238*z+0.000793650793651)*z-0.002777777777778)*z +
			0.083333333333333)/x);  
}

void RbMath::matrixInverse(MatrixReal& a, MatrixReal& aInv) {

    // get dimensions: we assume a square matrix
	int n = a.getNumRows();
    
    // copy original matrix, a, into a working version, aTmp
    MatrixReal aTmp(a);
    
    // set up some matrices for work
	MatrixReal lMat(n, n, 0.0);
	MatrixReal uMat(n, n, 0.0);
    MatrixReal identity(n, n, 0.0);
    for (int i=0; i<n; i++)
        identity[i][i] = 1.0;
	std::vector<double> bVec(n);
    
    // compute the matrix inverse
	RbMath::computeLandU(aTmp, lMat, uMat);
	for (int k=0; k<n; k++) 
		{
		for (int i=0; i<n; i++)
			bVec[i] = identity[i][k];

		/* Answer of Ly = b (which is solving for y) is copied into b. */
		forwardSubstitutionRow(lMat, bVec);

		/* Answer of Ux = y (solving for x and the y was copied into b above) 
			is also copied into b. */
		backSubstitutionRow(uMat, bVec);
		for (int i=0; i<n; i++)
			aInv[i][k] = bVec[i];
		}
}

/*!
 * This function normalizes a vector such that its sum is some value.
 *
 * \brief Vector normalization function.
 * \param x is a reference to the vector to be normalized .
 * \param sum is the desired sum for the normalized vector.
 * \return Does not return a value. 
 * \throws Does not throw an error.
 */
void RbMath::normalize(std::vector<double>& x, double sum) {

    double s = 0.0;
    for (size_t i=0; i<x.size(); i++)
        s += x[i];
    double factor = sum / s;
    for (size_t i=0; i<x.size(); i++)
        x[i] *= factor;
}

/*!
 * This function returns the round off unit for floating point arithmetic.
 * The returned value is a number, r, which is a power of 2 with the property
 * that, to the precision of the computer's arithmetic, 1 < 1 + r, but
 * 1 = 1 + r / 2. This function comes from John Burkardt.
 *
 * \brief Round off unity for floating point arithmetic.
 * \return Returns round off unity for floating point arithmetic. 
 * \throws Does not throw an error.
 */
double RbMath::rbEpsilon(void) {

	double r = 1.0;
	while ( 1.0 < (double)(1.0 + r)  )
		r = r / 2.0;
	return 2.0 * r;
}

/*!
 * Transpose the matrix a. The matrix a should be m X n whereas the
 * matrix t should be n X m. If not, we return 1. On success, 0 is
 * returned.
 *
 * \brief Transpose a matrix
 * \param a [in] Matrix to transpose
 * \param t [out] Transposed matrix
 * \return 0 on success, 1 on failure
 */
int RbMath::transposeMatrix(const MatrixReal& a, MatrixReal& t) {
	
	int m = a.getNumRows();
	int n = a.getNumCols();
	
	if ( m != t.getNumCols() || n != t.getNumRows() )
		return (1);

	for (int i=0; i<m; i++)
		for (int j=0; j<n; j++)
			t[j][i] = a[i][j];
	return (0);
}


