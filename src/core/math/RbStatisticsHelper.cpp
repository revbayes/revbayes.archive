//
//  RbStatisticsHelper.cpp
//  RevBayes
//
//  Created by Sebastian Hoehna on 6/7/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "RbStatisticsHelper.h"

#include "RbMathCombinatorialFunctions.h"

#include <cmath>
#include <iostream>

/*!
 * This function is used to calculate the concentration parameter
 * of the DPP from a given number of tables and a given number of 
 * data elements.
 *
 * \brief Subfunction for DPP.
 * \param tables is the given number of tables. 
 * \param num is the given number of data elemnts. 
 * \return Returns an approximate value of alpha. 
 * \throws 
 */
double RbStatistics::Helper::dppConcParamFromNumTables(double tables, double num){
    
    if ( tables < 1.0 )
		throw RbException( "The Dirichlet Process expects there to be 1 or more clusters: clusters > 0." );
	if ( num < 2.0)
		throw RbException( "The Dirichlet Process expects there to be more than 1 data element: number > 2." );
	if ( tables > num)
		throw RbException( "The Dirichlet Process expects there to fewer clusters than data elements: clusters < number." );
	double a = 0.000001;
	double ea = dppExpectNumTableFromConcParam(a, num);
	bool goUp;
	double target = tables;
	if (target <= 1.0) 
		target = 1.01;
	if (ea < target)
		goUp = true;
	else
		goUp = false;
	double increment = 0.1;
	while ( fabs(ea - target) > 0.000001 ) {
		if (ea < target && goUp == true) {
			a += increment;
		}
		else if (ea > target && goUp == false) {
			a -= increment;
		}
		else if (ea < target && goUp == false) {
			increment /= 2.0;
			goUp = true;
			a += increment;
		}
		else {
			increment /= 2.0;
			goUp = false;
			a -= increment;
		}
		ea = dppExpectNumTableFromConcParam(a, num);
	}
	return a;
}

/*!
 * This function is used to calculate the expected number of tables
 * of the DPP from a given value of alpha and a given number of 
 * data elements.
 *
 * \brief Subfunction for DPP.
 * \param tables is the given number of tables. 
 * \param num is the given number of data elemnts. 
 * \return Returns an approximate value of alpha. 
 * \throws 
 */
double RbStatistics::Helper::dppExpectNumTableFromConcParam(double conp, double num){
    
    if ( conp <= 0.0 )
		throw RbException( "The concentration parameter must be larger than 0.0." );
	if ( num < 2.0)
		throw RbException( "The Dirichlet Process expects there to be more than 1 data element: number > 2." );
	double expectedNum = 0.0;
	for (int i=1; i<=num; i++)
		expectedNum += ( 1.0 / (i - 1.0 + conp) );
	expectedNum *= conp;
	return expectedNum;
}

/*!
 * This function generates a Poisson-distributed random variable using
 * inversion by the chop down method. 
 *
 * \brief Poisson random variables using the chop down method.
 * \param lambda is the rate parameter of the Poisson. 
 * \return Returns a Poisson-distributed random variable. 
 * \throws Does not throw an error.
 */
int RbStatistics::Helper::poissonInver(double lambda, RandomNumberGenerator& rng) {
    
	const int bound = 130;
	static double p_L_last = -1.0;
	static double p_f0;
	int x;
    
	if (lambda != p_L_last) 
    {
		p_L_last = lambda;
		p_f0 = exp(-lambda);
    } 
    
	while (1) 
    {  
		double r = rng.uniform01();  
		x = 0;  
		double f = p_f0;
		do 
        {
			r -= f;
			if (r <= 0.0) 
				return x;
			x++;
			f *= lambda;
			r *= x;
        } while (x <= bound);
    }
}

/*!
 * This function generates a Poisson-distributed random variable for
 * small values of lambda. The method is a simple calculation of the 
 * probabilities of x = 1 and x = 2. Larger values are ignored.
 *
 * \brief Poisson random variables for small lambda.
 * \param lambda is the rate parameter of the Poisson. 
 * \return Returns a Poisson-distributed random variable. 
 * \throws Does not throw an error.
 */
int RbStatistics::Helper::poissonLow(double lambda, RandomNumberGenerator& rng) {
    
	double d = sqrt(lambda);
	if (rng.uniform01() >= d) 
		return 0;
	double r = rng.uniform01() * d;
	if (r > lambda * (1.0 - lambda)) 
		return 0;
	if (r > 0.5 * lambda * lambda * (1.0 - lambda)) 
		return 1;
	return 2;
}

