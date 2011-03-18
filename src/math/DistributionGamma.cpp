/**
 * @file DistributionGamma
 * This file contains the functions of the gamma distribution.
 *
 * @brief Implementation of the gamma distribution.
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

#include <cmath>

#include "DistributionChisq.h"
#include "DistributionGamma.h"
#include "DistributionPoisson.h"
#include "RbConstants.h"
#include "RbMathFunctions.h"
#include "RbMathLogic.h"

/*!
 * This function calculates the probability density 
 * for a gamma-distributed random variable.
 *
 * \brief Gamma probability density.
 * \param a is the shape parameter of the gamma. 
 * \param b is the scale parameter of the gamma. 
 * \param x is the gamma random variable. 
 * \return Returns the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::Gamma::pdf(double a, double b, double x) {
    
//	return (pow(b, a) / RbMath::gamma(a)) * pow(x, a - 1.0) * exp(-x * b);
    return pdf(a, b, x, false)
}

/*!
 * This function calculates the natural log of the probability density 
 * for a gamma-distributed random variable.
 *
 * \brief Natural log of gamma probability density.
 * \param a is the shape parameter of the gamma. 
 * \param b is the scale parameter of the gamma. 
 * \param x is the gamma random variable. 
 * \return Returns the natural log of the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::Gamma::lnPdf(double a, double b, double x) {
    
//	return a * log(b) - RbMath::lnGamma(a) + (a - 1.0) * log(x) - x * b;
    return pdf(a, b, x, true)
}

/*!
 * This function calculates probability density 
 * for a gamma-distributed random variable.
 *
 * The code is adapted from R v2.12.2

 * DESCRIPTION
 *
 *   Computes the density of the gamma distribution,
 *
 *                   1/s (x/s)^{a-1} exp(-x/s)
 *        p(x;a,s) = -----------------------
 *                            (a-1)!
 *
 *   where `s' is the scale (= 1/lambda in other parametrizations)
 *     and `a' is the shape parameter ( = alpha in other contexts).
 *
 * 
 *
 * \brief Gamma probability density.
 * \param a is the shape parameter of the gamma. 
 * \param b is the scale parameter of the gamma. 
 * \param x is the gamma random variable. 
 * \param isLog if the pdf is given in log scale (natural log). 
 * \return Returns the natural log of the probability density.
 * \throws Does not throw an error.
 */

double RbStatistics::Gamma::pdf(double shape, double scale, double x, bool isLog)
{
    double pr;
    if (shape < 0 || scale <= 0) {
        std::ostringstream s;
        s << "Cannot compute the pdf for the gamma distribution for shape = " << shape << " and scale = " << scale;
        throw (RbException(s));
    }
    if (x < 0)
        return 0.0;
    
    if (shape == 0) /* point mass at 0 */
        return (x == 0)? RbConstants::Double::inf : 0.0;

    if (x == 0) {
        if (shape < 1) return RbConstants::Double::inf;
        if (shape > 1) return 0.0;
        /* else */
        return isLog ? -log(scale) : 1 / scale;
    }

    if (shape < 1) {
        pr = RbStatistics::Poisson::pdf(shape, x/scale, give_log);
        return isLog ?  pr + log(shape/x) : pr*shape/x;
    }
    /* else  shape >= 1 */
    pr = dpois_raw(shape-1, x/scale, give_log);
    return give_log ? pr - log(scale) : pr/scale;
}

/*!
 * This function calculates the cumulative probability  
 * for a gamma-distributed random variable.
 *
 * \brief Gamma cumulative probability.
 * \param a is the shape parameter of the gamma. 
 * \param b is the scale parameter of the gamma. 
 * \param x is the gamma random variable. 
 * \return Returns the cumulative probability.
 * \throws Does not throw an error.
 */
