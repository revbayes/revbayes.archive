//
//  TestPathSampling.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 9/9/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#include "TestPathSampling.h"

// sorted
#include "BetaSimplexMove.h"
#include "BetaDistribution.h"
#include "BranchHistory.h"
#include "CharacterEvent.h"
#include "CharacterHistoryNodeMonitor.h"
#include "Clade.h"
#include "ConstantRateBirthDeathProcess.h"
#include "ConstantNode.h"
#include "ContinuousStochasticNode.h"
#include "DeterministicNode.h"
#include "DirichletDistribution.h"
#include "ExponentialDistribution.h"
#include "FileMonitor.h"
#include "GammaDistribution.h"
#include "Mcmc.h"
#include "MetropolisHastingsMove.h"
#include "Model.h"
#include "Monitor.h"
#include "Move.h"
#include "NormalDistribution.h"
#include "NclReader.h"
#include "PhylowoodNhxMonitor.h"
#include "RateMatrix.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "ScaleProposal.h"
#include "ScreenMonitor.h"
#include "SimplexMove.h"
#include "SlidingMove.h"
#include "StochasticNode.h"
#include "TimeTree.h"
#include "TimeAtlas.h"
#include "TimeAtlasDataReader.h"
#include "TestPathSampling.h"
#include "UniformDistribution.h"
#include "VectorFunction.h"
#include "VectorScaleMove.h"

// experimental RateMap setup
#include "RateMap_Biogeography.h"
#include "BiogeographyRateMapFunction.h"
#include "BiogeographicTreeHistoryCtmc.h"
#include "FreeBinaryRateMatrixFunction.h"
#include "TreeCharacterHistoryNodeMonitor.h"
#include "TreeCharacterHistoryNhxMonitor.h"
#include "MetropolisHastingsMove.h"
#include "PathRejectionSampleProposal.h"
#include "BiogeographyPathRejectionSampleProposal.h"
#include "NodeRejectionSampleProposal.h"
#include "BiogeographyNodeRejectionSampleProposal.h"
#include "TipRejectionSampleProposal.h"
#include "PathRejectionSampleMove.h"
#include "UniformTimeTreeDistribution.h"
#include "PathUniformizationSampleProposal.h"


// amino acid uniformization test
#include "GeneralRateMapFunction.h"
#include "AminoAcidState.h"
#include "RateMap.h"
#include "RateMatrix_Blosum62.h"
#include "GeneralTreeHistoryCtmc.h"



using namespace RevBayesCore;

TestPathSampling::TestPathSampling(void)
{
    ;
}
TestPathSampling::~TestPathSampling() {
    // nothing to do
}

bool TestPathSampling::run(void)
{
    int i = 1;
    if      (i == 0)
        return run_binary();
    else if (i == 1)
        return run_aa();
    else return false;
}

