//
//  TestCharacterHistory.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 8/6/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "TestCharacterHistory.h"

// sorted
#include "AreaSizeRateModifier.h"
#include "BetaSimplexMove.h"
#include "BetaDistribution.h"
#include "BranchHistory.h"
#include "CharacterEvent.h"
#include "CharacterHistoryNodeMonitor.h"
#include "CharacterHistoryCtmcNodeUpdate.h"
#include "CharacterHistoryCtmcPathUpdate.h"
#include "Clade.h"
#include "ConstantRateBirthDeathProcess.h"
#include "ConstantNode.h"
#include "ContinuousStochasticNode.h"
#include "DeterministicNode.h"
#include "DirichletDistribution.h"
#include "DispersalHistoryCtmc.h"
#include "ExponentialDistribution.h"
#include "FileMonitor.h"
#include "GammaDistribution.h"
#include "GeographicDistanceRateModifier.h"
#include "GeographicGridRateModifier.h"
#include "Mcmc.h"
#include "Model.h"
#include "Monitor.h"
#include "Move.h"
#include "NclReader.h"
#include "PhylowoodNhxMonitor.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "ScaleMove.h"
#include "ScreenMonitor.h"
#include "SimplexMove.h"
#include "StochasticNode.h"
#include "TimeAtlas.h"
#include "TimeTree.h"
#include "TimeAtlasDataReader.h"
#include "TreeCharacterHistory.h"
#include "TestCharacterHistory.h"
#include "UniformDistribution.h"
#include "VectorFunction.h"

// unsorted test headers
#include "RateMatrix_GTR.h"
#include "GtrRateMatrixFunction.h"

// experimental RateMap setup
#include "RateMap_Biogeography.h"
#include "BiogeographyRateMapFunction.h"
#include "BiogeographicTreeHistoryCtmc.h"
#include "FreeBinaryRateMatrixFunction.h"
#include "SamplePathHistoryCtmcMove.h"
#include "SampleNodeHistoryCtmcMove.h"
#include "TreeCharacterHistoryNodeMonitor.h"
#include "TreeCharacterHistoryNhxMonitor.h"
#include "MetropolisHastingsMove.h"
#include "PathRejectionSampleProposal.h"
#include "NodeRejectionSampleProposal.h"
#include "TipRejectionSampleProposal.h"
#include "PathRejectionSampleMove.h"
#include "UniformTimeTreeDistribution.h"
#include "BranchLengthTree.h"
#include "UniformBranchLengthTreeDistribution.h"
#include "DistanceDependentDispersalFunction.h"

//#define USE_DDBD

using namespace RevBayesCore;


TestCharacterHistory::TestCharacterHistory(const std::string &afn,  const std::string &tfn, const std::string &gfn, int gen, const std::string &fp) : filepath(fp), areaFilename( afn ), treeFilename(tfn),  geoFilename(gfn), mcmcGenerations( gen )
{
    ;
}

TestCharacterHistory::TestCharacterHistory(int ac, const char* av[], const std::string &afn, const std::string &tfn, const std::string &gfn, int gen, const std::string &fp) : filepath(fp), areaFilename( afn ), treeFilename(tfn),  geoFilename(gfn), mcmcGenerations( gen ), argc(ac), argv(av)
{
    ;
}

void TestCharacterHistory::tokenizeArgv(void)
{
    for (int i = 0; i < argc; i++)
    {
        argTokens.push_back(argv[i]);
        std::cout << i << " " << argTokens[i] << "\n";
    }
    std::cout << argc << " == " << argTokens.size() << " arguments\n";
    if (argc > 1)
    {
        areaFilename = argTokens[1];
        treeFilename = argTokens[2];
    }

}

TestCharacterHistory::~TestCharacterHistory() {
    // nothing to do
}

