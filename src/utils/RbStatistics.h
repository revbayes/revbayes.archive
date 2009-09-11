/*!
* \file
* This file contains commonly used statistics functions that are used
* in REvBayes. The probability density (pdf), log of the probability
* density (lnPdf), cumulative probability (cdf), and quantiles of
* common probability distributions.
*
* \brief Namespace containing statistical functions
*
* (c) Copyright 2009- under GPL version 3
* \date Last modified: $Date$
 * \author Sebastian Hoehna (Department of Mathematics, Stockholm University)
 * \author John Huelsenbeck (Department of Integrative Biology, University of California, Berkeley)
 * \author Fredrik Ronquist (Department of Entomology, Swedish Museum of Natural History)
* \license GPL version 3
*
* $Id$
*/
#ifndef RbStatistics_H
#define RbStatistics_H

#include <vector>

namespace RbStatistics {

#pragma mark Beta Distribution

	namespace Beta {
	
	double        pdf(double a, double b, double x);                                  /*!< Beta(a,b) probability density */
	double        lnPdf(double a, double b, double x);                                /*!< Beta(a,b) log_e probability density */
	double        cdf(double a, double b, double x);                                  /*!< Beta(a,b) cumulative probability */
	double        quantile(double a, double b, double p);                             /*!< Beta(a,b) quantile */
	}
	
#pragma mark Chi-Square Distribution

	namespace ChiSquare {
	double        pdf(double v, double x);                                            /*!< Chi-Square(v) probability density */
	double        lnPdf(double v, double x);                                          /*!< Chi-Square(v) log_e probability density */
	double        cdf(double v, double x);                                            /*!< Chi-Square(v) cumulative probability */
	double        quantile(double v, double p);                                       /*!< Chi-Square(v) quantile */
	}

#pragma mark Dirichlet Distribution

	namespace Dirichlet {
	double        pdf(const std::vector<double>& a, const std::vector<double>& z);    /*!< Dirichlet(a[]) probability density */
	double        lnPdf(const std::vector<double>& a, const std::vector<double>& z);  /*!< Dirichlet(a[]) log_e probability density */
	}

#pragma mark Exponential Distribution

	namespace Exponential {
	inline double pdf(double lambda, double x);                                       /*!< Exponential(lambda) probability density */
	inline double lnPdf(double lambda, double x);                                     /*!< Exponential(lambda) log_e probability density */
	inline double cdf(double lambda, double x);                                       /*!< Exponential(lambda) cumulative probability */
	inline double quantile(double lambda, double p);                                  /*!< Exponential(lambda) quantile */
	}

#pragma mark Gamma Distribution

	namespace Gamma {
	double        pdf(double a, double b, double x);                                  /*!< Gamma(a,b) probability density */
	double        lnPdf(double a, double b, double x);                                /*!< Log of the Gamma(a,b) probability density */  
	double        cdf(double a, double b, double x);                                  /*!< Gamma(a,b) cumulative probability */
	inline double quantile(double a, double b, double p);                             /*!< Gamma(a,b) quantile */
	}

#pragma mark LogNormal Distribution

	namespace LogNormal {
	inline double pdf(double mu, double sigma, double x);                             /*!< LogNormal(mu,sigma) probability density */
	inline double lnPdf(double mu, double sigma, double x);                           /*!< Log of the LogNormal(mu,sigma) probability density */
	inline double cdf(double mu, double sigma, double x);                             /*!< LogNormal(mu,sigma) cumulative probability */
	double        quantile(double mu, double sigma, double p);                        /*!< LogNormal(mu,sigma) quantile */
	}

#pragma mark Poisson Distribution

	namespace Poisson {
	inline double pdf(double lambda, int x);                                          /*!< Poisson(lambda) probability */
	inline double lnPdf(double lambda, int x);                                        /*!< Log of the Poisson(lambda) probability */
	double        cdf(double lambda, int x);                                          /*!< Poisson(lambda) cumulative probability */
	inline double quantile(double lambda, double p);                                  /*!< Poisson(lambda) quantile */
	}

#pragma mark Normal Distribution

	namespace Normal {
	double pdf(double mu, double sigma, double x);                                    /*!< Normal(mu,sigma) probability density */
	double lnPdf(double mu, double sigma, double x);                                  /*!< Log of the Normal(mu,sigma) probability density */
	double        cdf(double mu, double sigma, double x);                             /*!< Normal(mu,sigma) cumulative probability */
	double        quantile(double p);                                                 /*!< Normal(0,1) quantile */
	double        quantile(double mu, double sigma, double p);                        /*!< Normal(mu,sigma) quantile */
	}

#pragma mark Uniform(0,1) Distribution

	namespace Uniform01 {
	inline double pdf(void);                                                          /*!< Uniform(0,1) probability density */
	inline double lnPdf(void);                                                        /*!< Log of the Uniform(0,1) probability density */
	double        cdf(double x);                                                      /*!< Uniform(0,1) cumulative probability */
	inline double quantile(double p);                                                 /*!< Uniform(0,1) quantile */
	}
	
	namespace Uniform {
	inline double pdf(double a, double b);                                            /*!< Uniform(a,b) probability density */
	inline double lnPdf(double a, double b);                                          /*!< Log of the Uniform(a,b) probability density */
	inline double cdf(double a, double b, double x);                                  /*!< Uniform(a,b) cumulative probability */
	inline double quantile(double a, double b, double p);                             /*!< Uniform(a,b) quantile */
	}
}

#endif
