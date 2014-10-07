//
//  TestCharacterHistory.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 8/6/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "TestCharacterHistory.h"

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
#include "GeographyRateModifier.h"
#include "Mcmc.h"
#include "MetropolisHastingsMove.h"
#include "Model.h"
#include "Monitor.h"
#include "Move.h"
#include "NormalDistribution.h"
#include "NclReader.h"
#include "PhylowoodNhxMonitor.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "ScaleProposal.h"
#include "ScreenMonitor.h"
#include "SimplexMove.h"
#include "SlidingMove.h"
#include "StochasticNode.h"
#include "TimeAtlas.h"
#include "TimeTree.h"
#include "TimeAtlasDataReader.h"
#include "TestCharacterHistory.h"
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
#include "BranchLengthTree.h"
#include "UniformBranchLengthTreeDistribution.h"
#include "DistanceDependentDispersalFunction.h"
#include "PathUniformizationSampleProposal.h"
#include "FreeBinaryRateMatrixFunction.h"

// joint mol bd test
#include "RootTimeSlide.h"
#include "OriginTimeSlide.h"
#include "NodeTimeSlideUniform.h"
#include "GeneralBranchHeterogeneousCharEvoModel.h"
#include "GtrRateMatrixFunction.h"
#include "BirthRateConstBDStatistic.h"
#include "DeathRateConstBDStatistic.h"
#include "TreeScale.h"
#include "SubtreeScale.h"
#include "MeanVecContinuousValStatistic.h"
#include "TestTreeTraceSummary.h"
#include "TreeHeightStatistic.h"
#include "TreeSummary.h"
#include "TreeTrace.h"

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

bool TestCharacterHistory::run( void ) {
 
    int idx = 0;
    switch(idx)
    {
        case 0: return run_exp();
        case 1: return run_mol();
        case 2: return run_dollo();
        default: break;
    };
    return false;
    
}