bool TestCharacterHistory::run_exp(void) {
    
    
    ////////////
    // settings
    ////////////
    
    //unsigned int burn = (unsigned int)(mcmcGenerations * .2);

    ////////////
    // io
    ////////////
    
    std::vector<unsigned> old_seed = GLOBAL_RNG->getSeed();
    std::vector<unsigned> seed;
    seed.push_back(3); seed.push_back(2);
//    GLOBAL_RNG->setSeed(seed);
    std::stringstream ss;
    ss << ".s0_" << old_seed[0] << ".s1_" << old_seed[1];

    
    bool usingAmbiguousCharacters = !true;
    bool simulate = false;
    bool useDistances = !true;
    bool useClock = !true;
//    bool useTipProbs = false;
    bool forbidExtinction = true;
    filepath="/Users/mlandis/data/bayarea/output/";
    
    // binary characters
    //std::string fn = "vireya_sim.nex";
    //std::string fn = "vireya_sim2.nex";
    std::string fn = "vireya_sim_0_1.nex";
    //std::string fn = "vireya_gain0_02_loss0_06_dp3.nex";
    //std::string fn = "bg.test.nex";
    //std::string fn = "bg.break.nex";
    //std::string fn = "vireya.nex";
    std::string in_fp = "/Users/mlandis/Documents/code/revbayes-code/examples/data/";
    std::vector<AbstractCharacterData*> data = NclReader::getInstance().readMatrices(in_fp + fn);
    std::cout << "Read " << data.size() << " matrices." << std::endl;
    size_t numAreas = data[0]->getNumberOfCharacters();
    
//    for (size_t i = 0; i < data[0]->getNumberOfTaxa(); i++)
//    {
//        const DiscreteTaxonData<StandardState>& dtd = static_cast<const DiscreteTaxonData<StandardState>& >(data[0]->getTaxonData(i));
//        std::cout << dtd << "\n";
//    }
    
    
    // tree
    std::vector<TimeTree*> trees = NclReader::getInstance().readTimeTrees( in_fp + fn );
    std::cout << "Read " << trees.size() << " trees." << std::endl;
    std::cout << trees[0]->getNewickRepresentation() << std::endl;
    size_t numNodes = trees[0]->getNumberOfNodes();

    // geo by epochs
    TimeAtlasDataReader tsdr(in_fp+"vireya.atlas.txt",'\t');
    TimeAtlas* ta = new TimeAtlas(&tsdr);
    
    ////////////
    // model
    ////////////
    
    // clock
    ContinuousStochasticNode* clockRate = new ContinuousStochasticNode("clockRate", new GammaDistribution( new ConstantNode<double>("clockPrior_A", new double(2.0)),
                                                                                                           new ConstantNode<double>("clockPrior_B", new double(2.0))));
    if (!useClock)
        clockRate->setValue(new double(1.0));
    
    // tree
    std::vector<std::string> names = data[0]->getTaxonNames();
    ConstantNode<double>* origin = new ConstantNode<double>( "origin", new double( trees[0]->getRoot().getAge() ) );
    ConstantNode<double> *div = new ConstantNode<double>("diversification", new double(1.0));
    ConstantNode<double> *turn = new ConstantNode<double>("turnover", new double(0.0));
    ConstantNode<double> *rho = new ConstantNode<double>("rho", new double(1.0));
//    StochasticNode<TimeTree>* tau = new StochasticNode<TimeTree>("tau", new UniformTimeTreeDistribution(origin, names));
    StochasticNode<TimeTree> *tau = new StochasticNode<TimeTree>( "tau", new ConstantRateBirthDeathProcess(origin, div, turn, rho, "uniform", "survival", int(names.size()), names, std::vector<Clade>()) );
    tau->setValue( trees[0] );
    
    
    // geo distances
    DeterministicNode<GeographicDistanceRateModifier>* ddd = NULL;
    ContinuousStochasticNode* dp = NULL;
    if (useDistances)
    {
        ConstantNode<double> *dp_pr = new ConstantNode<double>( "distancePowerPrior", new double(1.0));
        dp = new ContinuousStochasticNode("distancePower", new ExponentialDistribution(dp_pr));
        //dp->setValue(new double(0.1));
        ddd = new DeterministicNode<GeographicDistanceRateModifier>("dddFunction", new DistanceDependentDispersalFunction(dp,ta));
    }
    
    // ctmc rates
    ConstantNode<double>* gainRatePrior = new ConstantNode<double>("glr_pr", new double(1.0));
    std::vector<const TypedDagNode<double> *> gainLossRates;
	std::vector< ContinuousStochasticNode *> gainLossRates_nonConst;
	for( size_t i=0; i<2; i++){
        std::ostringstream glr_name;
        glr_name << "r(" << i << ")";
		ContinuousStochasticNode* tmp_glr = new ContinuousStochasticNode( glr_name.str(), new ExponentialDistribution(gainRatePrior));
        //tmp_glr->setValue(new double(0.01));
		gainLossRates.push_back( tmp_glr );
		gainLossRates_nonConst.push_back( tmp_glr );
	}
    
//    ConstantNode<std::vector<double> > *glr_dir_pr = new ConstantNode<std::vector<double> >( "glr_dir_pr", new std::vector<double>(2,1.0) );
//    StochasticNode<std::vector<double> > *glr_dir = new StochasticNode<std::vector<double> >( "glr_dir", new DirichletDistribution(glr_dir_pr) );
//    DeterministicNode< std::vector< double > >* glr_dir_vector = new DeterministicNode< std::vector< double > >( "glr_dir_vector", new VectorFunction< double >( glr_dir ) );
    
    DeterministicNode< std::vector< double > >* glr_vector = new DeterministicNode< std::vector< double > >( "glr_vector", new VectorFunction< double >( gainLossRates ) );

    // Q-map used to compute likehood under the full model
    BiogeographyRateMapFunction* brmf_likelihood = new BiogeographyRateMapFunction(numAreas, forbidExtinction);
    brmf_likelihood->setGainLossRates(glr_vector);
//    brmf_likelihood->setGainLossRates(glr_dir);
    brmf_likelihood->setClockRate(clockRate);
    if (useDistances)
        brmf_likelihood->setGeographicDistanceRateModifier(ddd);
    DeterministicNode<RateMap> *q_likelihood = new DeterministicNode<RateMap>("Q_like", brmf_likelihood);
    
    // Q-map used to sample path histories
    BiogeographyRateMapFunction* brmf_sample = new BiogeographyRateMapFunction(numAreas, false);
    brmf_sample->setGainLossRates(glr_vector);
    brmf_sample->setClockRate(clockRate);
    DeterministicNode<RateMap> *q_sample = new DeterministicNode<RateMap>("Q_sample", brmf_sample);
        
    // and the character model
    BiogeographicTreeHistoryCtmc<StandardState, TimeTree> *biogeoCtmc = new BiogeographicTreeHistoryCtmc<StandardState, TimeTree>(tau, 2, numAreas, usingAmbiguousCharacters, forbidExtinction);
    biogeoCtmc->setRateMap(q_likelihood);
    if (data.size() == 2 && usingAmbiguousCharacters)
        biogeoCtmc->setTipProbs( data[1] );
    
    StochasticNode< AbstractCharacterData > *charactermodel = new StochasticNode< AbstractCharacterData >("ctmc", biogeoCtmc );
    
    // simulated data
    if (simulate)
        biogeoCtmc->simulate();

    // real data
    else
        charactermodel->clamp( data[0] );
    
    // initialize mapping
    charactermodel->redraw();
    
    std::cout << "lnL = " << charactermodel->getDistribution().computeLnProbability() << "\n";
    
    
    GLOBAL_RNG->setSeed(old_seed);
    gainLossRates_nonConst[0]->redraw();
    gainLossRates_nonConst[1]->redraw();

    std::cout << "seed " << old_seed[0] << " " << old_seed[1] << "\n";
    
    ////////////
    // moves
    ////////////
    
    std::cout << "Adding moves\n";
    std::vector<Move*> moves;
    
    if (useClock)
    {
        moves.push_back( new ScaleMove(clockRate, 1.0, true, 2) );
        moves.push_back( new ScaleMove(clockRate, 0.25, true, 4) );
    }
    
    if (useDistances)
    {
        moves.push_back( new ScaleMove(dp, 0.25, true, 4) );
        moves.push_back( new ScaleMove(dp, 1.0, true, 2) );
    }
    
    for( size_t i=0; i<2; i++)
    {
        moves.push_back( new ScaleMove(gainLossRates_nonConst[i], 1.0, false, 3) );
		moves.push_back( new ScaleMove(gainLossRates_nonConst[i], 0.25, false, 5) );
    }

    TopologyNode* nd = NULL; // &tau->getValue().getNode(60);
    PathRejectionSampleProposal<StandardState,TimeTree>* pathSampleProposal = new PathRejectionSampleProposal<StandardState,TimeTree>(charactermodel, tau, q_sample, 0.4, nd);
    moves.push_back(new PathRejectionSampleMove<StandardState, TimeTree>(charactermodel, tau, q_sample, pathSampleProposal, 0.5, false, 100));
    
    NodeRejectionSampleProposal<StandardState,TimeTree>* nodeSampleProposal = new NodeRejectionSampleProposal<StandardState,TimeTree>(charactermodel, tau, q_sample, 0.2, nd);
    moves.push_back(new PathRejectionSampleMove<StandardState, TimeTree>(charactermodel, tau, q_sample, nodeSampleProposal, 0.5, false, 50));//numNodes));

 

//    
//    if (useTipProbs)
//    {
//        TipRejectionSampleProposal<StandardState,TimeTree>* tipSampleProposal = new TipRejectionSampleProposal<StandardState,TimeTree>(charactermodel, tau, q_sample, 0.3);
//        moves.push_back(new PathRejectionSampleMove<StandardState, TimeTree>(charactermodel, tau, q_sample, tipSampleProposal, 0.3, false, 1*numNodes));
//    }


//    moves.push_back(new SamplePathHistoryCtmcMove<StandardState, TimeTree>(charactermodel, tau, 0.2, false, 50));
    
    
    ////////////
    // monitors
    ////////////
    
    std::cout << "Adding monitors\n";
    std::vector<Monitor*> monitors;
    
    std::set<DagNode*> monitoredNodes;
    monitoredNodes.insert(clockRate);
    if (useDistances)
        monitoredNodes.insert( dp );
    monitoredNodes.insert( glr_vector );
    

    monitors.push_back(new FileMonitor(monitoredNodes, 100, filepath + "rb" + ss.str() + ".mcmc.txt", "\t"));
    monitors.push_back(new ScreenMonitor(monitoredNodes, 1, "\t" ) );
    monitors.push_back(new TreeCharacterHistoryNodeMonitor<StandardState,TimeTree>(charactermodel, tau, 100, filepath + "rb." + ss.str() + ".tree_chars.txt", "\t"));
    // monitors.push_back(new TreeCharacterHistoryNhxMonitor<StandardState,TimeTree>(charactermodel, tau, geo_coords, 50, mcmcGenerations, burn, filepath + "rb.phylowood.txt", "\t"));
    //    monitors.push_back( new CharacterHistoryNodeMonitor( tau, bh_vector_stochastic, 50, filepath + "rb.tree_chars.txt", "\t" ));
    
    //    PhylowoodNhxMonitor* phwnm = new PhylowoodNhxMonitor( tau, bh_vector_stochastic, geo_coords, 50, maxGen, burn, filepath + "rb.phylowood.txt", "\t" );
    //    monitors.push_back(phwnm);

    
    //////////
    // model
    //////////
    std::cout << "Instantiating model\n";
    Model myModel = Model(charactermodel);
    
    
    //////////
    // mcmc
    //////////
    std::cout << "Instantiating mcmc\n";
    Mcmc myMcmc = Mcmc( myModel, moves, monitors, "random", true, 1.0, 0 );
    myMcmc.run(mcmcGenerations*10);
    myMcmc.printOperatorSummary();
    

    //////////
    // clean-up
    //////////
    std::cout << "Cleaning up objects\n";
    
    // segfault when uncommented? strange.
//    delete dp;
//    delete glr;
//    delete sglr;
    
    for (std::vector<Move*>::iterator it = moves.begin(); it != moves.end(); ++it) {
        const Move *theMove = *it;
        delete theMove;
    }
    for (std::vector<Monitor*>::iterator it = monitors.begin(); it != monitors.end(); ++it) {
        const Monitor *theMonitor = *it;
        delete theMonitor;
    }
    
    std::cout << "Finished CharacterHistory model test." << std::endl;
    
    return true;
}




