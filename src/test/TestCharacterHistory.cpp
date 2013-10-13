//
//  TestCharacterHistory.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 8/6/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "TestCharacterHistory.h"

// sorted
#include "BetaDistribution.h"
#include "BranchHistory.h"
#include "CharacterEvent.h"
#include "CharacterHistoryCtmcNodeUpdate.h"
#include "CharacterHistoryCtmcPathUpdate.h"
#include "Clade.h"
#include "ConstantBirthDeathProcess.h"
#include "ConstantNode.h"
#include "ContinuousStochasticNode.h"
#include "DeterministicNode.h"
#include "DirichletDistribution.h"
#include "DispersalHistoryCtmc.h"
#include "ExponentialDistribution.h"
#include "FileMonitor.h"
#include "GeographicDistanceRateModifier.h"
#include "Mcmc.h"
#include "Model.h"
#include "Monitor.h"
#include "Move.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "ScaleMove.h"
#include "ScreenMonitor.h"
#include "StochasticNode.h"
#include "TimeTree.h"
#include "TreeCharacterHistory.h"
#include "TestCharacterHistory.h"
#include "UniformDistribution.h"
#include "VectorFunction.h"

// unsorted test headers
#include "RateMatrix_GTR.h"
#include "GtrRateMatrixFunction.h"

using namespace RevBayesCore;


TestCharacterHistory::TestCharacterHistory(const std::string &afn,  const std::string &tfn, const std::string &gfn, int gen, const std::string &fp) : areaFilename( afn ), filepath(fp), treeFilename(tfn), geoFilename(gfn), mcmcGenerations( gen )
{
    ;
}