/*!
 * This function generates a Poisson-distributed random variable using
 * the ratio-of-uniforms rejectin method. 
 *
 * \brief Poisson random variables using the ratio-of-uniforms method.
 * \param lambda is the rate parameter of the Poisson. 
 * \return Returns a Poisson-distributed random variable. 
 * \throws Does not throw an error.
 * \see Stadlober, E. 1990. The ratio of uniforms approach for generating
 *      discrete random variates. Journal of Computational and Applied 
 *      Mathematics 31:181-189.
 */
int RbStatistics::Helper::poissonRatioUniforms(double lambda, RandomNumberGenerator& rng) {
    
	static double p_L_last = -1.0;  /* previous L */
	static double p_a;              /* hat center */
	static double p_h;              /* hat width */
	static double p_g;              /* ln(L) */
	static double p_q;              /* value at mode */
	static int p_bound;             /* upper bound */
	int mode;                       /* mode */
	double u;                       /* uniform random */
	double lf;                      /* ln(f(x)) */
	double x;                       /* real sample */
	int k;                          /* integer sample */
    
	if (p_L_last != lambda) 
    {
		p_L_last = lambda;
		p_a = lambda + 0.5;
		mode = (int)lambda;
		p_g  = log(lambda);
		p_q = mode * p_g - RbMath::lnFactorial(mode);
		p_h = sqrt(2.943035529371538573 * (lambda + 0.5)) + 0.8989161620588987408;
		p_bound = (int)(p_a + 6.0 * p_h);
    }
    
	while(1) 
    {
		u = rng.uniform01();
		if (u == 0.0) 
			continue;
		x = p_a + p_h * (rng.uniform01() - 0.5) / u;
		if (x < 0 || x >= p_bound) 
			continue;
		k = (int)(x);
		lf = k * p_g - RbMath::lnFactorial(k) - p_q;
		if (lf >= u * (4.0 - u) - 3.0) 
			break;
		if (u * (u - lf) > 1.0) 
			continue;
		if (2.0 * log(u) <= lf) 
			break;
    }
	return(k);
}

/*!
 * This function is used when generating gamma-distributed random variables.
 *
 * \brief Subfunction for gamma random variables.
 * \param s is the shape parameter of the gamma. 
 * \return Returns a gamma-distributed random variable. 
 * \throws Does not throw an error.
 */
double RbStatistics::Helper::rndGamma(double s, RandomNumberGenerator& rng) {
    
	double r = 0.0;
	if (s <= 0.0)      
		std::cout << "Gamma parameter less than zero" << std::endl;
	else if (s < 1.0)  
		r = rndGamma1(s, rng);
	else if (s > 1.0)  
		r = rndGamma2(s, rng);
	else           
		r = - log(rng.uniform01());
	return (r);
}

/*!
 * This function is used when generating gamma-distributed random variables.
 *
 * \brief Subfunction for gamma random variables.
 * \param s is the shape parameter of the gamma. 
 * \return Returns a gamma-distributed random variable. 
 * \throws Does not throw an error.
 */
double RbStatistics::Helper::rndGamma1(double s, RandomNumberGenerator& rng) {
    
	double			r, x = 0.0, small = 1e-37, w;
	static double   a, p, uf, ss = 10.0, d;
	
	if (s != ss) 
    {
		a  = 1.0 - s;
		p  = a / (a + s * exp(-a));
		uf = p * pow(small / a, s);
		d  = a * log(a);
		ss = s;
    }
	for (;;) 
    {
		r = rng.uniform01();
		if (r > p)        
			x = a - log((1.0 - r) / (1.0 - p)), w = a * log(x) - d;
		else if (r>uf)  
			x = a * pow(r / p, 1.0 / s), w = x;
		else            
			return (0.0);
		r = rng.uniform01();
		if (1.0 - r <= w && r > 0.0)
            if (r*(w + 1.0) >= 1.0 || -log(r) <= w)  
                continue;
		break;
    }
    
	return (x);
}

/*!
 * This function is used when generating gamma-distributed random variables.
 *
 * \brief Subfunction for gamma random variables.
 * \param s is the shape parameter of the gamma. 
 * \return Returns a gamma-distributed random variable. 
 * \throws Does not throw an error.
 */
double RbStatistics::Helper::rndGamma2(double s, RandomNumberGenerator& rng) {
    
	double			r, d, f, g, x;
	static double	b, h, ss = 0.0;
	
	if (s != ss) 
    {
		b  = s - 1.0;
		h  = sqrt(3.0 * s - 0.75);
		ss = s;
    }
	for (;;) 
    {
		r = rng.uniform01();
		g = r - r * r;
		f = (r - 0.5) * h / sqrt(g);
		x = b + f;
		if (x <= 0.0) 
			continue;
		r = rng.uniform01();
		d = 64.0 * r * r * g * g * g;
		if (d * x < x - 2.0 * f * f || log(d) < 2.0 * (b * log(x / b) - f))  
			break;
    }
    
	return (x);
}

