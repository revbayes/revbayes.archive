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
#include "RbMcmc.h"
#include "RbModel.h"
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

	std::vector<DAGNode*> sn;
	for (int i=0; i<10; i++)
		{
		StochasticNode* s = new StochasticNode();
		sn.push_back( s );
		s->clamp( x[i] );
		}
    
    std::vector<Distribution*> dists;
    for (int i=0; i<10; i++)
    	{
    	DistNormal* d = new DistNormal(snMu, snSigma);
    	dists.push_back( d );
    	((StochasticNode*)sn[i])->assignDistribution(d);
    	sn[i]->addParentNode(snMu);
    	sn[i]->addParentNode(snSigma);
    	snMu->addChildNode(sn[i]);
    	snSigma->addChildNode(sn[i]);
    	}
    DistUniform* uniDist = new DistUniform( cnMuPriorLower, cnMuPriorUpper );
    DistExponential* expDist = new DistExponential( cnSigmaPrior );
	snMu->assignDistribution(uniDist);
	snSigma->assignDistribution(expDist);
	
	std::vector<unsigned int> seed1;
	seed1.push_back(1);
	seed1.push_back(2);
	RandomNumberGenerator* rng = new RandomNumberGenerator(seed1);
	RbMoveSchedule* msSigma = new RbMoveSchedule( rng );
	msSigma->addMove( new MoveScale( snSigma, new RbDouble(1.5), rng ), 1.0 );
	snSigma->assignMoveSchedule( msSigma );
	
	std::vector<unsigned int> seed2;
	seed2.push_back(7);
	seed2.push_back(5);
	RandomNumberGenerator* rng2 = new RandomNumberGenerator(seed2);
	RbMoveSchedule* msMu = new RbMoveSchedule( rng2 );
	msMu->addMove( new MoveSlidingWindow( snMu, new RbDouble(0.1), muPriorLower, muPriorUpper, rng2 ), 1.0 );
	snMu->assignMoveSchedule( msMu );

    // create model
    RbModel* model = new RbModel(sn);

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
