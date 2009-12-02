/*
 * testGTR.cpp
 *
 *  Created on: 2 dec 2009
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

    // get a random number generator
    std::vector<unsigned int> seed1;
    seed1.push_back(1);
    seed1.push_back(2);
    RandomNumberGenerator* rng = new RandomNumberGenerator(seed1);

    // create the transition matrix


    // create tree

    // create topology

    // create branches

    // create nodes



    // add moves
    RbMoveSchedule* msSigma = new RbMoveSchedule( rng, 1.0 );
    msSigma->addMove( new MoveScale( snSigma, new RbDouble(1.5), rng ), 1.0 );
    snSigma->assignMoveSchedule( msSigma );

    // create monitors
    RbInt freq = RbInt(100);
    RbMonitor* mSigma = new RbMonitor(std::cout, snSigma, freq);
    snSigma->addMonitor(mSigma);

    // create model
    RbModel* gtr = new RbModel(sn, rng);

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