double RbStatistics::Gamma::cdf(double a, double b, double x) {
    
//	return RbMath::incompleteGamma( b*x, a, RbMath::lnGamma(a) );

    const static double
	xbig = 1.0e+8,
	xlarge = 1.0e+37,
    
	/* normal approx. for alph > alphlimit */
	alphlimit = 1e5;/* was 1000. till R.1.8.x */
    
    double pn1, pn2, pn3, pn4, pn5, pn6, arg, a, b, c, an, osum, sum;
    long n;
    int pearson;
    
    /* check that we have valid values for x and alph */
    if(alph <= 0. || scale <= 0.) {
        std::ostringstream s;
        s << "Cannot compute the cdf for the gamma distribution for shape = " << shape << " and scale = " << scale;
        throw (RbException(s));
    }
    
    x /= scale;
    
    if (x <= 0.)
        return 0.0;
    
    if (alph > alphlimit) { /* use a normal approximation */
        pn1 = sqrt(alph) * 3. * (pow(x/alph, 1./3.) + 1. / (9. * alph) - 1.);
        return RbStatistics::Normal::cdf(pn1);
    }
    
    if (x > xbig * alph) {
        if (x > RbConstants::Double::max * alph)
	    /* if x is extremely large __compared to alph__ then return 1 */
            return 1.0;
        else { /* this only "helps" when log_p = TRUE */
            pn1 = sqrt(alph) * 3. * (pow(x/alph, 1./3.) + 1. / (9. * alph) - 1.);
            return RbStatistics::Normal::cdf(pn1);
        }
    }
    
    if (x <= 1. || x < alph) {
        
        pearson = 1;/* use pearson's series expansion. */
        
        arg = alph * log(x) - x - RbMath::lnGamma(alph + 1.);
        
        c = 1.;
        sum = 1.;
        a = alph;
        do {
            a += 1.;
            c *= x / a;
            sum += c;
        } while (c > DBL_EPSILON * sum);
    }
    else { /* x >= max( 1, alph) */
        
        pearson = 0;/* use a continued fraction expansion */
        
        arg = alph * log(x) - x - RbMathh::lnGamma(alph);
        
        a = 1. - alph;
        b = a + x + 1.;
        pn1 = 1.;
        pn2 = x;
        pn3 = x + 1.;
        pn4 = x * b;
        sum = pn3 / pn4;
        for (n = 1; ; n++) {
            a += 1.;/* =   n+1 -alph */
            b += 2.;/* = 2(n+1)-alph+x */
            an = a * n;
            pn5 = b * pn3 - an * pn1;
            pn6 = b * pn4 - an * pn2;
            if (fabs(pn6) > 0.) {
                osum = sum;
                sum = pn5 / pn6;
                if (fabs(osum - sum) <= DBL_EPSILON * fmin2(1., sum))
                    break;
            }
            pn1 = pn3;
            pn2 = pn4;
            pn3 = pn5;
            pn4 = pn6;
            if (fabs(pn5) >= xlarge) {
                /* re-scale the terms in continued fraction if they are large */
                pn1 /= xlarge;
                pn2 /= xlarge;
                pn3 /= xlarge;
                pn4 /= xlarge;
            }
        }
    }
    
    arg += log(sum);
    
    /* sum = exp(arg); and return   if(lower_tail) sum	else 1-sum : */
    return  exp(arg);
}


/*!
 * This function returns the quantile of a gamma probability 
 * distribution.
 *
 * \brief Gamma quantile.
 * \param a is the shape parameter. 
 * \param b is the scale parameter. 
 * \param p is the probability up to the quantile. 
 * \return Returns the quantile.
 * \throws Does not throw an error.
 */
