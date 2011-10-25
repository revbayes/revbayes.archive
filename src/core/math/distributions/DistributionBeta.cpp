/**
 * @file DistributionBeta
 * This file contains the functions of the beta distribution.
 *
 * @brief Implementation of the beta distribution.
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-03-17, version 1.0
 *
 * $Id$
 */

#define MAXK 20

#include <cmath>

#include "DistributionBeta.h"
#include "DistributionGamma.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbMathFunctions.h"
#include "RbMathLogic.h"

/*!
 * This function returns the probability density for a
 * Beta-distributed random variable.
 *
 * \brief Beta probability density.
 * \param a parameter of the Beta.
 * \param b parameter of the Beta.
 * \return Returns the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::Beta::pdf(double a, double b, double x) {

	double pdf;
	if ( x < 0.0 || 1.0 < x )
		pdf = 0.0;
	else
		pdf = std::pow(x, (a - 1.0)) * std::pow((1.0 - x), (b - 1.0)) / RbMath::beta(a, b);
	return pdf;
}

/*!
 * This function returns the natural log of the probability density
 * for a Beta-distributed random variable.
 *
 * \brief Beta log probability density.
 * \param a parameter of the Beta.
 * \param b parameter of the Beta.
 * \return Returns the natural log of the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::Beta::lnPdf(double a, double b, double x) {

	return ( (RbMath::lnGamma(a + b) - RbMath::lnGamma(a) - RbMath::lnGamma(b)) + (a - 1.0) * std::log(x) + (b - 1.0) * std::log(1.0 - x) );
}

/*!
 * This function returns the cumulative probability for a
 * Beta-distributed random variable.
 *
 * \brief Beta cumulative probability.
 * \param a parameter of the Beta.
 * \param b parameter of the Beta.
 * \return Returns the cumulative probability.
 * \throws Does not throw an error.
 */
double RbStatistics::Beta::cdf(double a, double b, double x) {

	double cdf;
	if ( x <= 0.0 )
		cdf = 0.0;
	else if ( x <= 1.0 )
		cdf = RbMath::incompleteBeta(a, b, x);
	else
		cdf = 1.0;
	return cdf;
}

/*!
 * This function returns the quantile for a
 * Beta-distributed random variable.
 *
 * \brief Beta quantile.
 * \param a parameter of the Beta.
 * \param b parameter of the Beta.
 * \param p is the probability up to the quantile.
 * \return Returns the quantile.
 * \throws Does not throw an error.
 */
double RbStatistics::Beta::quantile(double a, double b, double p) {

	double error = 0.0001;
	double errapp = 0.01;

	/* estimate the solution */
	double x = a / ( a + b );

	double xOld = 0.0;
	int loopCnt = 2;
	double d[MAXK * (MAXK-1)];
	while ( errapp <= fabs ( ( x - xOld ) / x ) && loopCnt != 0 )
        {
		xOld = x;
		loopCnt--;
		double cdfX = Beta::cdf(a, b, x);
		double pdfX = Beta::pdf(a, b, x);
		double q    = (p - cdfX) / pdfX;
		double t    = 1.0 - x;
		double s1   = q * ( b - 1.0 ) / t;
		double s2   = q * ( 1.0 - a ) / x;
		d[2-1+0*MAXK] = s1 + s2;
		double tail = d[2-1+0*MAXK] * q / 2.0;
		x = x + q + tail;

		int k = 3;
		while ( error < fabs ( tail / x ) && k <= MAXK )
            {
			/* find D(2,K-2) */
			s1 = q * ((double)(k) - 2.0) * s1 / t;
			s2 = q * (2.0 - (double)(k)) * s2 / x;
			d[2-1+(k-2)*MAXK] = s1 + s2;

			/* find D(3,K-3), D(4,K-4), D(5,K-5), ... , D(K-1,1) */
			for (int i=3; i<=k-1; i++)
                {
				double sum2 = d[2-1+0*MAXK] * d[i-2+(k-i)*MAXK];
				double bcoeff = 1.0;
				for (int j=1; j<=k - i; j++)
                    {
					bcoeff = ( bcoeff * ( double ) ( k - i - j + 1 ) ) / ( double ) ( j );
					sum2 = sum2 + bcoeff * d[2-1+j*MAXK] * d[i-2+(k-i-j)*MAXK];
                    }
				d[i-1+(k-i)*MAXK] = sum2 + d[i-2+(k-i+1)*MAXK] / (double)(i - 1);
                }

			/* compute D(K,0) and use it to expand the series */
			d[k-1+0*MAXK] = d[2-1+0*MAXK] * d[k-2+0*MAXK] + d[k-2+1*MAXK] / (double)(k - 1);
			tail = d[k-1+0*MAXK] * q / (double)(k);
			x += tail;
			/* check for divergence */
			if ( x <= 0.0 || 1.0 <= x )
                {
                std::ostringstream s;
                s << "Error in betaQuantile: The series has diverged";
                throw (RbException(s));
                }
			k++;
            }
        }
	return x;
}


