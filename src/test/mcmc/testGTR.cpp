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
#include <string>

#include "Argument.h"
#include "ConstantNode.h"
#include "DAGNode.h"
#include "DeterministicNode.h"
#include "Distribution.h"
#include "DistDirichlet.h"
#include "DistExponential.h"
#include "DistNormal.h"
#include "DistUniform.h"
#include "DistUnifUnrootedTopology.h"
#include "IntVector.h"
#include "MoveSlidingWindow.h"
#include "MoveScale.h"
#include "RbCharacterMatrix.h"
#include "RbDouble.h"
#include "RbInt.h"
#include "RbFunction_readCharacterMatrix.h"
#include "RbFunction_GTR.h"
#include "RbMcmc.h"
#include "RbModel.h"
#include "RbMonitor.h"
#include "RbMove.h"
#include "RbMoveSchedule.h"
#include "RbNames.h"
#include "RbObject.h"
#include "RbException.h"
#include "RbString.h"
#include "StochasticNode.h"
#include "RandomNumberGenerator.h"

int main(int argc, char **argv) {

    // get a random number generator
    std::vector<unsigned int> seed1;
    seed1.push_back(1);
    seed1.push_back(2);
    RandomNumberGenerator* rng = new RandomNumberGenerator(seed1);
    
    // get the data
    ConstantNode *fname = new ConstantNode( new RbString("/Users/johnh/Desktop/hdpp/bglobin.in"));
    ConstantNode *tname = new ConstantNode( new RbString(RbNames::AlignmentFileType::phylip));
    Argument fNameArg = Argument(RbNames::ReadAlignment::fileName, fname);
    Argument fTypeArg = Argument(RbNames::ReadAlignment::fileType, tname);
    std::vector<Argument> args;
	args.push_back( fNameArg );
	args.push_back( fTypeArg );
    RbFunction_readCharacterMatrix read;
    RbCharacterMatrix* aln = (RbCharacterMatrix*)read.execute(args);
    ConstantNode* alnNode = new ConstantNode( aln );
 
    // create the transition matrix
    ConstantNode* nTaxa = new ConstantNode(new RbInt(aln->getNumTaxa()));
    StochasticNode* treetop = new StochasticNode(new DistUnifUnrootedTopology(nTaxa, rng));

    std::vector<int> bv;
    for (int i=0; i<4; i++) {
        bv.push_back(1);
    }
    ConstantNode* b = new ConstantNode(new IntVector(bv));
    StochasticNode* baseFreq = new StochasticNode(new DistDirichlet(b,rng));
    std::vector<int> av;
        for (int i=0; i<6; i++) {
            av.push_back(1);
        }
    ConstantNode* a = new ConstantNode(new IntVector(av));
    StochasticNode* rates = new StochasticNode(new DistDirichlet(a,rng));


    //DeterministicNode* q = new DeterministicNode(new RbFunction_GTR());

#   if 0
    ConstantNode* lambda = new ConstantNode(new RbDouble(10.0));
    RbContainer* branches = new RbContainer();
    for (int i=0; i<aln.size()*2-2; i++) {
        RbContainer brParams;
        brParams.insert(q);
        StochasticNode brLength = new StochasticNode(new DistExponential(lambda), rng);
        brParams.insert(brLenght);
        branches.insert(new DeterministicNode(new RbFunction_Branch(brParams)));
    }
    StochasticNode* topo = new StochasticNode( new DistUniUnrootedTopology(), rng );
    DeterministicNode* t = new DeterministicNode( new RbFunction_UnrootedTree(aln.getTaxonList() ), rng);

    RbContainer nodeParams;
    RbContainer* nodes = new RbContainer();
    for (int i=0; i<aln.size()*2-2; i++) {
        nodes.insert(new DeterministicNode(new RbFunction_Node(nodeParams)));
    }

    DeterministicNode uTree = new DeterministicNode(new RbFunction_UnrootedTree(treeTop, branches, nodes));

    RbContainer* obs = new RbContainer();
    for (int i=0; i<aln.numberOfSites(); i++) {
        StochasticNode* s = new StochasticNode(new DistPhylogenetic(uTree), rng);
        s->clamp(aln.getColumn(i));
        obs.insert(s);
    }

    // add moves
    RbMoveSchedule* msSigma = new RbMoveSchedule( rng, 1.0 );
    msSigma->addMove( new MoveScale( snSigma, new RbDouble(1.5), rng ), 1.0 );
    snSigma->assignMoveSchedule( msSigma );

    // create monitors
    RbInt freq = RbInt(100);
    RbMonitor* mSigma = new RbMonitor(std::cout, snSigma, freq);
    snSigma->addMonitor(mSigma);

    // create model
    RbModel* gtr = new RbModel(obs, rng);

    // run MCMC
    RbMcmc* mcmc = new RbMcmc(model, rng);
    mcmc->runChain();
#	endif
}

