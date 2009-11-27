#include <iostream>
#include <cmath>
#include <assert.h>

#include "Argument.h"
#include "ConstantNode.h"
#include "DAGNode.h"
#include "RbDouble.h"
#include "RbInt.h"
#include "RbFunction_dbeta.h"
#include "RbFunction_pbeta.h"
#include "RbFunction_rbeta.h"
#include "RbFunction_qbeta.h"
#include "RbFunction_dchisq.h"
#include "RbFunction_pchisq.h"
#include "RbFunction_rchisq.h"
#include "RbFunction_qchisq.h"
#include "RbFunction_dexp.h"
#include "RbFunction_pexp.h"
#include "RbFunction_rexp.h"
#include "RbFunction_qexp.h"
#include "RbFunction_dnorm.h"
#include "RbFunction_pnorm.h"
#include "RbFunction_rnorm.h"
#include "RbFunction_qnorm.h"
#include "RbObject.h"
#include "RbException.h"

int main(int argc, char **argv) {
	// create a DistNormal object
	
	RbFunction_dbeta   dbeta;
	RbFunction_pbeta   pbeta;
	RbFunction_qbeta   qbeta;
	RbFunction_rbeta   rbeta;	
	
	RbFunction_dchisq  dchisq;
	RbFunction_pchisq  pchisq;
	RbFunction_qchisq  qchisq;
	RbFunction_rchisq  rchisq;	
	
	RbFunction_dexp   dexp;
	RbFunction_pexp    pexp;
	RbFunction_qexp    qexp;
	RbFunction_rexp    rexp;	
	
	RbFunction_dnorm   dnorm;
	RbFunction_pnorm   pnorm;
	RbFunction_qnorm   qnorm;
	RbFunction_rnorm   rnorm;	
	
	RbDouble* sigma  = new RbDouble(1.0);
	RbDouble* mu     = new RbDouble(0.0);
	RbDouble* x      = new RbDouble(0.5);
	RbDouble* alpha  = new RbDouble(2.0);
	RbDouble* beta   = new RbDouble(2.0);
	RbInt*    df     = new RbInt(2);
	RbDouble* lambda = new RbDouble(1.0);
	
	ConstantNode* cnSigma  = new ConstantNode(sigma);
	ConstantNode* cnMu     = new ConstantNode(mu);
	ConstantNode* cnX      = new ConstantNode(x);
	ConstantNode* cnAlpha  = new ConstantNode(alpha);
	ConstantNode* cnBeta   = new ConstantNode(beta);
	ConstantNode* cnDf     = new ConstantNode(df);
	ConstantNode* cnLambda = new ConstantNode(lambda);
	
	Argument* aMu     = new Argument("mu", cnMu);
	Argument* aSigma  = new Argument("sigma", cnSigma);
	Argument* aX      = new Argument("", cnX);
	Argument* aAlpha  = new Argument("alpha", cnAlpha);
	Argument* aBeta   = new Argument("beta", cnBeta);
	Argument* aDf     = new Argument("df", cnDf);
	Argument* aLambda = new Argument("rate", cnLambda);
	
	std::vector<Argument*> args;

	// TODO some more meaningful test

	// calculate the pdf for some known parameters
	
	try 
	{
		//Beta distribution
		std::cout << "Testing Beta Distribution" << '\n';
		args.push_back( aX );
		args.push_back( aAlpha );
		args.push_back( aBeta );
	
		double pdf = ((RbDouble*) dbeta.execute(args))->getValue();
		double expectedPDF = 1.5; //taken from R
		std::cout << "The pdf is " << pdf << '\n';
		std::cout << "The expected pdf is " << expectedPDF << '\n';
		assert(fabs(pdf-expectedPDF) <= 0.0000001);

		// calculate the cdf for some known parameters
		double cdf = ((RbDouble*) pbeta.execute(args))->getValue();
		double expectedCDF = 0.5; //taken from R
		std::cout << "The cdf is " << cdf << '\n';
		std::cout << "The expected cdf is " << expectedCDF << '\n';
		assert(fabs(cdf-expectedCDF) <= 0.0000001);

		// calculate the quantile for some known parameters
		double q = ((RbDouble*) qbeta.execute(args))->getValue();
		double expectedQ = 0.5; //taken from R
		std::cout << "The quantile is " << q << '\n';
		std::cout << "The expected quantile is " << expectedQ << '\n';
		assert(fabs(q-expectedQ) <= 0.0000001);
		
		
		
		// chi-square distribution
		std::cout << "Testing Chi-Square Distribution" << '\n';
		args.clear();
		args.push_back( aX );
		args.push_back( aDf );
	
		pdf = ((RbDouble*) dchisq.execute(args))->getValue();
		expectedPDF = 0.3894004; //taken from R
		std::cout << "The pdf is " << pdf << '\n';
		std::cout << "The expected pdf is " << expectedPDF << '\n';
		assert(fabs(pdf-expectedPDF) <= 0.0000001);

		// calculate the cdf for some known parameters
		cdf = ((RbDouble*) pchisq.execute(args))->getValue();
		expectedCDF = 0.2211992; //taken from R
		std::cout << "The cdf is " << cdf << '\n';
		std::cout << "The expected cdf is " << expectedCDF << '\n';
		assert(fabs(cdf-expectedCDF) <= 0.0000001);

		// calculate the quantile for some known parameters
		q = ((RbDouble*) qchisq.execute(args))->getValue();
		expectedQ = 1.386294; //taken from R
		std::cout << "The quantile is " << q << '\n';
		std::cout << "The expected quantile is " << expectedQ << '\n';
		assert(fabs(q-expectedQ) <= 0.00001);
		
		
		
		// exponential distribution
		std::cout << "Testing Exponential Distribution" << '\n';
		args.clear();
		args.push_back( aX );
		args.push_back( aLambda );
	
		pdf = ((RbDouble*) dexp.execute(args))->getValue();
		expectedPDF = 0.6065307; //taken from R
		std::cout << "The pdf is " << pdf << '\n';
		std::cout << "The expected pdf is " << expectedPDF << '\n';
		assert(fabs(pdf-expectedPDF) <= 0.0000001);

		// calculate the cdf for some known parameters
		cdf = ((RbDouble*) pexp.execute(args))->getValue();
		expectedCDF = 0.3934693; //taken from R
		std::cout << "The cdf is " << cdf << '\n';
		std::cout << "The expected cdf is " << expectedCDF << '\n';
		assert(fabs(cdf-expectedCDF) <= 0.0000001);

		// calculate the quantile for some known parameters
		q = ((RbDouble*) qexp.execute(args))->getValue();
		expectedQ = 0.6931472; //taken from R
		std::cout << "The quantile is " << q << '\n';
		std::cout << "The expected quantile is " << expectedQ << '\n';
		assert(fabs(q-expectedQ) <= 0.0000001);
		
		
		
		
		// normal distribution
		std::cout << "Testing Normal Distribution" << '\n';
		args.clear();
		args.push_back( aX );
		args.push_back( aSigma );
		args.push_back( aMu );
	
		pdf = ((RbDouble*) dnorm.execute(args))->getValue();
		expectedPDF = 0.3520653; //taken from R
		std::cout << "The pdf is " << pdf << '\n';
		std::cout << "The expected pdf is " << expectedPDF << '\n';
		assert(fabs(pdf-expectedPDF) <= 0.0000001);

		// calculate the cdf for some known parameters
		cdf = ((RbDouble*) pnorm.execute(args))->getValue();
		expectedCDF = 0.6914625; //taken from R
		std::cout << "The cdf is " << cdf << '\n';
		std::cout << "The expected cdf is " << expectedCDF << '\n';
		assert(fabs(cdf-expectedCDF) <= 0.0000001);

		// calculate the quantile for some known parameters
		q = ((RbDouble*) qnorm.execute(args))->getValue();
		expectedQ = 0.0; //taken from R
		std::cout << "The quantile is " << q << '\n';
		std::cout << "The expected quantile is " << expectedQ << '\n';
		assert(fabs(q-expectedQ) <= 0.0000001);
	}
	catch (RbException e)
	{
		std::cout << e.getMessage() << std::endl;
	}
	// draw some random values some known parameters and compare this to the pdf and cdf.

	assert(true);

}