/* Reference:
 * R. C. H. Cheng (1978).
 * Generating beta variates with nonintegral shape parameters.
 * Communications of the ACM 21, 317-322.
 * (Algorithms BB and BC)
 */
#define expmax	(DBL_MAX_EXP * RbConstants::LN2)/* = log(DBL_MAX) */

double RbStatistics::Beta::rv(double aa, double bb, RbPtr<RandomNumberGenerator> rng) {
    double a, b, alpha;
    double r, s, t, u1, u2, v, w, y, z;

    int qsame;
    /* FIXME:  Keep Globals (properly) for threading */
    /* Uses these GLOBALS to save time when many rv's are generated : */
    static double beta, gamma, delta, k1, k2;
    static double olda = -1.0;
    static double oldb = -1.0;

    if (aa <= 0. || bb <= 0. || (!RbMath::isFinite(aa) && !RbMath::isFinite(bb)))
        {
        std::ostringstream s;
        s << "Cannot draw random variable from beta distribution for a = " << aa << " and b = " << bb;
        throw (RbException(s));
        }

    if (!RbMath::isFinite(aa))
    	return 1.0;

    if (!RbMath::isFinite(bb))
    	return 0.0;

    /* Test if we need new "initializing" */
    qsame = (olda == aa) && (oldb == bb);
    if (!qsame) { olda = aa; oldb = bb; }

    a = RbMath::min(aa, bb);
    b = RbMath::max(aa, bb); /* a <= b */
    alpha = a + b;

#define v_w_from__u1_bet(AA) 			                \
v = beta * log(u1 / (1.0 - u1));	                    \
if (v <= expmax) {		                                \
w = AA * exp(v);		                                \
if(!RbMath::isFinite(w)) w = RbConstants::Double::max;	\
} else				                                    \
w = RbConstants::Double::max

    if (a <= 1.0)
        {
        /* --- Algorithm BC --- */

        /* changed notation, now also a <= b (was reversed) */
        if (!qsame)
            {
            /* initialize */
            beta = 1.0 / a;
            delta = 1.0 + b - a;
            k1 = delta * (0.0138889 + 0.0416667 * a) / (b * beta - 0.777778);
            k2 = 0.25 + (0.5 + 0.25 / delta) * a;
            }
        /* FIXME: "do { } while()", but not trivially because of "continue"s:*/
        for(;;)
            {
            u1 = rng->uniform01();
            u2 = rng->uniform01();
            if (u1 < 0.5)
                {
                y = u1 * u2;
                z = u1 * y;
                if (0.25 * u2 + z - y >= k1)
                    continue;
                }
            else
                {
                z = u1 * u1 * u2;
                if (z <= 0.25)
                    {
                    v_w_from__u1_bet(b);
                    break;
                    }
                if (z >= k2)
                    continue;
                }
            v_w_from__u1_bet(b);
            if (alpha * (log(alpha / (a + w)) + v) - 1.3862944 >= log(z))
                break;
            }
        return (aa == a) ? a / (a + w) : w / (a + w);
        }
    else
        {
        /* Algorithm BB */

        if (!qsame)
            {
            /* initialize */
            beta = sqrt((alpha - 2.0) / (2.0 * a * b - alpha));
            gamma = a + 1.0 / beta;
            }
        do {
            u1 = rng->uniform01();
            u2 = rng->uniform01();

            v_w_from__u1_bet(a);

            z = u1 * u1 * u2;
            r = gamma * v - 1.3862944;
            s = a + r - w;
            if (s + 2.609438 >= 5.0 * z)
                break;
            t = log(z);
            if (s > t)
                break;
            } while (r + alpha * log(alpha / (b + w)) < t);
        return (aa != a) ? b / (b + w) : w / (b + w);
        }
}

#undef MAXK
