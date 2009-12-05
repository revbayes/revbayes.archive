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
#include "RbInt.h"
#include "RbMcmc.h"
#include "RbModel.h"
#include "RbMonitor.h"
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

	std::vector<unsigned int> seed1;
	seed1.push_back(1);
	seed1.push_back(2);
	RandomNumberGenerator* rng = new RandomNumberGenerator(seed1);
    StochasticNode* snSigma = new StochasticNode( new DistExponential( cnSigmaPrior, rng) );
    StochasticNode* snMu = new StochasticNode( new DistUniform( cnMuPriorLower, cnMuPriorUpper, rng) );

	std::vector<DAGNode*> sn;
	for (int i=0; i<10; i++)
		{
		StochasticNode* s = new StochasticNode(new DistNormal(snMu, snSigma, rng) );
		sn.push_back( s );
		s->clamp( x[i] );
		}
	
	RbMoveSchedule* msSigma = new RbMoveSchedule( rng, 1.0 );
	msSigma->addMove( new MoveScale( snSigma, new RbDouble(1.5), rng ), 1.0 );
	snSigma->assignMoveSchedule( msSigma );
	
	std::vector<unsigned int> seed2;
	seed2.push_back(7);
	seed2.push_back(5);
	RandomNumberGenerator* rng2 = new RandomNumberGenerator(seed2);
	RbMoveSchedule* msMu = new RbMoveSchedule( rng2, 2.5 );
	msMu->addMove( new MoveSlidingWindow( snMu, muPriorLower, muPriorUpper, new RbDouble(0.1), rng2 ), 1.0 );
	snMu->assignMoveSchedule( msMu );
	
	// create monitors
	RbInt freq = RbInt(100);
	RbMonitor* mSigma = new RbMonitor(std::cout, snSigma, freq);
	snSigma->addMonitor(mSigma);
	RbMonitor* mMu = new RbMonitor(std::cout, snMu, freq);
	snMu->addMonitor(mMu);
	
    // create model
    RbModel* model = new RbModel(sn, rng);

    // run MCMC
    RbMcmc* mcmc = new RbMcmc(model, rng);
    mcmc->runChain();
    
    // TODO some more meaningful test

    // calculate the pdf for some known parameters

    try
    {


    }
    catch (RbException e)
    {
        std::cout << e.getMessage() << std::endl;
    }
    // draw some random values some known parameters and compare this to the pdf and cdf.

    assert(true);

}