double RbStatistics::Gamma::quantile(double shape, double scale, double p) {
    
//	return ChiSquare::quantile(p, 2.0 * a) / (2.0*b);

#define EPS1 1e-2
#define EPS2 5e-7/* final precision of AS 91 */
#define EPS_N 1e-15/* precision of Newton step / iterations */
#define LN_EPS -36.043653389117156 /* = log(.Machine$double.eps) iff IEEE_754 */
    
#define MAXIT 1000/* was 20 */
    
#define pMIN 1e-100   /* was 0.000002 = 2e-6 */
#define pMAX (1-1e-14)/* was (1-1e-12) and 0.999998 = 1 - 2e-6 */
    
    const static double
	i420  = 1./ 420.,
	i2520 = 1./ 2520.,
	i5040 = 1./ 5040;
    
    double p_, a, b, c, g, ch, ch0, p1;
    double p2, q, s1, s2, s3, s4, s5, s6, t, x;
    int i, max_it_Newton = 1;
    
    /* test arguments and initialise */
//    R_Q_P01_boundaries(p, 0., ML_POSINF);
    
    if (alpha < 0 || scale <= 0) ML_ERR_return_NAN;
    
    if (alpha == 0) /* all mass at 0 : */ return 0.;
    
    if (alpha < 1e-10) {
        /* Warning seems unnecessary now: */\
        max_it_Newton = 7;/* may still be increased below */
    }
    
    p_ = R_DT_qIv(p);/* lower_tail prob (in any case) */
    
    g = RbMath::lnGamma(alpha);/* log Gamma(v/2) */
    
    /*----- Phase I : Starting Approximation */
    ch = qchisq_appr(p, /* nu= 'df' =  */ 2*alpha, /* lgamma(nu/2)= */ g,
                     lower_tail, log_p, /* tol= */ EPS1);
    if(!RbMath::isFinite(ch)) {
        /* forget about all iterations! */
        max_it_Newton = 0; goto END;
    }
    if(ch < EPS2) {/* Corrected according to AS 91; MM, May 25, 1999 */
        max_it_Newton = 20;
        goto END;/* and do Newton steps */
    }
    
    /* FIXME: This (cutoff to {0, +Inf}) is far from optimal
     * -----  when log_p or !lower_tail, but NOT doing it can be even worse */
    if(p_ > pMAX || p_ < pMIN) {
        /* did return ML_POSINF or 0.;	much better: */
        max_it_Newton = 20;
        goto END;/* and do Newton steps */
    }
    
    
    /*----- Phase II: Iteration
     *	Call pgamma() [AS 239]	and calculate seven term taylor series
     */
    c = alpha-1;
    s6 = (120+c*(346+127*c)) * i5040; /* used below, is "const" */
    
    ch0 = ch;/* save initial approx. */
    for(i=1; i <= MAXIT; i++ ) {
        q = ch;
        p1 = 0.5*ch;
        p2 = p_ - pgamma_raw(p1, alpha, /*lower_tail*/TRUE, /*log_p*/FALSE);
                    
        if(errno != 0 || ch <= 0)
                    { ch = ch0; max_it_Newton = 27; goto END; }/*was  return ML_NAN;*/
        
        t = p2*exp(alpha*M_LN2+g+p1-c*log(ch));
        b = t/ch;
        a = 0.5*t - b*c;
        s1 = (210+ a*(140+a*(105+a*(84+a*(70+60*a))))) * i420;
        s2 = (420+ a*(735+a*(966+a*(1141+1278*a)))) * i2520;
        s3 = (210+ a*(462+a*(707+932*a))) * i2520;
        s4 = (252+ a*(672+1182*a) + c*(294+a*(889+1740*a))) * i5040;
        s5 = (84+2264*a + c*(1175+606*a)) * i2520;
        
        ch += t*(1+0.5*t*s1-b*c*(s1-b*(s2-b*(s3-b*(s4-b*(s5-b*s6))))));
        if(fabs(q - ch) < EPS2*ch)
            goto END;
        if(fabs(q - ch) > 0.1*ch) {/* diverging? -- also forces ch > 0 */
            if(ch < q) ch = 0.9 * q; else ch = 1.1 * q;
                }
    }
    /* no convergence in MAXIT iterations -- but we add Newton now... */\
    
END:
    /* PR# 2214 :	 From: Morten Welinder <terra@diku.dk>, Fri, 25 Oct 2002 16:50
     --------	 To: R-bugs@biostat.ku.dk     Subject: qgamma precision
     
     * With a final Newton step, double accuracy, e.g. for (p= 7e-4; nu= 0.9)
     *
     * Improved (MM): - only if rel.Err > EPS_N (= 1e-15);
     *		    - also for lower_tail = FALSE	 or log_p = TRUE
     * 		    - optionally *iterate* Newton
     */
    x = 0.5*scale*ch;
    if(max_it_Newton) {
        /* always use log scale */
        if (!log_p) {
            p = log(p);
            log_p = TRUE;
        }
        if(x == 0) {
            const double _1_p = 1. + 1e-7;
            const double _1_m = 1. - 1e-7;
            x = DBL_MIN;
            p_ = pgamma(x, alpha, scale, lower_tail, log_p);
            if(( lower_tail && p_ > p * _1_p) ||
               (!lower_tail && p_ < p * _1_m))
                return(0.);
            /* else:  continue, using x = DBL_MIN instead of  0  */
        }
        else
            p_ = pgamma(x, alpha, scale, lower_tail, log_p);
            for(i = 1; i <= max_it_Newton; i++) {
                p1 = p_ - p;
                        if(fabs(p1) < fabs(EPS_N * p))
                            break;
                /* else */
                if((g = dgamma(x, alpha, scale, log_p)) == R_D__0) {
                        break;
                }
                /* else :
                 * delta x = f(x)/f'(x);
                 * if(log_p) f(x) := log P(x) - p; f'(x) = d/dx log P(x) = P' / P
                 * ==> f(x)/f'(x) = f*P / P' = f*exp(p_) / P' (since p_ = log P(x))
                 */
                t = log_p ? p1*exp(p_ - g) : p1/g ;/* = "delta x" */
                t = lower_tail ? x - t : x + t;
                p_ = pgamma (t, alpha, scale, lower_tail, log_p);
                if (fabs(p_ - p) > fabs(p1) ||
                    (i > 1 && fabs(p_ - p) == fabs(p1)) /* <- against flip-flop */) {
                    /* no improvement */
                        break;
                } /* else : */
                        x = t;
            }
    }
    
    return x;
}