bool TestCharacterHistory::run_exp(void) {
    
    
    ////////////
    // settings
    ////////////
    
    mcmcGenerations = 1000000;
//    *= 10;
    //    mcmcGenerations=35;
    unsigned int burn = (unsigned int)(mcmcGenerations * .5);
    
    ////////////
    // io
    ////////////
    
    std::vector<unsigned> old_seed = GLOBAL_RNG->getSeed();
    std::cout << old_seed[0] << " " << old_seed[1] << "\n";
    std::vector<unsigned> seed;
//    seed.push_back(1+1); seed.push_back(1);
//    old_seed = seed;
//    GLOBAL_RNG->setSeed(seed);
    std::stringstream ss;
    ss << ".s0_" << old_seed[0] << ".s1_" << old_seed[1];
    
    bool usingAmbiguousCharacters   = !true;
    bool simulate                   = false;
    bool useClock                   = !true;
    bool useBdProcess               = !true;
    
    bool forbidExtinction           = true;
    bool useCladogenesis            = true;
    bool useDistances               = !true;
    bool useAdjacency               = true;
    bool useAvailable               = true;
    bool useRootFreqs               = !true;
    
    
    filepath="/Users/mlandis/data/bayarea/output/";
    
    // binary characters
    std::string fn = "";
//    fn = "vireya.nex";
//    fn = "psychotria_range.nex";
//    fn = "16tip_100areas.nex";
//    fn = "sim_aus_50tip_33area.nex";
//    fn = "turtles.nex";
    fn = "earth23.nex";
    std::string in_fp = "/Users/mlandis/Documents/code/revbayes-code/examples/data/";
    std::vector<AbstractCharacterData*> data = NclReader::getInstance().readMatrices(in_fp + fn);
    std::cout << "Read " << data.size() << " matrices." << std::endl;
//    size_t numAreas = data[0]->getNumberOfCharacters();
    
    // tree
    std::vector<TimeTree*> trees = NclReader::getInstance().readTimeTrees( in_fp + fn );
//    std::vector<TimeTree*> trees = NclReader::getInstance().readTimeTrees( "/Users/mlandis/Desktop/test_tree.txt" );
    std::cout << "Read " << trees.size() << " trees." << std::endl;
    std::cout << trees[0]->getNewickRepresentation() << std::endl;
    size_t numNodes = trees[0]->getNumberOfNodes();
    
    // geo by epochs
    std::string afn="";
//    afn = "earth23.atlas.txt";
//    afn = "malesia_static.atlas.txt";
//    afn = "hawaii_dynamic.atlas.txt";
//    afn = "hawaii_static.atlas.txt";
//    afn = "hawaii_dynamic_ss.atlas.txt";
//    afn = "100area.atlas.txt";
//    afn = "sim_aus_50tip_33area.atlas.txt";
    afn = "earth2.atlas.txt";
    TimeAtlasDataReader tsdr(in_fp + afn,'\t');
    const TimeAtlas* ta = new TimeAtlas(&tsdr);
    size_t numAreas = ta->getNumAreas();
    
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
    double rootAge = trees[0]->getRoot().getAge();
    
    ConstantNode<double> *meanOT    = new ConstantNode<double>("meanOT", new double(rootAge*1.5));
    ConstantNode<double> *stdOT     = new ConstantNode<double>("stdOT", new double(10.0));
    StochasticNode<double> *origin  = new StochasticNode<double>( "origin", new NormalDistribution(meanOT, stdOT) );
    origin->setValue(rootAge);
    StochasticNode<TimeTree> *tau = NULL;
    
    std::vector<RevBayesCore::Taxon> taxa;
    for (size_t i = 0; i < names.size(); ++i)
    {
        taxa.push_back( Taxon( names[i] ) );
    }
    
    if (useBdProcess)
    {
        ConstantNode<double> *div = new ConstantNode<double>("diversification", new double(1.0));
        ConstantNode<double> *turn = new ConstantNode<double>("turnover", new double(0.0));
        ConstantNode<double> *rho = new ConstantNode<double>("rho", new double(1.0));
        
        tau = new StochasticNode<TimeTree>( "tau", new ConstantRateBirthDeathProcess(origin, NULL, div, turn, rho, "uniform", "survival", taxa, std::vector<Clade>()) );
        
    }
    else
    {
        tau = new StochasticNode<TimeTree>("tau", new UniformTimeTreeDistribution(origin, names));
    }
    tau->setValue( trees[0] );
    
    
    // geo distances
    DeterministicNode<GeographyRateModifier>* ddd = NULL;
    ContinuousStochasticNode* dp = NULL;
    ConstantNode<double> *dp_pr = NULL;
    dp_pr = new ConstantNode<double>( "distancePowerPrior", new double(10.0));
    dp = new ContinuousStochasticNode("distancePower", new ExponentialDistribution(dp_pr));
//    ConstantNode<double> *dp_mean   = new ConstantNode<double>("dp_mean", new double(0.0));
//    ConstantNode<double> *dp_sd     = new ConstantNode<double>("dp_sd", new double(1.0));
//    dp  = new ContinuousStochasticNode( "dp", new NormalDistribution(dp_mean, dp_sd) );
//    ConstantNode<double> *dp2 = new ConstantNode<double>("dp2", new double(0.0));
    dp->setValue(new double(0.00001));
    ddd = new DeterministicNode<GeographyRateModifier>("dddFunction", new DistanceDependentDispersalFunction(dp, ta, useAdjacency, useAvailable, useDistances));
    
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
    glr_nonConst[0]->setValue(0.01);
    glr_nonConst[1]->setValue(0.01);
    DeterministicNode< std::vector< double > >* glr_vector = new DeterministicNode< std::vector< double > >( "glr_vector", new VectorFunction< double >( glr ) );
    DeterministicNode<RateMatrix> *q_glr = new DeterministicNode<RateMatrix>( "Q", new FreeBinaryRateMatrixFunction(glr_vector) );
    
    // root frequencies
    // gtr model priors
    ConstantNode<std::vector<double> > *pi_pr = new ConstantNode<std::vector<double> >( "pi_rf", new std::vector<double>(2,1.0) );
    StochasticNode<std::vector<double> > *pi = new StochasticNode<std::vector<double> >( "pi", new DirichletDistribution(pi_pr) );
    
    // cladogenic state frequencies
    std::vector<double> csf_pr_v(3,1.0);
    csf_pr_v[0] = 100.0;
//    ConstantNode<std::vector<double> > *csf_pr = new ConstantNode<std::vector<double> >( "csf_pr", new std::vector<double>(3,1.0) );
    ConstantNode<std::vector<double> > *csf_pr = new ConstantNode<std::vector<double> >( "csf_pr", new std::vector<double>(csf_pr_v) );
    StochasticNode<std::vector<double> > *csf = new StochasticNode<std::vector<double> >( "csf", new DirichletDistribution(csf_pr) );
    if (!useCladogenesis)
    {
        std::vector<double> csf_val(3,1e-6);
        csf_val[0] = 1.0 - (2 * 1e-6);
        csf->setValue(csf_val);
    }
    
    // proportion of areas occupied
//    double f_on = 0.2;
//    double var_on = 0.1;
//    double eff_b = pow(1.0-f_on, 2) * f_on / var_on + f_on - 1;
//    double eff_a = f_on * eff_b / (1.0 - f_on);

    std::vector<double> crm_pr_v;
    crm_pr_v.push_back(5.0);
    crm_pr_v.push_back(50.0);
    ConstantNode<std::vector<double> > *crm_pr = new ConstantNode<std::vector<double> >( "crm_pr", new std::vector<double>( crm_pr_v ));
    StochasticNode<std::vector<double> > *crm = new StochasticNode<std::vector<double> >( "crm", new DirichletDistribution(crm_pr) );

    // Q-map used to compute likehood under the full model
    BiogeographyRateMapFunction* brmf_likelihood = new BiogeographyRateMapFunction(numAreas, forbidExtinction);
    
    brmf_likelihood->setRateMatrix(q_glr);
    brmf_likelihood->setClockRate(clockRate);
    if (useDistances || useAvailable || useAdjacency)
        brmf_likelihood->setGeographyRateModifier(ddd);
    if (useRootFreqs)
        brmf_likelihood->setRootFrequencies(pi);
    
    DeterministicNode<RateMap> *q_likelihood = new DeterministicNode<RateMap>("Q_like", brmf_likelihood);

    
    // Q-map used to sample path histories
    BiogeographyRateMapFunction* brmf_sample = new BiogeographyRateMapFunction(numAreas, false);
    brmf_sample->setRateMatrix(q_glr);
    brmf_sample->setClockRate(clockRate);
    if (useDistances || useAvailable || useAdjacency)
        brmf_sample->setGeographyRateModifier(ddd);
    if (useRootFreqs)
        brmf_likelihood->setRootFrequencies(pi);
    
    DeterministicNode<RateMap> *q_sample = new DeterministicNode<RateMap>("Q_sample", brmf_sample);
    
    
   
    
    // and the character model
    BiogeographicTreeHistoryCtmc<StandardState, TimeTree> *biogeoCtmc = new BiogeographicTreeHistoryCtmc<StandardState, TimeTree>(tau, 2, numAreas, usingAmbiguousCharacters, forbidExtinction, useCladogenesis);
    biogeoCtmc->setRateMap(q_likelihood);
    biogeoCtmc->setCladogenicStateFrequencies(csf);
    if (data.size() == 2 && usingAmbiguousCharacters)
        biogeoCtmc->setTipProbs( data[1] );
    
    StochasticNode< AbstractCharacterData > *charactermodel = new StochasticNode< AbstractCharacterData >("ctmc", biogeoCtmc );
    
    // simulated data
    simulate = true;
    if (simulate)
    {
        charactermodel->redraw();
//        charactermodel->clamp( &biogeoCtmc->getValue() );
    }
    // real data
    else
    {
        // clamp calls setValue, which calls redrawValue, which calls simulate
        charactermodel->clamp( data[0] );
    }
    // initialize mapping
//    charactermodel->redraw();
    
    
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
    //    TopologyNode* nd = NULL; // &tau->getValue().getNode(60);
    
//	moves.push_back( new RootTimeSlide( tau, 50.0, true, 5.0 ) );
//	moves.push_back( new OriginTimeSlide( origin, tau, 50.0, true, 5.0 ) );
//	moves.push_back( new NodeTimeSlideUniform( tau, numNodes*2 ) );
//    
    
    if (useClock)
    {
        moves.push_back( new MetropolisHastingsMove( new ScaleProposal(clockRate, 0.25), 1.0, false) );
        moves.push_back( new MetropolisHastingsMove( new ScaleProposal(clockRate, 0.1), 2.0, false) );
    }
    
    if (useDistances)
    {
//        moves.push_back( new MetropolisHastingsMove( new ScaleProposal(dp, 0.1), 2.0, !true ) );
//        moves.push_back( new MetropolisHastingsMove( new ScaleProposal(dp, 0.5), 4.0, !true ) );
        moves.push_back( new SlidingMove(dp, 0.5, false, 5.0 ));
    }
    
    if (useRootFreqs)
    {
        moves.push_back( new SimplexMove( pi, 20.0, 1, 0, true, 2.0 ) );
        moves.push_back( new SimplexMove( pi, 100.0, 2, 0, true, 2.0 ) );
    }
    
    if (useCladogenesis)
    {
        moves.push_back( new SimplexMove( csf, 250.0, 3, 0, true, 2.0, 1.0 ) );
        moves.push_back( new SimplexMove( csf, 200.0, 1, 0, false, 1.0 ) );
    }
    
    moves.push_back( new VectorScaleMove(glr_stoch, 0.5, false, 2.0));
    moves.push_back( new VectorScaleMove(glr_stoch, 0.1, false, 4.0));
    for( size_t i=0; i<2; i++)
    {
        moves.push_back( new MetropolisHastingsMove( new ScaleProposal(glr_nonConst[i], 0.1), 2.0, !true ) );
        moves.push_back( new MetropolisHastingsMove( new ScaleProposal(glr_nonConst[i], 0.5), 4.0, !true ) );
    
    }
    
    // path
    moves.push_back(new PathRejectionSampleMove<StandardState, TimeTree>(charactermodel, tau, q_sample, new BiogeographyPathRejectionSampleProposal<StandardState,TimeTree>(charactermodel, tau, q_sample, 0.2), 0.2, false, numNodes * 2));
    moves.push_back(new PathRejectionSampleMove<StandardState, TimeTree>(charactermodel, tau, q_sample, new BiogeographyPathRejectionSampleProposal<StandardState,TimeTree>(charactermodel, tau, q_sample, 0.8), 0.8, false, numNodes));
    
    // node
    moves.push_back(new PathRejectionSampleMove<StandardState, TimeTree>(charactermodel, tau, q_sample, new BiogeographyNodeRejectionSampleProposal<StandardState,TimeTree>(charactermodel, tau, q_sample, 0.05), 0.05, false, numNodes*2));
    moves.push_back(new PathRejectionSampleMove<StandardState, TimeTree>(charactermodel, tau, q_sample, new BiogeographyNodeRejectionSampleProposal<StandardState,TimeTree>(charactermodel, tau, q_sample, 0.3), 0.3, false, numNodes));
    moves.push_back(new PathRejectionSampleMove<StandardState, TimeTree>(charactermodel, tau, q_sample, new BiogeographyNodeRejectionSampleProposal<StandardState,TimeTree>(charactermodel, tau, q_sample, 0.8), 0.8, false, numNodes));

    
    
    
    ////////////
    // monitors
    ////////////
    
    std::cout << "Adding monitors\n";
    RbVector<Monitor> monitors;
    
    std::set<DagNode*> monitoredNodes;
    monitoredNodes.insert(clockRate);
    if (useDistances)
        monitoredNodes.insert( dp );
    monitoredNodes.insert( glr_vector );
    if (useRootFreqs)
        monitoredNodes.insert( pi );
//    if (useCladogenesis)
        monitoredNodes.insert( csf );
    
    monitors.push_back(new FileMonitor(monitoredNodes, 10, filepath + "rb" + ss.str() + ".parameters.txt", "\t"));
    monitors.push_back(new ScreenMonitor(monitoredNodes, 10, "\t" ) );
    monitors.push_back(new TreeCharacterHistoryNodeMonitor<StandardState,TimeTree>(charactermodel, tau, 50, filepath + "rb" + ss.str() + ".events.txt", "\t"));
    monitors.push_back(new TreeCharacterHistoryNodeMonitor<StandardState,TimeTree>(charactermodel, tau, 50, filepath + "rb" + ss.str() + ".counts.txt", "\t", true, true, true, false, true, true, false));
    monitors.push_back(new TreeCharacterHistoryNhxMonitor<StandardState,TimeTree>(charactermodel, tau, ta, 50, mcmcGenerations, burn, filepath + "rb" + ss.str() + ".nhx.txt", "\t"));
    
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


















bool TestCharacterHistory::run_mol(void) {
    
    
    ////////////
    // settings
    ////////////
    
    mcmcGenerations *= 1;
    //    mcmcGenerations=35;
    unsigned int burn = (unsigned int)(mcmcGenerations * .2);
    
    ////////////
    // io
    ////////////
    
    std::vector<unsigned> old_seed = GLOBAL_RNG->getSeed();
    std::cout << old_seed[0] << " " << old_seed[1] << "\n";
    std::vector<unsigned> seed;
    seed.push_back(1+1); seed.push_back(1);
    //    old_seed = seed;
        GLOBAL_RNG->setSeed(seed);
    std::stringstream ss;
    ss << ".s0_" << old_seed[0] << ".s1_" << old_seed[1];
    
    bool usingAmbiguousCharacters   = !true;
    bool simulate                   = false;
    bool useClock                   = !true;
    bool useBdProcess               = !true;
    bool forbidExtinction           = true;
    bool useCladogenesis            = true;
    bool useDistances               = true;
    bool useAdjacency               = true;
    bool useAvailable               = true;
    
    filepath="/Users/mlandis/data/bayarea/output/";
    
    // binary characters
    std::string fn = "";
    //    fn = "vireya.nex";
    fn = "psychotria_range.nex";
    //    fn = "16tip_20areas.nex";
    std::string in_fp = "/Users/mlandis/Documents/code/revbayes-code/examples/data/";
    std::vector<AbstractCharacterData*> data = NclReader::getInstance().readMatrices(in_fp + fn);
    std::cout << "Read " << data.size() << " matrices." << std::endl;
    size_t numAreas = data[0]->getNumberOfCharacters();
    
    std::string fn1 = "psychotria_its.nex";
    std::vector<AbstractCharacterData*> data_its = NclReader::getInstance().readMatrices(in_fp + fn1);
    
    std::string fn2 = "psychotria_ets.nex";
    std::vector<AbstractCharacterData*> data_ets = NclReader::getInstance().readMatrices(in_fp + fn2);
    
    // tree
    std::vector<TimeTree*> trees = NclReader::getInstance().readTimeTrees( in_fp + fn );
    std::cout << "Read " << trees.size() << " trees." << std::endl;
    std::cout << trees[0]->getNewickRepresentation() << std::endl;
    size_t numNodes = trees[0]->getNumberOfNodes();
    
    // geo by epochs
    std::string afn="";
    //    afn = "malesia_static.atlas.txt";
    //    afn = "hawaii_static.atlas.txt";
    afn = "hawaii_dynamic.atlas.txt";
    TimeAtlasDataReader tsdr(in_fp + afn,'\t');
    const TimeAtlas* ta = new TimeAtlas(&tsdr);
    

    // model dims
    size_t numBranches = 2 * data[0]->getNumberOfTaxa() - 2;
    std::vector<std::string> names = data[0]->getTaxonNames();
    
    
    ///////////////
    // tree model
    ///////////////
    
    double rootAge = trees[0]->getRoot().getAge();
    
    //   Constant nodes
	ConstantNode<double> *dLambda = new ConstantNode<double>("div_rate", new double(1.0 / 5.0));		// Exponential rate for prior on div
	ConstantNode<double> *turnA   = new ConstantNode<double>("turn_alpha", new double(2.0));			// Beta distribution alpha
	ConstantNode<double> *turnB   = new ConstantNode<double>("turn_beta", new double(2.0));				// Beta distribution beta
    ConstantNode<double> *rho     = new ConstantNode<double>("rho", new double(1.0));					// assume 100% sampling for now
//	ConstantNode<double> *meanOT  = new ConstantNode<double>("meanOT", new double(rootAge*1.5));
//	ConstantNode<double> *stdOT   = new ConstantNode<double>("stdOT", new double(rootAge*1.0));
	
	//   Stochastic nodes
//    StochasticNode<double> *origin  = new StochasticNode<double>( "origin", new NormalDistribution(meanOT, stdOT) );
    ConstantNode<double> *origin  = new ConstantNode<double>( "origin", new double(rootAge));
	
	//   Stochastic nodes
    StochasticNode<double> *div   = new StochasticNode<double>("diversification", new ExponentialDistribution(dLambda));
    StochasticNode<double> *turn  = new StochasticNode<double>("turnover", new BetaDistribution(turnA, turnB));
	
	//   Deterministic nodes
	//    birthRate = div / (1 - turn)
	DeterministicNode<double> *birthRate = new DeterministicNode<double>("birth_rate", new BirthRateConstBDStatistic(div, turn));
	//    deathRate = (div * turn) / ( 1 - turn)
	DeterministicNode<double> *deathRate = new DeterministicNode<double>("death_rate", new DeathRateConstBDStatistic(div, turn));
    
	// For some datasets with large root ages, if div>1.0 (or so), the probability is NaN
    //	RandomNumberGenerator* rng = GLOBAL_RNG;
    //	div->setValue(rng->uniform01() / 1.5);
	
	// Birth-death tree
    std::vector<RevBayesCore::Taxon> taxa;
    for (size_t i = 0; i < names.size(); ++i)
    {
        taxa.push_back( Taxon( names[i] ) );
    }
    StochasticNode<TimeTree> *tau = new StochasticNode<TimeTree>( "tau", new ConstantRateBirthDeathProcess(origin, NULL, birthRate, deathRate, rho, "uniform", "nTaxa", taxa, std::vector<Clade>()) );
    
    tau->setValue( trees[0] );
        
    // branch rates
    ConstantNode<double> *a      = new ConstantNode<double>("a", new double(2.0) );
    ConstantNode<double> *b      = new ConstantNode<double>("b", new double(2.0) );
    
	std::vector<const TypedDagNode<double> *> brates;
	std::vector< StochasticNode<double> *> bratesNotConst;
	for( size_t i=0; i<numBranches; i++){
        std::ostringstream brName;
        brName << "br(" << i << ")";
		StochasticNode<double> *tmpRt = new StochasticNode<double>(brName.str(), new GammaDistribution(a, b) );
		brates.push_back(tmpRt);
		bratesNotConst.push_back(tmpRt);
	}
    DeterministicNode< std::vector< double > >* branchRates = new DeterministicNode< std::vector< double > >( "BranchRates", new VectorFunction< double >( brates ) );

    
    /////////////////
    // biogeo model
    /////////////////
    
    // clock
    ConstantNode<double>* clockRate_a = new ConstantNode<double>("clockPrior_A", new double(2.0*rootAge));
    ConstantNode<double>* clockRate_b = new ConstantNode<double>("clockPrior_B", new double(2.0));
    ContinuousStochasticNode* clockRate = new ContinuousStochasticNode("clockRate", new GammaDistribution(clockRate_a, clockRate_b));
    
    // geo distances
    DeterministicNode<GeographyRateModifier>* ddd = NULL;
    ContinuousStochasticNode* dp = NULL;
    ConstantNode<double> *dp_pr = NULL;
    dp_pr = new ConstantNode<double>( "distancePowerPrior", new double(10.0));
    dp = new ContinuousStochasticNode("distancePower", new ExponentialDistribution(dp_pr));
    dp->setValue(new double(0.00001));
    ddd = new DeterministicNode<GeographyRateModifier>("dddFunction", new DistanceDependentDispersalFunction(dp, ta, useAdjacency, useAvailable, useDistances));
    
    // ctmc rates
    ConstantNode<double>* glr_pr = new ConstantNode<double>("glr_pr", new double(10.0));
    //    ConstantNode<double>* glr_pr_a = new ConstantNode<double>("glr_pr_a", new double(1.1));
    //    ConstantNode<double>* glr_pr_b = new ConstantNode<double>("glr_pr_b", new double(11.0));
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
    DeterministicNode<RateMatrix> *q_glr = new DeterministicNode<RateMatrix>( "Q", new FreeBinaryRateMatrixFunction(glr_vector) );
    
    // Q-map used to compute likehood under the full model
    BiogeographyRateMapFunction* brmf_likelihood = new BiogeographyRateMapFunction(numAreas, forbidExtinction);
    brmf_likelihood->setRateMatrix(q_glr);
    brmf_likelihood->setClockRate(clockRate);
    if (useDistances || useAvailable || useAdjacency)
        brmf_likelihood->setGeographyRateModifier(ddd);
    
    DeterministicNode<RateMap> *q_likelihood = new DeterministicNode<RateMap>("Q_like", brmf_likelihood);
    
    // Q-map used to sample path histories
    BiogeographyRateMapFunction* brmf_sample = new BiogeographyRateMapFunction(numAreas, false);
    brmf_sample->setRateMatrix(q_glr);
    brmf_sample->setClockRate(clockRate);
    if (useDistances || useAvailable || useAdjacency)
        brmf_sample->setGeographyRateModifier(ddd);
    
    DeterministicNode<RateMap> *q_sample = new DeterministicNode<RateMap>("Q_sample", brmf_sample);
    
    // and the character model
    BiogeographicTreeHistoryCtmc<StandardState, TimeTree> *biogeoCtmc = new BiogeographicTreeHistoryCtmc<StandardState, TimeTree>(tau, 2, numAreas, usingAmbiguousCharacters, forbidExtinction, useCladogenesis);
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
    //    charactermodel->redraw();
    
    std::cout << "lnL = " << charactermodel->getDistribution().computeLnProbability() << "\n";
    
    
    //////////////////
    // mol ITS model
    //////////////////
    
    // gtr model priors
    ConstantNode<std::vector<double> > *bf_its = new ConstantNode<std::vector<double> >( "bf_its", new std::vector<double>(4,1.0) );
    ConstantNode<std::vector<double> > *e_its  = new ConstantNode<std::vector<double> >( "e_its", new std::vector<double>(6,1.0) );
    
    // then the parameters
    StochasticNode<std::vector<double> > *pi_its = new StochasticNode<std::vector<double> >( "pi_its", new DirichletDistribution(bf_its) );
    StochasticNode<std::vector<double> > *er_its = new StochasticNode<std::vector<double> >( "er_its", new DirichletDistribution(e_its) );
    
    DeterministicNode<RateMatrix> *q_its = new DeterministicNode<RateMatrix>( "Q_its", new GtrRateMatrixFunction(er_its, pi_its) );
    
    GeneralBranchHeterogeneousCharEvoModel<DnaState, TimeTree> *phyloCTMC_its = new GeneralBranchHeterogeneousCharEvoModel<DnaState, TimeTree>(tau, 4, true, data_its[0]->getNumberOfCharacters());
	phyloCTMC_its->setClockRate( branchRates );
    phyloCTMC_its->setRateMatrix( q_its );
    StochasticNode< AbstractCharacterData > *molevol_its = new StochasticNode< AbstractCharacterData >("S_its", phyloCTMC_its );
	molevol_its->clamp( data_its[0] );
    
    
    //////////////////
    // mol ETS model
    //////////////////
    
    // gtr model priors
    ConstantNode<std::vector<double> > *bf_ets = new ConstantNode<std::vector<double> >( "bf_ets", new std::vector<double>(4,1.0) );
    ConstantNode<std::vector<double> > *e_ets  = new ConstantNode<std::vector<double> >( "e_ets", new std::vector<double>(6,1.0) );
    
    // then the parameters
    StochasticNode<std::vector<double> > *pi_ets = new StochasticNode<std::vector<double> >( "pi_ets", new DirichletDistribution(bf_ets) );
    StochasticNode<std::vector<double> > *er_ets = new StochasticNode<std::vector<double> >( "er_ets", new DirichletDistribution(e_ets) );
    
    DeterministicNode<RateMatrix> *q_ets = new DeterministicNode<RateMatrix>( "Q_ets", new GtrRateMatrixFunction(er_ets, pi_ets) );
    
    GeneralBranchHeterogeneousCharEvoModel<DnaState, TimeTree> *phyloCTMC_ets = new GeneralBranchHeterogeneousCharEvoModel<DnaState, TimeTree>(tau, 4, true, data_ets[0]->getNumberOfCharacters());
	phyloCTMC_ets->setClockRate( branchRates );
    phyloCTMC_ets->setRateMatrix( q_ets );
    StochasticNode< AbstractCharacterData > *molevol_ets = new StochasticNode< AbstractCharacterData >("S_ets", phyloCTMC_ets );
	molevol_ets->clamp( data_ets[0] );
    
    ////////////
    // moves
    ////////////
    
    std::cout << "Adding moves\n";
    RbVector<Move> moves;
    
    if (useDistances)
    {
        moves.push_back(new SlidingMove(dp, 0.1, false, 5.0 ));
        moves.push_back(new SlidingMove(dp, 0.3, false, 2.0 ));
    }
    
    moves.push_back( new MetropolisHastingsMove( new ScaleProposal(clockRate, 0.5), 4.0, true));
    moves.push_back( new VectorScaleMove(glr_stoch, 0.25, false, 4.0));
    moves.push_back( new VectorScaleMove(glr_stoch, 0.1, false, 4.0));
    for( size_t i=0; i<2; i++)
    {
        moves.push_back( new MetropolisHastingsMove( new ScaleProposal(glr_nonConst[i], 0.1), 4.0, !true ) );
        moves.push_back( new MetropolisHastingsMove( new ScaleProposal(glr_nonConst[i], 0.25), 4.0, !true ) );
        //        moves.push_back( new SlidingMove( glr_nonConst[i], 0.1, false, 2.0 ));
    }
    
    //            moves.push_back( new MetropolisHastingsMove( new ScaleProposal(glr_nonConst[0], 0.1), 1.0, !true ) );
    
    bool useChMoves = true;
    bool useMhMoves = !true;
    
    if (useChMoves == false)
    {
        ;
    }
    else if (useMhMoves)
    {
        // path
        moves.push_back( new MetropolisHastingsMove( new BiogeographyPathRejectionSampleProposal<StandardState,TimeTree>(charactermodel, tau, q_sample, 0.1), numNodes*2, false));
        moves.push_back( new MetropolisHastingsMove( new BiogeographyPathRejectionSampleProposal<StandardState,TimeTree>(charactermodel, tau, q_sample, 1.0), numNodes, false));
        
        // node
        //        moves.push_back( new MetropolisHastingsMove( new BiogeographyNodeRejectionSampleProposal<StandardState,TimeTree>(charactermodel, tau, q_sample, 0.1), numNodes*2, false));
        //        moves.push_back( new MetropolisHastingsMove( new BiogeographyNodeRejectionSampleProposal<StandardState,TimeTree>(charactermodel, tau, q_sample, 0.3), numNodes, false));
    }
    else if (useCladogenesis)
    {
        // path
        moves.push_back(new PathRejectionSampleMove<StandardState, TimeTree>(charactermodel, tau, q_sample, new BiogeographyPathRejectionSampleProposal<StandardState,TimeTree>(charactermodel, tau, q_sample, 0.1), 0.1, false, numNodes * 4));
        moves.push_back(new PathRejectionSampleMove<StandardState, TimeTree>(charactermodel, tau, q_sample, new BiogeographyPathRejectionSampleProposal<StandardState,TimeTree>(charactermodel, tau, q_sample, 0.9), 0.9, false, numNodes * 2));
        
        // node
        //        BiogeographyPathRejectionSampleProposal<StandardState,TimeTree>* eprsp = new BiogeographyPathRejectionSampleProposal<StandardState,TimeTree>(charactermodel, tau, q_sample, 0.3);
        //        moves.push_back(new PathRejectionSampleMove<StandardState, TimeTree>(charactermodel, tau, q_sample, new BiogeographyNodeRejectionSampleProposal<StandardState,TimeTree>(charactermodel, tau, q_sample, eprsp, 0.2), 0.2, false, numNodes*2));
        
        moves.push_back(new PathRejectionSampleMove<StandardState, TimeTree>(charactermodel, tau, q_sample, new BiogeographyNodeRejectionSampleProposal<StandardState,TimeTree>(charactermodel, tau, q_sample, 0.1), 0.1, false, numNodes*4));
        moves.push_back(new PathRejectionSampleMove<StandardState, TimeTree>(charactermodel, tau, q_sample, new BiogeographyNodeRejectionSampleProposal<StandardState,TimeTree>(charactermodel, tau, q_sample, 0.9), 0.9, false, numNodes*2));
        
    }
    
    else if (!useCladogenesis)
    {
        // path
        moves.push_back(new PathRejectionSampleMove<StandardState, TimeTree>(charactermodel, tau, q_sample, new PathRejectionSampleProposal<StandardState,TimeTree>(charactermodel, tau, q_sample, 0.1), 0.1, false, numNodes * 2));
        moves.push_back(new PathRejectionSampleMove<StandardState, TimeTree>(charactermodel, tau, q_sample, new PathRejectionSampleProposal<StandardState,TimeTree>(charactermodel, tau, q_sample, 1.0), 1.0, false, numNodes));
        
        //        // node
        //        moves.push_back(new PathRejectionSampleMove<StandardState, TimeTree>(charactermodel, tau, q_sample, new NodeRejectionSampleProposal<StandardState,TimeTree>(charactermodel, tau, q_sample, 0.2), 0.2, false, numNodes*2));
    }
    
    //        BiogeographyPathRejectionSampleProposal<StandardState,TimeTree>* eprsp = new BiogeographyPathRejectionSampleProposal<StandardState,TimeTree>(charactermodel, tau, q_sample, 0.3, nd);
    //        moves.push_back(new PathRejectionSampleMove<StandardState, TimeTree>(charactermodel, tau, q_sample, new BiogeographyNodeRejectionSampleProposal<StandardState,TimeTree>(charactermodel, tau, q_sample, eprsp, 0.1, nd), 0.1, false, numNodes*2));
    //        moves.push_back(new PathRejectionSampleMove<StandardState, TimeTree>(charactermodel, tau, q_sample, new BiogeographyNodeRejectionSampleProposal<StandardState,TimeTree>(charactermodel, tau, q_sample, eprsp, 0.3, nd), 0.3, false, numNodes));
    
    //        BiogeographyPathRejectionSampleProposal<StandardState,TimeTree>* eprsp = new BiogeographyPathRejectionSampleProposal<StandardState,TimeTree>(charactermodel, tau, q_sample, 0.3, nd);
    
    
    
    // tree proposals
    moves.push_back( new MetropolisHastingsMove( new ScaleProposal(div, 1.0), 2, true ) );
    moves.push_back( new MetropolisHastingsMove( new ScaleProposal(turn, 1.0), 2, true ) );
	moves.push_back( new SubtreeScale( tau, 5.0 ) );
//	moves.push_back( new TreeScale( tau, 1.0, true, 2.0 ) );
//	moves.push_back( new RootTimeSlide( tau, 50.0, true, 10.0 ) );
    moves.push_back( new NodeTimeSlideUniform( tau, 30.0 ) );
	for( size_t i=0; i<numBranches; i++) {
        moves.push_back( new MetropolisHastingsMove( new ScaleProposal(bratesNotConst[i], log(2.0)), 2, true ) );
	}
    
    // mol_its proposals
    moves.push_back( new SimplexMove( er_its, 450.0, 6, 0, true, 2.0, 1.0 ) );
    moves.push_back( new SimplexMove( pi_its, 250.0, 4, 0, true, 2.0, 1.0 ) );
    moves.push_back( new SimplexMove( er_its, 200.0, 1, 0, false, 1.0 ) );
    moves.push_back( new SimplexMove( pi_its, 100.0, 1, 0, false, 1.0 ) );
	
    // mol_ets proposals
    moves.push_back( new SimplexMove( er_ets, 450.0, 6, 0, true, 2.0, 1.0 ) );
    moves.push_back( new SimplexMove( pi_ets, 250.0, 4, 0, true, 2.0, 1.0 ) );
    moves.push_back( new SimplexMove( er_ets, 200.0, 1, 0, false, 1.0 ) );
    moves.push_back( new SimplexMove( pi_ets, 100.0, 1, 0, false, 1.0 ) );
	
    
    // add some tree stats to monitor
    DeterministicNode<double> *treeHeight = new DeterministicNode<double>("TreeHeight", new TreeHeightStatistic(tau) );
	DeterministicNode<double> *meanBrRate = new DeterministicNode<double>("MeanBranchRate", new MeanVecContinuousValStatistic(branchRates) );
    
    
    
    
    ////////////
    // monitors
    ////////////
    
    std::cout << "Adding monitors\n";
    RbVector<Monitor> monitors;
    
    std::set<DagNode*> monitoredNodes;
    monitoredNodes.insert(clockRate);
    if (useDistances)
        monitoredNodes.insert( dp );
    monitoredNodes.insert( glr_nonConst[0] );
    monitoredNodes.insert( glr_nonConst[1] );
    
    monitoredNodes.insert( meanBrRate );
	monitoredNodes.insert( treeHeight );
	
	monitoredNodes.insert( div );
	monitoredNodes.insert( turn );
	monitoredNodes.insert( birthRate );
	monitoredNodes.insert( deathRate );
	monitoredNodes.insert( pi_its );
    monitoredNodes.insert( er_its );
    monitoredNodes.insert( pi_ets );
    monitoredNodes.insert( er_ets );

	monitoredNodes.insert( branchRates );
    
    monitors.push_back(new FileMonitor(monitoredNodes, 10, filepath + "rb" + ss.str() + ".mcmc.txt", "\t"));
    monitors.push_back(new ScreenMonitor(monitoredNodes, 10, "\t" ) );
    monitors.push_back(new TreeCharacterHistoryNodeMonitor<StandardState,TimeTree>(charactermodel, tau, 100, filepath + "rb" + ss.str() + ".tree_chars.txt", "\t"));
    monitors.push_back(new TreeCharacterHistoryNodeMonitor<StandardState,TimeTree>(charactermodel, tau, 100, filepath + "rb" + ss.str() + ".num_events.txt", "\t", true, true, true, false, true, true, false));
    monitors.push_back(new TreeCharacterHistoryNhxMonitor<StandardState,TimeTree>(charactermodel, tau, ta, 100, mcmcGenerations, burn, filepath + "rb" + ss.str() + ".nhx.txt", "\t"));
    
    monitors.push_back(new FileMonitor(tau, 10, filepath + "rb" + ss.str() + ".trees.txt", "\t"));
    
    //////////
    // model
    //////////
    std::cout << "Instantiating model\n";
    Model myModel = Model(charactermodel);
    
    
    //////////
    // mcmc
    //////////
    
    GLOBAL_RNG->setSeed(old_seed);
    //    glr_nonConst[0]->redraw();
    //    glr_nonConst[1]->redraw();
    
    //    std::cout << "seed " << old_seed[0] << " " << old_seed[1] << "\n";
    
    std::cout << "Instantiating mcmc\n";
    Mcmc myMcmc = Mcmc( myModel, moves, monitors );
    //    myMcmc.setScheduleType("single");
    myMcmc.run(mcmcGenerations);
    myMcmc.printOperatorSummary();

//    TreeTrace<BranchLengthTree> trace = readTreeTrace(filepath + "rb" + ss.str() + ".trees.txt");
//    TreeSummary<BranchLengthTree> summary = TreeSummary<BranchLengthTree>(trace);
//    summary.summarize();
//    summary.printTreeSummary(std::cerr);
    
    
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
    seed.push_back(9); seed.push_back(2);
    //GLOBAL_RNG->setSeed(seed);
    std::stringstream ss;
    ss << ".s0_" << old_seed[0] << ".s1_" << old_seed[1];
    
    ////////////
    // io
    ////////////
    bool simulate = false;
    bool useClock = !true;
    bool forbidExtinction = !true;
    bool usingAmbiguousCharacters = true;
    bool useCladogenesis = false;
    filepath="/Users/mlandis/data/ngene/";
    
    // binary characters
    std::string fn = "dolloplus.nex";
    std::string in_fp = "/Users/mlandis/data/ngene/";
   
    std::vector<AbstractCharacterData*> data = NclReader::getInstance().readMatrices(in_fp + fn);
    std::cout << "Read " << data.size() << " matrices." << std::endl;
    size_t numAreas = data[0]->getNumberOfCharacters();
    
    // tree
    // std::vector<TimeTree*> trees = NclReader::getInstance().readTimeTrees( in_fp + fn );
    std::vector<BranchLengthTree*>* trees_ptr = NclReader::getInstance().readBranchLengthTrees(in_fp + fn); //TEMPORARY FIX, OTHERWISE DOES NOT COMPILE,"nexus");
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
    //clockRate->setValue(new double(1.0));
    
    // tree
    std::vector<std::string> names = data[0]->getTaxonNames();
    ConstantNode<double>* maxBrLen = new ConstantNode<double>("maxBrLen", new double(10e6));
    StochasticNode<BranchLengthTree>* tau = new StochasticNode<BranchLengthTree>("tau_tmp", new UniformBranchLengthTreeDistribution(maxBrLen, names));
    tau->setValue( trees[0] );
    
    // gain loss rates
    ConstantNode<double>* gainRatePrior = new ConstantNode<double>("glr_pr", new double(4.0));
    std::vector<const TypedDagNode<double> *> gainLossRates;
	std::vector< ContinuousStochasticNode *> gainLossRates_nonConst;
    std::vector<StochasticNode<double>* > glr_stoch;

	for( size_t i=0; i<2; i++){
        std::ostringstream glr_name;
        glr_name << "r(" << i << ")";
		ContinuousStochasticNode* tmp_glr = new ContinuousStochasticNode( glr_name.str(), new ExponentialDistribution(gainRatePrior, new ConstantNode<double>("offset", new double(0.0) )));
        tmp_glr->setValue(new double(0.1));
		gainLossRates.push_back( tmp_glr );
		gainLossRates_nonConst.push_back( tmp_glr );
        glr_stoch.push_back(tmp_glr);
	}
    DeterministicNode< std::vector< double > >* glr_vector = new DeterministicNode< std::vector< double > >( "glr_vector", new VectorFunction< double >( gainLossRates ) );
    DeterministicNode<RateMatrix> *q_glr = new DeterministicNode<RateMatrix>( "Q", new FreeBinaryRateMatrixFunction(glr_vector) );
    
    // Q-map used to compute likehood under the full model
    BiogeographyRateMapFunction* brmf_likelihood = new BiogeographyRateMapFunction(numAreas,forbidExtinction);
    brmf_likelihood->setRateMatrix(q_glr);
    brmf_likelihood->setClockRate(clockRate);
    DeterministicNode<RateMap> *q_likelihood = new DeterministicNode<RateMap>("Q_l", brmf_likelihood);
    
    // Q-map used to sample path histories
    BiogeographyRateMapFunction* brmf_sample = new BiogeographyRateMapFunction(numAreas,false);
    brmf_sample->setRateMatrix(q_glr);
    brmf_sample->setClockRate(clockRate);
    DeterministicNode<RateMap> *q_sample = new DeterministicNode<RateMap>("Q_s", brmf_sample);
    
    // and the character model
    BiogeographicTreeHistoryCtmc<StandardState, BranchLengthTree> *biogeoCtmc = new BiogeographicTreeHistoryCtmc<StandardState, BranchLengthTree>(tau, 2, numAreas, usingAmbiguousCharacters, forbidExtinction, useCladogenesis);
    biogeoCtmc->setRateMap(q_likelihood);
    
    if (data.size() == 2 && usingAmbiguousCharacters)
        biogeoCtmc->setTipProbs( data[1] );

    StochasticNode< AbstractCharacterData > *charactermodel = new StochasticNode< AbstractCharacterData >("ctmc", biogeoCtmc );
    
    GLOBAL_RNG->setSeed(old_seed);
//    gainLossRates_nonConst[0]->redraw();
//    gainLossRates_nonConst[1]->redraw();

    
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
    RbVector<Move> moves;
    if (useClock)
    {
        moves.push_back( new MetropolisHastingsMove( new ScaleProposal(clockRate, 0.5), 5, true ) );
        moves.push_back( new MetropolisHastingsMove( new ScaleProposal(clockRate, 0.1), 5, true ) );
    }
    
    
    moves.push_back( new VectorScaleMove(glr_stoch, 0.25, false, 2));
    moves.push_back( new VectorScaleMove(glr_stoch, 0.1, false, 2));
    for( size_t i=0; i<2; i++)
    {
        moves.push_back( new MetropolisHastingsMove( new ScaleProposal(gainLossRates_nonConst[i], 0.5), 2, false ) );
        moves.push_back( new MetropolisHastingsMove( new ScaleProposal(gainLossRates_nonConst[i], 0.1), 2, false ) );
    }
    
    
    TopologyNode* nd = NULL;

    moves.push_back(new PathRejectionSampleMove<StandardState, BranchLengthTree>(charactermodel, tau, q_sample, new PathRejectionSampleProposal<StandardState,BranchLengthTree>(charactermodel, tau, q_sample, 0.5, nd), 0.5, false, numNodes));
    moves.push_back(new PathRejectionSampleMove<StandardState, BranchLengthTree>(charactermodel, tau, q_sample, new PathRejectionSampleProposal<StandardState,BranchLengthTree>(charactermodel, tau, q_sample, 0.1, nd), 0.1, false, numNodes*2));
    
    
    moves.push_back(new PathRejectionSampleMove<StandardState, BranchLengthTree>(charactermodel, tau, q_sample, new NodeRejectionSampleProposal<StandardState,BranchLengthTree>(charactermodel, tau, q_sample, 0.5, nd), 0.5, false, numNodes));
    moves.push_back(new PathRejectionSampleMove<StandardState, BranchLengthTree>(charactermodel, tau, q_sample, new NodeRejectionSampleProposal<StandardState,BranchLengthTree>(charactermodel, tau, q_sample, 0.1, nd), 0.1, false, numNodes*2));
    
    if (usingAmbiguousCharacters)
    {
        moves.push_back(new PathRejectionSampleMove<StandardState, BranchLengthTree>(charactermodel, tau, q_sample, new TipRejectionSampleProposal<StandardState,BranchLengthTree>(charactermodel, tau, q_sample, 0.5, nd), 0.5, false, numNodes));
        moves.push_back(new PathRejectionSampleMove<StandardState, BranchLengthTree>(charactermodel, tau, q_sample, new TipRejectionSampleProposal<StandardState,BranchLengthTree>(charactermodel, tau, q_sample, 0.1, nd), 0.1, false, numNodes*2));
    }
    
    
    ////////////
    // monitors
    ////////////
    
    std::cout << "Adding monitors\n";
    RbVector<Monitor> monitors;
    
    std::set<DagNode*> monitoredNodes;
    if (useClock)
        monitoredNodes.insert(clockRate);

    monitoredNodes.insert( glr_vector );
    
    monitors.push_back(new FileMonitor(monitoredNodes, 100, filepath + "rb_dollo" + ss.str() + ".mcmc.txt", "\t"));
    monitors.push_back(new ScreenMonitor(monitoredNodes, 100, "\t" ) );
    monitors.push_back(new TreeCharacterHistoryNodeMonitor<StandardState,BranchLengthTree>(charactermodel, tau, 100, filepath + "rb_dollo" + ss.str() + ".tree_chars.txt", "\t"));
    monitors.push_back(new TreeCharacterHistoryNodeMonitor<StandardState,BranchLengthTree>(charactermodel, tau, 100, filepath + "rb_dollo" + ss.str() + ".num_events.txt", "\t", true, true, true, false, true, true, false));

    
    
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
//    myMcmc.run(mcmcGenerations);
        myMcmc.run(100);
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

// modified from TestTreeTraceSummary.cpp
TreeTrace<BranchLengthTree> TestCharacterHistory::readTreeTrace(const std::string &fname) {
    // check that the file/path name has been correctly specified
    RbFileManager myFileManager( fname );
    if ( (myFileManager.isFileNamePresent() == false && myFileManager.testDirectory() == false) ||
        (myFileManager.isFileNamePresent() == true  && (myFileManager.testFile() == false || myFileManager.testDirectory() == false)) ){
        std::string errorStr = "";
        myFileManager.formatError(errorStr);
        throw RbException(errorStr);
    }
    
    // are we reading a single file or are we reading the contents of a directory?
    bool readingDirectory = myFileManager.isDirectory();
    if (readingDirectory == true)
        std::cerr << "Recursively reading the contents of a directory" << std::endl;
    else
        std::cerr << "Attempting to read the contents of file \"" << myFileManager.getFileName() << "\"" << std::endl;
    
    // set up a vector of strings containing the name or names of the files to be read
    std::vector<std::string> vectorOfFileNames;
    if (readingDirectory == true) {
        myFileManager.setStringWithNamesOfFilesInDirectory(vectorOfFileNames);
    }
    else {
#       if defined (WIN32)
        vectorOfFileNames.push_back( myFileManager.getFilePath() + "\\" + myFileManager.getFileName() );
#       else
        vectorOfFileNames.push_back( myFileManager.getFilePath() + "/" + myFileManager.getFileName() );
#       endif
    }
    if (readingDirectory == true) {
        std::cerr << "Found " << vectorOfFileNames.size() << " files in directory" << std::endl;
    }
    
    std::vector<TreeTrace<BranchLengthTree> > data;
    
    
    // Set up a map with the file name to be read as the key and the file type as the value. Note that we may not
    // read all of the files in the string called "vectorOfFileNames" because some of them may not be in a format
    // that can be read.
    std::map<std::string,std::string> fileMap;
    for (std::vector<std::string>::iterator p = vectorOfFileNames.begin(); p != vectorOfFileNames.end(); p++) {
        bool hasHeaderBeenRead = false;
        
        /* Open file */
        std::ifstream inFile( fname.c_str() );
        
        if ( !inFile )
            throw RbException( "Could not open file \"" + fname + "\"" );
        
        /* Initialize */
        std::string commandLine;
        std::cerr << "Processing file \"" << fname << "\"" << std::endl;
        
        /* Command-processing loop */
        while ( inFile.good() ) {
            
            // Read a line
            std::string line;
            getline( inFile, line );
            
            // skip empty lines
            //line = StringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
            if (line.length() == 0)
            {
                continue;
            }
            
            
            // removing comments
            if (line[0] == '#') {
                continue;
            }
            
            // splitting every line into its columns
            std::vector<std::string> columns;
            // first, getting the file delimmiter
            std::string delimiter = "\t";
            
            // we should provide other delimiters too
            StringUtilities::stringSplit(line, delimiter, columns);
            
            // we assume a header at the first line of the file
            if (!hasHeaderBeenRead) {
                
                for (size_t j=1; j<columns.size(); j++) {
                    TreeTrace<BranchLengthTree> t = TreeTrace<BranchLengthTree>();
                    
                    std::string parmName = columns[j];
                    t.setParameterName(parmName);
                    t.setFileName(fname);
                    
                    data.push_back( t );
                }
                
                hasHeaderBeenRead = true;
                
                continue;
            }
            
            // adding values to the Tracess
            for (size_t j=1; j<columns.size(); j++) {
                TreeTrace<BranchLengthTree>& t = data[j-1];
                //                std::istringstream stm;
                //                stm.str(columns[j]);
                //                double d;
                //                stm >> d;
                
                NewickConverter c;
                BranchLengthTree *tau = c.convertFromNewick( columns[j] );
                
                //                std::cerr << *tau << std::endl;
                
                t.addObject( *tau );
                
                delete tau;
            }
        }
    }
    
    return data[0];
}