bool TestPathSampling::run_binary( void )
{
    
    ////////////
    // settings
    ////////////
    unsigned mcmcGenerations = 1000;
    
    ////////////
    // io
    ////////////
    
    std::vector<unsigned> old_seed = GLOBAL_RNG->getSeed();
    std::vector<unsigned> seed;
    //    seed.push_back(1+1); seed.push_back(1);
    //    GLOBAL_RNG->setSeed(seed);
    std::stringstream ss;
    ss << ".s0_" << old_seed[0] << ".s1_" << old_seed[1];
    
    
    std::string filepath="/Users/mlandis/data/bayarea/output/";
    
    // binary characters
    std::string fn = "";
    fn = "16tip_100areas.nex";
    
    std::string in_fp = "/Users/mlandis/Documents/code/revbayes-code/examples/data/";
    std::vector<AbstractCharacterData*> data = NclReader::getInstance().readMatrices(in_fp + fn);
    std::cout << "Read " << data.size() << " matrices." << std::endl;
    size_t numAreas = data[0]->getNumberOfCharacters();
    
    // tree
    std::vector<TimeTree*> trees = NclReader::getInstance().readTimeTrees( in_fp + fn );
    std::cout << "Read " << trees.size() << " trees." << std::endl;
    std::cout << trees[0]->getNewickRepresentation() << std::endl;
    size_t numNodes = trees[0]->getNumberOfNodes();
    
    // geo by epochs
    std::string afn = "100area.atlas.txt";
    TimeAtlasDataReader tsdr(in_fp + afn,'\t');
    const TimeAtlas* ta = new TimeAtlas(&tsdr);
    
    ////////////
    // model
    ////////////
    
    // tree
    ConstantNode<TimeTree>* tau = new ConstantNode<TimeTree>("tree", new TimeTree( *trees[0] ));
    
    // ctmc rates
    ConstantNode<double>* glr_pr = new ConstantNode<double>("glr_pr", new double(100.0));
    std::vector<const TypedDagNode<double> *> glr;
    std::vector<StochasticNode<double>* > glr_stoch;
	std::vector< ContinuousStochasticNode *> glr_nonConst;
	for( size_t i=0; i<2; i++){
        std::ostringstream glr_name;
        glr_name << "r(" << i << ")";
		ContinuousStochasticNode* tmp_glr = new ContinuousStochasticNode( glr_name.str(), new ExponentialDistribution(glr_pr));
        //        ContinuousStochasticNode* tmp_glr = new ContinuousStochasticNode( glr_name.str(), new GammaDistribution(glr_pr_a, glr_pr_b));
        //        tmp_glr->setValue(new double(0.1));
		glr.push_back( tmp_glr );
		glr_nonConst.push_back( tmp_glr );
        glr_stoch.push_back(tmp_glr);
	}
    glr_nonConst[0]->setValue(0.1);
    glr_nonConst[1]->setValue(0.1);
    DeterministicNode< std::vector< double > >* glr_vector = new DeterministicNode< std::vector< double > >( "glr_vector", new VectorFunction< double >( glr ) );
    
    // Q-map used to compute likehood under the full model
    BiogeographyRateMapFunction* brmf_likelihood = new BiogeographyRateMapFunction(numAreas, false);
    brmf_likelihood->setGainLossRates(glr_vector);
    DeterministicNode<RateMap> *q_likelihood = new DeterministicNode<RateMap>("Q_like", brmf_likelihood);
    
    // Q-map used to sample path histories
    FreeBinaryRateMatrixFunction* fbrmf = new FreeBinaryRateMatrixFunction(glr_vector);
    DeterministicNode<RateMatrix>* q_sample = new DeterministicNode<RateMatrix>("Q_samp", fbrmf);
    
    // and the character model
    BiogeographicTreeHistoryCtmc<StandardState, TimeTree> *biogeoCtmc = new BiogeographicTreeHistoryCtmc<StandardState, TimeTree>(tau, 2, numAreas, false, false, false);
    biogeoCtmc->setRateMap(q_likelihood);
    StochasticNode< AbstractCharacterData > *charactermodel = new StochasticNode< AbstractCharacterData >("ctmc", biogeoCtmc );
    
    // assign tips and sample histories
    charactermodel->clamp( data[0] );
    charactermodel->redraw();
    
    std::cout << "lnL = " << charactermodel->getDistribution().computeLnProbability() << "\n";
    
    //    GLOBAL_RNG->setSeed(old_seed);
    //    glr_nonConst[0]->redraw();
    //    glr_nonConst[1]->redraw();
    
    std::cout << "seed " << old_seed[0] << " " << old_seed[1] << "\n";
    
    ////////////
    // moves
    ////////////
    
    std::cout << "Adding moves\n";
    RbVector<Move> moves;
    
    // update our transition rates
    for( size_t i=0; i<2; i++)
    {
        moves.push_back( new MetropolisHastingsMove( new ScaleProposal(glr_nonConst[i], 0.1), 2.0, !true ) );
    }
    
    // path
    moves.push_back(new MetropolisHastingsMove(new PathUniformizationSampleProposal<StandardState,TimeTree>(charactermodel, tau, q_sample, 0.5), numNodes*2, false));
//    moves.push_back(new PathRejectionSampleMove<StandardState, TimeTree>(charactermodel, tau, q_sample, new PathRejectionSampleProposal<StandardState,TimeTree>(charactermodel, tau, q_sample, 0.4), 0.4, false, numNodes));
    
//    // node
//    moves.push_back(new PathRejectionSampleMove<StandardState, TimeTree>(charactermodel, tau, q_sample, new NodeRejectionSampleProposal<StandardState,TimeTree>(charactermodel, tau, q_sample, 0.1), 0.1, false, numNodes*2));
//    moves.push_back(new PathRejectionSampleMove<StandardState, TimeTree>(charactermodel, tau, q_sample, new NodeRejectionSampleProposal<StandardState,TimeTree>(charactermodel, tau, q_sample, 0.4), 0.4, false, numNodes));
    
    
    
    ////////////
    // monitors
    ////////////
    
    std::cout << "Adding monitors\n";
    RbVector<Monitor> monitors;
    
    std::set<DagNode*> monitoredNodes;
    monitoredNodes.insert( glr_vector );
    
    monitors.push_back(new FileMonitor(monitoredNodes, 10, filepath + "rb" + ss.str() + ".parameters.txt", "\t"));
    monitors.push_back(new ScreenMonitor(monitoredNodes, 10, "\t" ) );
    monitors.push_back(new TreeCharacterHistoryNodeMonitor<StandardState,TimeTree>(charactermodel, tau, 10, filepath + "rb" + ss.str() + ".events.txt", "\t"));
    monitors.push_back(new TreeCharacterHistoryNodeMonitor<StandardState,TimeTree>(charactermodel, tau, 10, filepath + "rb" + ss.str() + ".counts.txt", "\t", true, true, true, false, true, true, false));
    monitors.push_back(new TreeCharacterHistoryNhxMonitor<StandardState,TimeTree>(charactermodel, tau, ta, 100, mcmcGenerations, 0.2, filepath + "rb" + ss.str() + ".nhx.txt", "\t"));
    
    monitors.push_back(new FileMonitor(tau, 10, filepath + "rb" + ss.str() + ".trees.txt", "\t"));
    
    //////////
    // model
    //////////
    std::cout << "Instantiating model\n";
    Model myModel = Model(charactermodel);
    
    
    //////////
    // mcmc
    //////////
    std::cout << "Instantiating mcmc\n";
    Mcmc myMcmc = Mcmc( myModel, moves, monitors );
    myMcmc.setScheduleType("single");
    myMcmc.run(mcmcGenerations);
    myMcmc.printOperatorSummary();
    
    
    //////////
    // clean-up
    //////////
    std::cout << "Cleaning up objects\n";
    
    // segfault when uncommented? strange.
    //    delete dp;
    //    delete glr;
    //    delete sglr;
    std::cout << "Finished CharacterHistory model test." << std::endl;
    
    return true;
}