////////////////////////
// drichter test module
////////////////////////


bool TestCharacterHistory::run_dollo(void) {
    
    
    ////////////
    // settings
    ////////////
    
    //unsigned int burn = (unsigned int)(mcmcGenerations * .2);
    std::vector<unsigned> old_seed = GLOBAL_RNG->getSeed();
    std::vector<unsigned> seed;
    seed.push_back(3); seed.push_back(2);
    //    GLOBAL_RNG->setSeed(seed);
    std::stringstream ss;
    ss << ".s0_" << old_seed[0] << ".s1_" << old_seed[1];
    
    ////////////
    // io
    ////////////
    bool simulate = false;
    bool useClock = false;
    bool forbidExtinction = !true;
    bool usingAmbiguousCharacters = !true;
    filepath="/Users/mlandis/data/ngene/";
    
    // binary characters
    std::string fn = "dolloplus.nex";
    std::string in_fp = "/Users/mlandis/data/ngene/";
   
    std::vector<AbstractCharacterData*> data = NclReader::getInstance().readMatrices(in_fp + fn);
    std::cout << "Read " << data.size() << " matrices." << std::endl;
    size_t numAreas = data[0]->getNumberOfCharacters();
    
    // tree
//    std::vector<TimeTree*> trees = NclReader::getInstance().readTimeTrees( in_fp + fn );
    std::vector<BranchLengthTree*>* trees_ptr = NclReader::getInstance().readBranchLengthTrees(in_fp + fn,"nexus");
    std::vector<BranchLengthTree*> trees = *trees_ptr;
    std::cout << "Read " << trees.size() << " trees." << std::endl;
    std::cout << trees[0]->getNewickRepresentation() << std::endl;
    size_t numNodes = trees[0]->getNumberOfNodes();
    
    ////////////
    // model
    ////////////
    
    // clock
    ConstantNode<double> *clockPriorA = new ConstantNode<double>("clockPrior_A", new double(2.0));
    ConstantNode<double> *clockPriorB = new ConstantNode<double>("clockPrior_B", new double(2.0));
    StochasticNode<double> *clockRate = new StochasticNode<double>("clockRate", new GammaDistribution(clockPriorA, clockPriorB) );
    clockRate->setValue(new double(1.0));
    
    // tree
    std::vector<std::string> names = data[0]->getTaxonNames();
    ConstantNode<double>* maxBrLen = new ConstantNode<double>("maxBrLen", new double(10e6));
    StochasticNode<BranchLengthTree>* tau = new StochasticNode<BranchLengthTree>("tau_tmp", new UniformBranchLengthTreeDistribution(maxBrLen, names));
    tau->setValue( trees[0] );
    
    // gain loss rates
    ConstantNode<double>* gainRatePrior = new ConstantNode<double>("glr_pr", new double(4.0));
    std::vector<const TypedDagNode<double> *> gainLossRates;
	std::vector< ContinuousStochasticNode *> gainLossRates_nonConst;
	for( size_t i=0; i<2; i++){
        std::ostringstream glr_name;
        glr_name << "r(" << i << ")";
		ContinuousStochasticNode* tmp_glr = new ContinuousStochasticNode( glr_name.str(), new ExponentialDistribution(gainRatePrior, new ConstantNode<double>("offset", new double(0.0) )));
        //tmp_glr->setValue(new double(0.1));
		gainLossRates.push_back( tmp_glr );
		gainLossRates_nonConst.push_back( tmp_glr );
	}
    DeterministicNode< std::vector< double > >* glr_vector = new DeterministicNode< std::vector< double > >( "glr_vector", new VectorFunction< double >( gainLossRates ) );
    
    // Q-map used to compute likehood under the full model
    BiogeographyRateMapFunction* brmf_likelihood = new BiogeographyRateMapFunction(numAreas,forbidExtinction);
    brmf_likelihood->setGainLossRates(glr_vector);
    DeterministicNode<RateMap> *q_likelihood = new DeterministicNode<RateMap>("Q_l", brmf_likelihood);
    
    // Q-map used to sample path histories
    BiogeographyRateMapFunction* brmf_sample = new BiogeographyRateMapFunction(numAreas,false);
    brmf_sample->setGainLossRates(glr_vector);
    DeterministicNode<RateMap> *q_sample = new DeterministicNode<RateMap>("Q_s", brmf_sample);
    
    // and the character model
    BiogeographicTreeHistoryCtmc<StandardState, BranchLengthTree> *biogeoCtmc = new BiogeographicTreeHistoryCtmc<StandardState, BranchLengthTree>(tau, 2, numAreas, usingAmbiguousCharacters,forbidExtinction);
    biogeoCtmc->setRateMap(q_likelihood);
    
    if (data.size() == 2 && usingAmbiguousCharacters)
        biogeoCtmc->setTipProbs( data[1] );

    StochasticNode< AbstractCharacterData > *charactermodel = new StochasticNode< AbstractCharacterData >("ctmc", biogeoCtmc );
    
    
    // simulated data
    if (simulate)
        biogeoCtmc->simulate();
    
    // real data
    else
        charactermodel->clamp( data[0] );
    
    // initialize mapping
    charactermodel->redraw();
    
    std::cout << "lnL = " << charactermodel->getDistribution().computeLnProbability() << "\n";
    //    std::cout << GLOBAL_RNG->getSeed()[0] << "\n";
    //    std::cout << GLOBAL_RNG->getSeed()[1] << "\n";
    
    ////////////
    // moves
    ////////////
    
    std::cout << "Adding moves\n";
    std::vector<Move*> moves;
    if (useClock)
    {
        moves.push_back( new ScaleMove(clockRate, 1.0, true, 2) );
        moves.push_back( new ScaleMove(clockRate, 0.25, true, 4) );
    }
    
    for( size_t i=0; i<2; i++)
    {
        moves.push_back( new ScaleMove(gainLossRates_nonConst[i], 1.0, false, 3) );
		moves.push_back( new ScaleMove(gainLossRates_nonConst[i], 0.25, false, 5) );
    }
    
    TopologyNode* nd = NULL;

    moves.push_back(new PathRejectionSampleMove<StandardState, BranchLengthTree>(charactermodel, tau, q_sample, new PathRejectionSampleProposal<StandardState,BranchLengthTree>(charactermodel, tau, q_sample, 0.2, nd), 0.2, false, 100));
   // moves.push_back(new PathRejectionSampleMove<StandardState, BranchLengthTree>(charactermodel, tau, q_sample, new PathRejectionSampleProposal<StandardState,BranchLengthTree>(charactermodel, tau, q_sample, 0.05, nd), 0.05, false, 100));
    
    
//    moves.push_back(new PathRejectionSampleMove<StandardState, BranchLengthTree>(charactermodel, tau, q_sample, new NodeRejectionSampleProposal<StandardState,BranchLengthTree>(charactermodel, tau, q_sample, 0.2, nd), 0.2, false, 50));
//    moves.push_back(new PathRejectionSampleMove<StandardState, BranchLengthTree>(charactermodel, tau, q_sample, new NodeRejectionSampleProposal<StandardState,BranchLengthTree>(charactermodel, tau, q_sample, 0.05, nd), 0.05, false, 50));
//    
    
    
    
    if (usingAmbiguousCharacters)
    {
        moves.push_back(new PathRejectionSampleMove<StandardState, BranchLengthTree>(charactermodel, tau, q_sample, new TipRejectionSampleProposal<StandardState,BranchLengthTree>(charactermodel, tau, q_sample, 0.5, nd), 0.5, false, 50));
    }
    
    
    ////////////
    // monitors
    ////////////
    
    std::cout << "Adding monitors\n";
    std::vector<Monitor*> monitors;
    
    std::set<DagNode*> monitoredNodes;
//    monitoredNodes.insert(clockRate);
    monitoredNodes.insert( glr_vector );
    //monitoredNodes.insert( dp );
    
    monitors.push_back(new FileMonitor(monitoredNodes, 100, filepath + "rb" + ss.str() + ".mcmc.txt", "\t"));
    monitors.push_back(new ScreenMonitor(monitoredNodes, 1, "\t" ) );
    monitors.push_back(new TreeCharacterHistoryNodeMonitor<StandardState,BranchLengthTree>(charactermodel, tau, 100, filepath + "rb." + ss.str() + ".tree_chars.txt", "\t"));

    // monitors.push_back(new TreeCharacterHistoryNhxMonitor<StandardState,TimeTree>(charactermodel, tau, geo_coords, 50, mcmcGenerations, burn, filepath + "rb.phylowood.txt", "\t"));
    //    monitors.push_back( new CharacterHistoryNodeMonitor( tau, bh_vector_stochastic, 50, filepath + "rb.tree_chars.txt", "\t" ));
    
    //    PhylowoodNhxMonitor* phwnm = new PhylowoodNhxMonitor( tau, bh_vector_stochastic, geo_coords, 50, maxGen, burn, filepath + "rb.phylowood.txt", "\t" );
    //    monitors.push_back(phwnm);
    
    
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
    myMcmc.run(mcmcGenerations*10);
    myMcmc.printOperatorSummary();
    
    
    //////////
    // clean-up
    //////////
    std::cout << "Cleaning up objects\n";
    
    // segfault when uncommented? strange.
    //    delete dp;
    //    delete glr;
    //    delete sglr;
    
    for (std::vector<Move*>::iterator it = moves.begin(); it != moves.end(); ++it) {
        const Move *theMove = *it;
        delete theMove;
    }
    for (std::vector<Monitor*>::iterator it = monitors.begin(); it != monitors.end(); ++it) {
        const Monitor *theMonitor = *it;
        delete theMonitor;
    }
    
    std::cout << "Finished CharacterHistory model test." << std::endl;
    
    return true;
}

























