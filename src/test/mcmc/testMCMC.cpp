/*
 * testMCMC.cpp
 *
 *  Created on: 26 nov 2009
 *      Author: Sebastian
 */

#include <iostream>
#include <cmath>
#include <assert.h>
#include <vector>

#include "Argument.h"
#include "ConstantNode.h"
#include "DAGNode.h"
#include "Distribution.h"
#include "DistExponential.h"
#include "DistNormal.h"
#include "DistUniform.h"
#include "MoveSlidingWindow.h"
#include "MoveScale.h"
#include "RbDouble.h"
#include "RbMove.h"
#include "RbMoveSchedule.h"
#include "RbObject.h"
#include "RbException.h"
#include "StochasticNode.h"

int main(int argc, char **argv) {
    // create a DistNormal object
    RbDouble* sigmaPrior = new RbDouble(1.0);
    RbDouble* muPriorLower = new RbDouble(-1.0);
    RbDouble* muPriorUpper = new RbDouble(1.0);
    
    std::vector<RbDouble*> x;
    for (int i=0; i<10; i++)
    	x.push_back( new RbDouble(0.5) );

    ConstantNode* cnSigmaPrior = new ConstantNode(sigmaPrior);
    ConstantNode* cnMuPriorLower = new ConstantNode(muPriorLower);
    ConstantNode* cnMuPriorUpper = new ConstantNode(muPriorUpper);

    StochasticNode* snSigma = new StochasticNode();
    StochasticNode* snMu = new StochasticNode();
    snSigma->addParentNode(cnSigmaPrior);
    snMu->addParentNode(cnMuPriorLower);
    snMu->addParentNode(cnMuPriorUpper);
    cnSigmaPrior->addChildNode(snSigma);
    cnMuPriorLower->addChildNode( snMu );
    cnMuPriorUpper->addChildNode( snMu );

	std::vector<StochasticNode*> sn;
	for (int i=0; i<10; i++)
		{
		StochasticNode* s = new StochasticNode();
		sn.push_back( s );
		s->clamp( x[i] );
		}
    
    std::vector<Distribution*> dists;
    for (int i=0; i<10; i++)
    	{
    	DistNormal* d = new DistNormal(snMu, snSigma, sn[i]);
    	dists.push_back( d );
    	sn[i]->assignDistribution(d);
    	sn[i]->addParentNode(snMu);
    	sn[i]->addParentNode(snSigma);
    	snMu->addChildNode(sn[i]);
    	snSigma->addChildNode(sn[i]);
    	}
    DistUniform* uniDist = new DistUniform( cnMuPriorLower, cnMuPriorUpper, snMu );
    DistExponential* expDist = new DistExponential( cnSigmaPrior, snSigma );
	snMu->assignDistribution(uniDist);
	snSigma->assignDistribution(expDist);
	
	std::vector<unsigned int> seed1;
	seed1.insert(1);
	seed1.insert(2);
	RandomNumberGenerator* rng = new RandomNumberGenerator(seed1);
	RbMoveSchedule* msSigma = new RbMoveSchedule( rng );
	msSigma.addMove( new MoveScale( snSigma, new RbDouble(1.5), rng ), 1.0 );
	snSigma->assignMoveSchedule( msSigma );
	
	std::vector<unsigned int> seed2;
	seed2.insert(7);
	seed2.insert(5);
	RandomNumberGenerator* rng2 = new RandomNumberGenerator(seed2);
	RbMoveSchedule* msMu = new RbMoveSchedule( rng2 );
	msMu.addMove( new MoveSlidingWindow( snMu, new RbDouble(0.1), muPriorLower, muPriorUpper, rng2 ), 1.0 );
	snMu->assignMoveSchedule( msSigma );

    // create model
    RbModel* model = new RbModel(sn);

    // run MCMC
    RbMcmc* mcmc = new RbMcmc(model);
    mcmc->runChain();
    
    // TODO some more meaningful test

    // calculate the pdf for some known parameters

    try
    {

        double pdf = ((RbDouble*) dnorm.execute(args))->getValue();
        double expectedPDF = 0.3520653; //taken from R
        std::cout << "The pdf is " << pdf << '\n';
        std::cout << "The expected pdf is " << expectedPDF << '\n';
        assert(abs(pdf-expectedPDF) <= 0.0000001);

    // calculate the lnpdf for some known parameters
//  double lnpdf = (double) dnorm.execute(args);
//  double expectedLnPDF = -1.043939; //taken from R
//  std::cout << "The lnpdf is " << lnpdf << '\n';
//  std::cout << "The expected lnpdf is " << expectedLnPDF << '\n';
//  assert(abs(lnpdf-expectedLnPDF) <= 0.000001);

        // calculate the cdf for some known parameters
        double cdf = ((RbDouble*) pnorm.execute(args))->getValue();
        double expectedCDF = 0.6914625; //taken from R
        std::cout << "The cdf is " << cdf << '\n';
        std::cout << "The expected cdf is " << expectedCDF << '\n';
        assert(abs(cdf-expectedCDF) <= 0.0000001);

        // calculate the quantile for some known parameters
        double q = ((RbDouble*) qnorm.execute(args))->getValue();
        double expectedQ = 0.0; //taken from R
        std::cout << "The quantile is " << q << '\n';
        std::cout << "The expected quantile is " << expectedQ << '\n';
        assert(abs(q-expectedQ) <= 0.0000001);
    }
    catch (RbException e)
    {
        std::cout << e.getMessage() << std::endl;
    }
    // draw some random values some known parameters and compare this to the pdf and cdf.

    assert(true);

}