TestCharacterHistory::TestCharacterHistory(int ac, const char* av[], const std::string &afn, const std::string &tfn, const std::string &gfn, int gen, const std::string &fp) : areaFilename( afn ), filepath(fp), treeFilename(tfn), geoFilename(gfn), mcmcGenerations( gen ), argc(ac), argv(av)
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
    
    //////////
    // files
    //////////
    
    filepath = "/Users/mlandis/data/bayarea/output/";
    geoFilename = "rb.geo.txt";
    areaFilename = "rb.areas.txt";
    treeFilename = "rb.tree.txt";
    
    
    //////////
    // test
    
    std::vector<unsigned int> seed;
    //seed.push_back(3); seed.push_back(3); GLOBAL_RNG->setSeed(seed);
    
    size_t numTaxa = 5;
    size_t numNodes = 2 * numTaxa - 1;
    std::vector<std::string> taxonNames;
    for (size_t i = 0; i < numTaxa; i++)
    {
        std::stringstream ss;
        ss << i;
        taxonNames.push_back("p" + ss.str());
    }
    
    size_t numCharacters = pow(5,2);
    size_t numStates = 2;
        
    std::vector<std::vector<double> > geo_coords;
    std::string geo_type = "line";
    if (geo_type == "square")
    {
        for (size_t i = 0; i < sqrt(numCharacters); i++)
        {
            for (size_t j = 0; j < sqrt(numCharacters); j++)
            {
                std::vector<double> tmp;
                tmp.push_back(i);
                tmp.push_back(j);
                geo_coords.push_back(tmp);
            }
        }
    }
    else if (geo_type == "line")
    {
        for (size_t i = 0; i < numCharacters; i++)
        {
            std::vector<double> tmp;
            tmp.push_back(i);
            tmp.push_back(0);
            geo_coords.push_back(tmp);
        }
    }

    
    //////////
    
    // To use RateMatrix, need to derive class with:
    // - non-reversible
    // - non-scaled (i.e. without rate 1 per row)
    // ... may not need to get transition probabilities using Rao-Teh sampling
    
    // gtr model priors
    ConstantNode<std::vector<double> > *bf = new ConstantNode<std::vector<double> >( "bf", new std::vector<double>(2,1.0) );
    ConstantNode<std::vector<double> > *e = new ConstantNode<std::vector<double> >( "e", new std::vector<double>(2,1.0) );
 
    // gtr model parameters
    StochasticNode<std::vector<double> > *pi = new StochasticNode<std::vector<double> >( "pi", new DirichletDistribution(bf) );
    StochasticNode<std::vector<double> > *er = new StochasticNode<std::vector<double> >( "er", new DirichletDistribution(e) );
    
    pi->setValue( new std::vector<double>(numStates,1.0/numStates) );
    er->setValue( new std::vector<double>(numStates,1.0/numStates) );
    
    DeterministicNode<RateMatrix> *q = new DeterministicNode<RateMatrix>( "Q", new GtrRateMatrixFunction(er, pi) );

    //////////
    
    // substitution rates
    ConstantNode<double>* rateGain_pr = new ConstantNode<double>("r1_pr", new double(1.0));
    ConstantNode<double>* rateLoss_pr = new ConstantNode<double>("r0_pr", new double(1.0));
    StochasticNode<double>* rateGain = new StochasticNode<double>("r1", new ExponentialDistribution(rateGain_pr));
    StochasticNode<double>* rateLoss = new StochasticNode<double>("r0", new ExponentialDistribution(rateLoss_pr));
    rateGain->setValue(new double(.0001));
    rateLoss->setValue(new double(.0002));
    std::vector<const TypedDagNode<double>* > rates;
    rates.push_back(rateLoss);
    rates.push_back(rateGain);
    
    // branch rate
    ConstantNode<double>* branchRate_pr = new ConstantNode<double>("br_pr", new double(1.0));
    StochasticNode<double>* branchRate = new StochasticNode<double>("br", new ExponentialDistribution(branchRate_pr));
    branchRate->setValue(1.0);
    
    // tree
    StochasticNode<double> *div = new StochasticNode<double>("diversification", new UniformDistribution(new ConstantNode<double>("", new double(0.0)), new ConstantNode<double>("", new double(100.0)) ));
    ConstantNode<double> *turn = new ConstantNode<double>("turnover", new double(0.0));
    ConstantNode<double> *rho = new ConstantNode<double>("rho", new double(1.0));
    ConstantNode<std::vector<double> > *met = new ConstantNode<std::vector<double> >("MET",new std::vector<double>() );
    ConstantNode<std::vector<double> > *mep = new ConstantNode<std::vector<double> >("MESP",new std::vector<double>() );
    div->setValue(new double(1.0));
    StochasticNode<TimeTree> *tau = new StochasticNode<TimeTree>( "tau", new ConstantBirthDeathProcess(div, turn, met, mep, rho, "uniform", "survival", int(taxonNames.size()), taxonNames, std::vector<Clade>()) );
    std::cout << tau->getValue().getNewickRepresentation() << "\n";

    // geographical distance power
    ConstantNode<double>* distancePower_pr = new ConstantNode<double>("dist_pow_pr", new double(5.0));
    StochasticNode<double>* distancePower = new StochasticNode<double>("dist_pow", new ExponentialDistribution(distancePower_pr));
    distancePower->setValue(new double(pow(10,-6)));
    
    // geographic distances
    GeographicDistanceRateModifier* gdrm = new GeographicDistanceRateModifier(geo_coords);
    gdrm->update();
    
    std::cout << "--------------\n";
    std::cout << "rateGain       = " << rateGain->getValue() << "\n";
    std::cout << "rateLoss       = " << rateLoss->getValue() << "\n";
    std::cout << "distancePower  = " << distancePower->getValue() << "\n";
    std::cout << "--------------\n";
    
    ///////////////
    // test for a tree character history
    
    // branch rates
    std::vector<const TypedDagNode<double>* > br_vector;
    for (size_t i = 0; i < numNodes; i++)
    {
        std::stringstream ss;
        ss << i;
        StochasticNode<double>* tmp_br = new StochasticNode<double>("br" + ss.str(), new ExponentialDistribution(branchRate_pr));
        tmp_br->setValue(1.0);
        br_vector.push_back(tmp_br);
    }

    // branch histories
    std::vector<const TypedDagNode<BranchHistory>* > bh_vector;
    std::vector<StochasticNode<BranchHistory>* > bh_vector_stochastic;
    for (size_t i = 0; i < numNodes; i++)
    {
        std::stringstream ss;
        ss << i;
        DispersalHistoryCtmc* tmp_ahc = new DispersalHistoryCtmc(q, rates, tau, br_vector[i], distancePower, numCharacters, numStates, i, gdrm);
        //DispersalHistoryCtmc* tmp_ahc = new DispersalHistoryCtmc(q, rates, tau, br_vector[i], distancePower, numCharacters, numStates, i, NULL);
        StochasticNode<BranchHistory>* sn_bh = new StochasticNode<BranchHistory>("bh" + ss.str(), tmp_ahc);
        bh_vector.push_back(sn_bh);
        bh_vector_stochastic.push_back(sn_bh);
    }
    
    // tree history
    TreeCharacterHistory* tch = new TreeCharacterHistory( bh_vector, tau, numCharacters, numStates );
    tch->simulate();
    StochasticNode<BranchHistory>* tr_chm = new StochasticNode<BranchHistory>("tr_model", tch);
    tr_chm->redraw();
    
    
    ///////////////
    
    std::cout << "Adding moves\n";
    std::vector<Move*> moves;
    moves.push_back( new ScaleMove(distancePower, 1.0, true, 5.0) );
    moves.push_back( new ScaleMove(rateGain, 1.0, true, 5.0) );
    moves.push_back( new ScaleMove(rateLoss, 1.0, true, 5.0) );
    for (size_t i = 0; i < br_vector.size(); i++)
    {
        TypedDagNode<double>* br_tdn = const_cast<TypedDagNode<double>* >(br_vector[i]);
        StochasticNode<double>* br_sn = static_cast<StochasticNode<double>* >(br_tdn);
        ;//moves.push_back( new ScaleMove(br_sn, 1.0, true, 2.0) );
    }
        
    for (size_t i = 0; i < bh_vector.size(); i++)
    {
        TypedDagNode<BranchHistory>* bh_tdn = const_cast<TypedDagNode<BranchHistory>* >(bh_vector[i]);
        StochasticNode<BranchHistory>* bh_sn = static_cast<StochasticNode<BranchHistory>* >(bh_tdn);
        moves.push_back( new CharacterHistoryCtmcPathUpdate(bh_sn, 0.2, true, 2.0) );
        if (i >= numTaxa)
            moves.push_back( new CharacterHistoryCtmcNodeUpdate(bh_sn, bh_vector_stochastic, tau, 0.2, true, 2.0));
    }
    
    for (size_t i = numTaxa; i < bh_vector.size(); i++)
    {
        ; // node update for bh
    }
    
    ///////////////
    
    std::cout << "Adding monitors\n";
    std::vector<Monitor*> monitors;

    std::set<DagNode*> monitoredNodes;
    monitoredNodes.insert( distancePower );
    monitoredNodes.insert( rateGain );
    monitoredNodes.insert( rateLoss );
    
    for (size_t i = 0; i < br_vector.size(); i++)
    {
        TypedDagNode<double>* br_tdn = const_cast<TypedDagNode<double>* >(br_vector[i]);
        ;//monitoredNodes.insert( br_tdn );
    }
    
    for (size_t i = 0; i < bh_vector.size(); i++)
    {
        TypedDagNode<BranchHistory>* bh_tdn = const_cast<TypedDagNode<BranchHistory>* >(bh_vector[i]);
        monitoredNodes.insert( bh_tdn );
    }

    
    monitors.push_back( new FileMonitor( monitoredNodes, 10, filepath + "rb.mcmc.txt", "\t" ) );
    monitors.push_back( new ScreenMonitor( monitoredNodes, 1, "\t" ) );
    
    ///////////////
    
    std::cout << "Instantiating model\n";
    Model myModel = Model(tr_chm);

    
    ///////////////
    std::cout << "Instantiating mcmc\n";
    Mcmc myMcmc = Mcmc( myModel, moves, monitors );
    myMcmc.run(mcmcGenerations/100);
    myMcmc.printOperatorSummary();

    
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