bool TestCharacterHistory::run( void ) {
    
    //////////
    // io
    //////////
    filepath = "/Users/mlandis/data/bayarea/output/";
    geoFilename = "rb.geo.txt";
    areaFilename = "rb.areas.txt";
    treeFilename = "rb.tree.txt";
    
    areaNodeFilename = "area_node.txt";
    areaEdgeFilename = "area_edge.txt";
    timeatlasFilename = "timeatlas.txt";
    TimeAtlasDataReader tsdr(filepath+timeatlasFilename,'\t');
    TimeAtlas ta(&tsdr);
    int numEpochs = (int)ta.getEpochs().size();
    
    //////////
    // test
    int maxGen = 10000;//(int)((double)mcmcGenerations / 10);
    std::vector<unsigned int> seed;
    //seed.push_back(3); seed.push_back(3); GLOBAL_RNG->setSeed(seed);
    
    size_t numTaxa = 20;
    size_t numNodes = 2 * numTaxa - 1;
    std::vector<std::string> taxonNames;
    for (size_t i = 0; i < numTaxa; i++)
    {
        std::stringstream ss;
        ss << i;
        taxonNames.push_back("p" + ss.str());
    }
    
    size_t numCharacters = std::pow(10,2.0);
    size_t numStates = 2;
    
    // assign area coordinates
    std::vector<std::vector<double> > geo_coords;
    std::string geo_type = "square";
    if (geo_type == "square")
    {
        for (int i = 0; i < sqrt(numCharacters); i++)
        {
            for (int j = 0; j < sqrt(numCharacters); j++)
            {
                std::vector<double> tmp;
                tmp.push_back(i-5);
                tmp.push_back(j-70);
                //std::cout << i << " " << j << "\n";
                geo_coords.push_back(tmp);
            }
        }
    }
    else if (geo_type == "line")
    {
        for (size_t i = 0; i < numCharacters; i++)
        {
            std::vector<double> tmp;
            tmp.push_back(0);
            tmp.push_back(i);
            geo_coords.push_back(tmp);
        }
    }

    // assign area sizes
    std::vector<double> areaSizes;
    double areaFactor = .5;
    if (geo_type == "square")
    {
        for (size_t i = 0; i < sqrt(numCharacters); i++)
        {
            for (size_t j = 0; j < sqrt(numCharacters); j++)
            {
                areaSizes.push_back(1 + i * areaFactor + j * areaFactor);
            }
        }
    }
    else if (geo_type == "line")
    {
        
        for (size_t i = 0; i < numCharacters; i++)
        {
            areaSizes.push_back(1 + i * areaFactor);
       
        }

    }
    //for (size_t i =0; i <numCharacters; i++) std::cout << areaSizes[i] << " "; std::cout << "\n";

    
    //////////
    
    // substitution rates
    ConstantNode<double>* rateGain_pr = new ConstantNode<double>("r1_pr", new double(5.0));
    ConstantNode<double>* rateLoss_pr = new ConstantNode<double>("r0_pr", new double(5.0));
    StochasticNode<double>* rateGain = new StochasticNode<double>("r1", new ExponentialDistribution(rateGain_pr));
    StochasticNode<double>* rateLoss = new StochasticNode<double>("r0", new ExponentialDistribution(rateLoss_pr));
    
    // tree
    ConstantNode<double> *div = new ConstantNode<double>("diversification", new double(1.0));
    ConstantNode<double> *turn = new ConstantNode<double>("turnover", new double(0.5));
    ConstantNode<double> *rho = new ConstantNode<double>("rho", new double(1.0));
    ConstantNode<double>* origin = new ConstantNode<double>( "origin", new double( 2.0 ) );
    StochasticNode<TimeTree>* tau = new StochasticNode<TimeTree>( "tau", new ConstantRateBirthDeathProcess(origin, div, turn, rho, "uniform", "survival", int(taxonNames.size()), taxonNames, std::vector<Clade>()) );
    tau->clamp(new TimeTree(tau->getValue()));
    std::cout << tau->getValue().getNewickRepresentation() << "\n";

    // geographical distance power
    ConstantNode<double>* distancePower_pr = new ConstantNode<double>("dist_pow_pr", new double(2.0));
    StochasticNode<double>* distancePower = new StochasticNode<double>("dist_pow", new ExponentialDistribution(distancePower_pr));
    //distancePower->setValue(new double(pow(10,-6)));
    
    // geographic distances
    GeographicDistanceRateModifier* gdrm;// = new GeographicDistanceRateModifier(geo_coords);
    //gdrm->update();
    
    // geographic grid timeatlas
    std::vector<GeographicGridRateModifier*> ggrmv;
//    for (size_t i = 0; i < (size_t)numTimes; i++)
//    {
//        ggrmv.push_back(new GeographicGridRateModifier(&ta, i));
//    }
    // For this to work, must provide DisperalHistoryCtmc the age of the event to query the proper atlas slice
    
    
    // range size bd rates
    ConstantNode<double>* extinctionPower_pr = new ConstantNode<double>("ext_pow_pr", new double(5.0));
    ConstantNode<double>* dispersalPower_pr = new ConstantNode<double>("disp_pow_pr", new double(5.0));
    ConstantNode<double>* areaStationaryFrequency_pr_a = new ConstantNode<double>("area_freq_pr_a", new double(1.0));
    ConstantNode<double>* areaStationaryFrequency_pr_b = new ConstantNode<double>("area_freq_pr_b", new double(1.0));
    
    StochasticNode<double>* extinctionPower = new StochasticNode<double>("ext_pow", new ExponentialDistribution(extinctionPower_pr));
    StochasticNode<double>* dispersalPower = new StochasticNode<double>("disp_pow", new ExponentialDistribution(dispersalPower_pr));
//    StochasticNode<double>* dispersalPower = new StochasticNode<double>("disp_pow", new BetaDistribution(areaStationaryFrequency_pr_a, areaStationaryFrequency_pr_b));
    StochasticNode<double>* areaStationaryFrequency = new StochasticNode<double>("area_freq", new BetaDistribution(areaStationaryFrequency_pr_a, areaStationaryFrequency_pr_b));
    
    RangeSizeRateModifier* grsrm = new RangeSizeRateModifier((unsigned int)numCharacters, 1);
    RangeSizeRateModifier* lrsrm = new RangeSizeRateModifier((unsigned int)numCharacters, 1);

    // area size rate modifiers    
    ConstantNode<double>* areaPower_pr = new ConstantNode<double>("area_pow_pr", new double(2.0));
    StochasticNode<double>* areaPower = new StochasticNode<double>("area_pow", new ExponentialDistribution(areaPower_pr));
    AreaSizeRateModifier* asrm = new AreaSizeRateModifier(areaSizes);
    
    // simulation settings
    
    ggrmv.clear();
    gdrm = NULL;
    grsrm = NULL;
    lrsrm = NULL;
    asrm = NULL;
    

    distancePower->setValue(new double(3.0));
    areaStationaryFrequency->setValue(new double(0.2));
    areaPower->setValue(new double(0.5));

//    rateGain->setValue(new double(.3));
//    rateLoss->setValue(new double(3.0));
    rateGain->setValue(new double(0.2));
    rateLoss->setValue(new double(3.0));

    std::vector<const TypedDagNode<double>* > rates;
    rates.push_back(rateLoss);
    rates.push_back(rateGain);
 
    extinctionPower->setValue(new double(0.0));
    dispersalPower->setValue(new double(0.0));
    
    std::cout << "--------------\n";
    std::cout << "rateGain       = " << rateGain->getValue() << "\n";
    std::cout << "rateLoss       = " << rateLoss->getValue() << "\n";
    std::cout << "dispersalPower = " << dispersalPower->getValue() << "\n";
    std::cout << "extinctPower   = " << extinctionPower->getValue() << "\n";
    std::cout << "areaFreq       = " << areaStationaryFrequency->getValue() << "\n";
    std::cout << "areaPower      = " << areaPower->getValue() << "\n";
    std::cout << "distancePower  = " << distancePower->getValue() << "\n";
    std::cout << "--------------\n";
    
    ///////////////
    // test for a tree character history
    
    // branch rates
    std::vector<const TypedDagNode<double>* > br_vector;
    ConstantNode<double>* branchRate_pr = new ConstantNode<double>("br_pr", new double(1.0));
    for (size_t i = 0; i < numNodes; i++)
    {
        std::stringstream ss;
        ss << i;
        StochasticNode<double>* tmp_br = new StochasticNode<double>("br" + ss.str(), new ExponentialDistribution(branchRate_pr));
        tmp_br->setValue(1.0);
        tmp_br->clamp(new double(1.0));
        br_vector.push_back(tmp_br);
    }

    // branch histories
    int maskMod = 2;
    std::set<int> maskSet;
    for (size_t i = 0; i < numCharacters; i++)
    {
        if (i % maskMod != 0)
            maskSet.insert(i);
    }
    
    std::vector<const TypedDagNode<BranchHistory>* > bh_vector;
    std::vector<StochasticNode<BranchHistory>* > bh_vector_stochastic;
    for (size_t i = 0; i < numNodes; i++)
    {
        std::stringstream ss;
        ss << i;
        DispersalHistoryCtmc* tmp_ahc = new DispersalHistoryCtmc(rates, tau, br_vector[i], distancePower, dispersalPower, extinctionPower, areaStationaryFrequency, areaPower, numCharacters, numStates, i, gdrm, grsrm, lrsrm, asrm, ggrmv);
        //DispersalHistoryCtmc* tmp_ahc = new DispersalHistoryCtmc(q, rates, tau, br_vector[i], distancePower, numCharacters, numStates, i, NULL);
        
        if (i < 2)
            tmp_ahc->getValue().setSampleChildCharacters(maskSet);
        
        StochasticNode<BranchHistory>* sn_bh = new StochasticNode<BranchHistory>("bh" + ss.str(), tmp_ahc);
        bh_vector.push_back(sn_bh);
        bh_vector_stochastic.push_back(sn_bh);
    }
    // unclamp parentcharacters...
    //bh_vector_stochastic[0]->getValue().setClampChildCharacters(false);
    
    // tree history
    TreeCharacterHistory* tch = new TreeCharacterHistory( bh_vector, tau, numCharacters, numStates );
    tch->simulate();

    // get true values under sim
    std::vector<CharacterEvent*> bh_true = bh_vector[0]->getValue().getChildCharacters();
    
    StochasticNode<BranchHistory>* tr_chm = new StochasticNode<BranchHistory>("tr_model", tch);
    tr_chm->redraw();
    
 //   rateGain->setValue(new double(rateGain->getValue()/10));
 //   rateLoss->setValue(new double(rateLoss->getValue()/10));
    
    ///////////////
    
    std::cout << "Adding moves\n";
    std::vector<Move*> moves;
    moves.push_back( new ScaleMove(distancePower, 1.0, true, 5.0) );
    moves.push_back( new ScaleMove(rateGain, 5.0, true, 5.0) );
    moves.push_back( new ScaleMove(rateLoss, 1.0, true, 5.0) );
//    moves.push_back( new ScaleMove(dispersalPower, 1.0, true, 5.0));
//    moves.push_back( new ScaleMove(extinctionPower, 1.0, true, 5.0));
//    moves.push_back( new BetaSimplexMove(areaStationaryFrequency, 5.0, true, 5.0));
//    moves.push_back( new ScaleMove(areaPower, 1.0, true, 5.0) );
    
    for (size_t i = 0; i < br_vector.size(); i++)
    {
        //TypedDagNode<double>* br_tdn = const_cast<TypedDagNode<double>* >(br_vector[i]);
        //StochasticNode<double>* br_sn = static_cast<StochasticNode<double>* >(br_tdn);
        ;//moves.push_back( new ScaleMove(br_sn, 1.0, true, 2.0) );
    }
        
    for (size_t i = 0; i < bh_vector.size(); i++)
    {
        TypedDagNode<BranchHistory>* bh_tdn = const_cast<TypedDagNode<BranchHistory>* >(bh_vector[i]);
        StochasticNode<BranchHistory>* bh_sn = static_cast<StochasticNode<BranchHistory>* >(bh_tdn);
        moves.push_back( new CharacterHistoryCtmcPathUpdate(bh_sn, 0.1, true, 2.0) );
        if (i >= numTaxa || i == 0 || i == 1)
            moves.push_back( new CharacterHistoryCtmcNodeUpdate(bh_sn, bh_vector_stochastic, tau, 0.1, true, 5.0));
    }
    
    for (size_t i = numTaxa; i < bh_vector.size(); i++)
    {
        ; // node update for bh
    }
    
    ///////////////
    
    std::cout << "Adding monitors\n";
    std::vector<Monitor*> monitors;

    std::set<DagNode*> monitoredNodes;
    monitoredNodes.insert( rateGain );
    monitoredNodes.insert( rateLoss );
    monitoredNodes.insert( distancePower );
//    monitoredNodes.insert( dispersalPower );
//    monitoredNodes.insert( extinctionPower );
//    monitoredNodes.insert( areaStationaryFrequency );
//    monitoredNodes.insert( areaPower );
    
    
    for (size_t i = 0; i < br_vector.size(); i++)
    {
        //TypedDagNode<double>* br_tdn = const_cast<TypedDagNode<double>* >(br_vector[i]);
        ;//monitoredNodes.insert( br_tdn );
    }
    
    for (size_t i = 0; i < bh_vector.size(); i++)
    {
        TypedDagNode<BranchHistory>* bh_tdn = const_cast<TypedDagNode<BranchHistory>* >(bh_vector[i]);
        monitoredNodes.insert( bh_tdn );
    }

    
    monitors.push_back( new FileMonitor( monitoredNodes, 10, filepath + "rb.mcmc.txt", "\t" ) );
    monitors.push_back( new CharacterHistoryNodeMonitor( tau, bh_vector_stochastic, 50, filepath + "rb.tree_chars.txt", "\t" ));
    unsigned int burn = (unsigned int)(maxGen * .2);
    PhylowoodNhxMonitor* phwnm = new PhylowoodNhxMonitor( tau, bh_vector_stochastic, geo_coords, 50, maxGen, burn, filepath + "rb.phylowood.txt", "\t" );
    monitors.push_back(phwnm);
    monitors.push_back( new ScreenMonitor( monitoredNodes, 1, "\t" ) );

    
    //TypedDagNode<TimeTree>* t1 = NULL;
    //std::vector<TypedDagNode<BranchHistory>* > bh1;
    
    //CharacterHistoryNodeMonitor(t1, bh1, 10, filepath+"rb.tree_chars.txt", "\t");
    
    //  CharacterHistoryNodeMonitor(TypedDagNode<TimeTree> *t, std::vector< TypedDagNode< BranchHistory >* > bh, int g, const std::string &fname, const std::string &del, bool pp=true, bool l=true, bool pr=true, bool ap=false, bool sm=true, bool sr=true);
 
    ///////////////
    
    std::cout << "Instantiating model\n";
    Model myModel = Model(tr_chm);

    
    ///////////////
    std::cout << "Instantiating mcmc\n";
    Mcmc myMcmc = Mcmc( myModel, moves, monitors );
    myMcmc.run(maxGen);
    myMcmc.printOperatorSummary();

    
//    BranchHistory bh_inf(bh_vector[0]->getValue());
    std::cout << "[";
    for (size_t i = 0; i < bh_true.size(); i++)
    {
        if (i != 0)
            std::cout << ",";
        std::cout << bh_true[i]->getState();
    }
    std::cout << "]\n";
    
//    std::vector<unsigned int> bh_inf = phwnm->getChildCharacterCounts(0);
//    long ns = phwnm->getNumSamples();
//    for (size_t i = 0; i < bh_inf.size(); i++)
//    {
//        std::cout << "true " << bh_true[i]->getState() << "\n";
//        std::cout << "inf " << bh_inf[i] << "\n";
//        std::cout << ns << "\n";
//        double x = (double)bh_true[i]->getState() - (double)bh_inf[i] / ns;
//        //std::cout << x << " ";
//    }
//    std::cout << "\n";
    
    
//    for (size_t i = 0; i < numNodes; i++)
//    {
//        TypedDagNode<BranchHistory>* bh_tdn = const_cast<TypedDagNode<BranchHistory>* >(bh_vector[i]);
//        bh_tdn->getValue().print();
//    }
    
    ///////////////
    
    std::cout << "Cleaning up objects\n";
    delete distancePower;
    delete rateGain;
    delete rateLoss;

    for (std::vector<Move*>::iterator it = moves.begin(); it != moves.end(); ++it) {
        const Move *theMove = *it;
        delete theMove;
    }
    for (std::vector<Monitor*>::iterator it = monitors.begin(); it != monitors.end(); ++it) {
        const Monitor *theMonitor = *it;
        delete theMonitor;
    }
    
    std::cout << "Finished CharacterHistory model test." << std::endl;
    
    
    return true;
}