double qchisq_appr(double p, double nu, double g/* = log Gamma(nu/2) */,
                   int lower_tail, int log_p, double tol /* EPS1 */)
{
#define C7	4.67
#define C8	6.66
#define C9	6.73
#define C10	13.32
    
    double p_, alpha, a, c, ch, p1;
    double p2, q, t, x;
    
    /* test arguments and initialise */
    
    R_Q_P01_check(p);
    if (nu <= 0) ML_ERR_return_NAN;
    
    p_ = R_DT_qIv(p);/* lower_tail prob (in any case) */
    
    alpha = 0.5 * nu;/* = [pq]gamma() shape */
    c = alpha-1;
    
    if(nu < (-1.24)*(p1 = R_DT_log(p))) {	/* for small chi-squared */
        /* log(alpha) + g = log(alpha) + log(gamma(alpha)) =
         *        = log(alpha*gamma(alpha)) = lgamma(alpha+1) suffers from
         *  catastrophic cancellation when alpha << 1
         */
        double lgam1pa = (alpha < 0.5) ? lgamma1p(alpha) : (log(alpha) + g);
        ch = exp((lgam1pa + p1)/alpha + M_LN2);
        
    } else if(nu > 0.32) {	/*  using Wilson and Hilferty estimate */
        
        x = qnorm(p, 0, 1, lower_tail, log_p);
        p1 = 2./(9*nu);
        ch = nu*pow(x*sqrt(p1) + 1-p1, 3);
        
        /* approximation for p tending to 1: */
        if( ch > 2.2*nu + 6 )
            ch = -2*(R_DT_Clog(p) - c*log(0.5*ch) + g);
        
    } else { /* "small nu" : 1.24*(-log(p)) <= nu <= 0.32 */
        
        ch = 0.4;
        a = R_DT_Clog(p) + g + c*M_LN2;
        do {
            q = ch;
            p1 = 1. / (1+ch*(C7+ch));
            p2 = ch*(C9+ch*(C8+ch));
            t = -0.5 +(C7+2*ch)*p1 - (C9+ch*(C10+3*ch))/p2;
            ch -= (1- exp(a+0.5*ch)*p2*p1)/t;
        } while(fabs(q - ch) > tol * fabs(ch));
    }
    
    return ch;
}


double RbStatistics::Gamma::rv(double a, double b, RandomNumberGenerator* rng) {
    
	return (RbStatistics::Helper::rndGamma(a, *rng) / b);
}