bool TestPathSampling::run_aa( void )
{
    

    ////////////
    // settings
    ////////////
    unsigned mcmcGenerations = 1000;
    
    ////////////
    // io
    ////////////
    
    std::vector<unsigned> old_seed = GLOBAL_RNG->getSeed();
    std::vector<unsigned> seed;
    //    seed.push_back(1+1); seed.push_back(1);
    //    GLOBAL_RNG->setSeed(seed);
    std::stringstream ss;
    ss << ".s0_" << old_seed[0] << ".s1_" << old_seed[1];
    
    std::string filepath="/Users/mlandis/data/bayarea/output/";
    std::string fn = "";
    fn = "16tip_10aa.nex";
    
    std::string in_fp = "/Users/mlandis/Documents/code/revbayes-code/examples/data/";
    std::vector<AbstractCharacterData*> data = NclReader::getInstance().readMatrices(in_fp + fn);
    std::cout << "Read " << data.size() << " matrices." << std::endl;
    size_t numChars = data[0]->getNumberOfCharacters();
    size_t numStates = 20;
    
    // tree
    std::vector<TimeTree*> trees = NclReader::getInstance().readTimeTrees( in_fp + fn );
    std::cout << "Read " << trees.size() << " trees." << std::endl;
    std::cout << trees[0]->getNewickRepresentation() << std::endl;
    size_t numNodes = trees[0]->getNumberOfNodes();
    
    ////////////
    // model
    ////////////
    
    // tree
    ConstantNode<TimeTree>* tau = new ConstantNode<TimeTree>("tree", new TimeTree( *trees[0] ));
    
    // Q matrix for independence model/sampling
    ConstantNode<RateMatrix>* q_sample = new ConstantNode<RateMatrix>("Q_samp", new RateMatrix_Blosum62());
    
    // Q-map used to compute likehood under the full model
    GeneralRateMapFunction* aarmf_likelihood = new GeneralRateMapFunction(numStates, numChars);
    aarmf_likelihood->setRateMatrix(q_sample);
    DeterministicNode<RateMap> *q_likelihood = new DeterministicNode<RateMap>("Q_like", aarmf_likelihood);
        
    // and the character model
    GeneralTreeHistoryCtmc<AminoAcidState, TimeTree> *ctmc = new GeneralTreeHistoryCtmc<AminoAcidState, TimeTree>(tau, 20, numChars, false);
    ctmc->setRateMap(q_likelihood);
    StochasticNode< AbstractCharacterData > *charactermodel = new StochasticNode< AbstractCharacterData >("ctmc", ctmc );
    
    // assign tips and sample histories
    charactermodel->clamp( data[0] );
    
    charactermodel->redraw();
    
    std::cout << "lnL = " << charactermodel->getDistribution().computeLnProbability() << "\n";
/*
    //    GLOBAL_RNG->setSeed(old_seed);
    //    glr_nonConst[0]->redraw();
    //    glr_nonConst[1]->redraw();
    
    std::cout << "seed " << old_seed[0] << " " << old_seed[1] << "\n";
 
    ////////////
    // moves
    ////////////
    
    std::cout << "Adding moves\n";
    RbVector<Move> moves;
    
    // update our transition rates
    for( size_t i=0; i<2; i++)
    {
        moves.push_back( new MetropolisHastingsMove( new ScaleProposal(glr_nonConst[i], 0.1), 2.0, !true ) );
    }
    
    // path
    moves.push_back(new MetropolisHastingsMove(new PathUniformizationSampleProposal<StandardState,TimeTree>(charactermodel, tau, q_sample, 0.5), numNodes*2, false));
    //    moves.push_back(new PathRejectionSampleMove<StandardState, TimeTree>(charactermodel, tau, q_sample, new PathRejectionSampleProposal<StandardState,TimeTree>(charactermodel, tau, q_sample, 0.4), 0.4, false, numNodes));
    
    //    // node
    //    moves.push_back(new PathRejectionSampleMove<StandardState, TimeTree>(charactermodel, tau, q_sample, new NodeRejectionSampleProposal<StandardState,TimeTree>(charactermodel, tau, q_sample, 0.1), 0.1, false, numNodes*2));
    //    moves.push_back(new PathRejectionSampleMove<StandardState, TimeTree>(charactermodel, tau, q_sample, new NodeRejectionSampleProposal<StandardState,TimeTree>(charactermodel, tau, q_sample, 0.4), 0.4, false, numNodes));
    
    
    
    ////////////
    // monitors
    ////////////
    
    std::cout << "Adding monitors\n";
    RbVector<Monitor> monitors;
    
    std::set<DagNode*> monitoredNodes;
    monitoredNodes.insert( glr_vector );
    
    monitors.push_back(new FileMonitor(monitoredNodes, 10, filepath + "rb" + ss.str() + ".parameters.txt", "\t"));
    monitors.push_back(new ScreenMonitor(monitoredNodes, 10, "\t" ) );
    monitors.push_back(new TreeCharacterHistoryNodeMonitor<StandardState,TimeTree>(charactermodel, tau, 10, filepath + "rb" + ss.str() + ".events.txt", "\t"));
    monitors.push_back(new TreeCharacterHistoryNodeMonitor<StandardState,TimeTree>(charactermodel, tau, 10, filepath + "rb" + ss.str() + ".counts.txt", "\t", true, true, true, false, true, true, false));
    monitors.push_back(new TreeCharacterHistoryNhxMonitor<StandardState,TimeTree>(charactermodel, tau, ta, 100, mcmcGenerations, 0.2, filepath + "rb" + ss.str() + ".nhx.txt", "\t"));
    
    monitors.push_back(new FileMonitor(tau, 10, filepath + "rb" + ss.str() + ".trees.txt", "\t"));
    
    //////////
    // model
    //////////
    std::cout << "Instantiating model\n";
    Model myModel = Model(charactermodel);
    
    
    //////////
    // mcmc
    //////////
    std::cout << "Instantiating mcmc\n";
    Mcmc myMcmc = Mcmc( myModel, moves, monitors );
    myMcmc.setScheduleType("single");
    myMcmc.run(mcmcGenerations);
    myMcmc.printOperatorSummary();
    
    
    //////////
    // clean-up
    //////////
    std::cout << "Cleaning up objects\n";
    
    // segfault when uncommented? strange.
    //    delete dp;
    //    delete glr;
    //    delete sglr;
    std::cout << "Finished CharacterHistory model test." << std::endl;
*/
    return true;
}


