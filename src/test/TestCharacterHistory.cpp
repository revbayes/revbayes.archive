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
#include "PhylowoodNhxMonitor.h"
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
    int maxGen = (int)((double)mcmcGenerations / 10);
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
    
    // assign area coordinates
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
          //  std::cout << areaSizes[i] << " ";
        }
       // std::cout << "\n";
    }
    
    
    //////////
    
    // substitution rates
    ConstantNode<double>* rateGain_pr = new ConstantNode<double>("r1_pr", new double(10.0));
    ConstantNode<double>* rateLoss_pr = new ConstantNode<double>("r0_pr", new double(10.0));
    StochasticNode<double>* rateGain = new StochasticNode<double>("r1", new ExponentialDistribution(rateGain_pr));
    StochasticNode<double>* rateLoss = new StochasticNode<double>("r0", new ExponentialDistribution(rateLoss_pr));
 
    // branch rate
    ConstantNode<double>* branchRate_pr = new ConstantNode<double>("br_pr", new double(1.0));
    StochasticNode<double>* branchRate = new StochasticNode<double>("br", new ExponentialDistribution(branchRate_pr));
    
    
    // tree
    StochasticNode<double> *div = new StochasticNode<double>("diversification", new UniformDistribution(new ConstantNode<double>("", new double(0.0)), new ConstantNode<double>("", new double(100.0)) ));
    ConstantNode<double> *turn = new ConstantNode<double>("turnover", new double(0.5));
    ConstantNode<double> *rho = new ConstantNode<double>("rho", new double(1.0));
    ConstantNode<std::vector<double> > *met = new ConstantNode<std::vector<double> >("MET",new std::vector<double>() );
    ConstantNode<std::vector<double> > *mep = new ConstantNode<std::vector<double> >("MESP",new std::vector<double>() );
    StochasticNode<TimeTree>* tau = new StochasticNode<TimeTree>( "tau", new ConstantBirthDeathProcess(div, turn, met, mep, rho, "uniform", "survival", int(taxonNames.size()), taxonNames, std::vector<Clade>()) );
    std::cout << tau->getValue().getNewickRepresentation() << "\n";

    // geographical distance power
    ConstantNode<double>* distancePower_pr = new ConstantNode<double>("dist_pow_pr", new double(2.0));
    StochasticNode<double>* distancePower = new StochasticNode<double>("dist_pow", new ExponentialDistribution(distancePower_pr));
    //distancePower->setValue(new double(pow(10,-6)));
    
    // geographic distances
    GeographicDistanceRateModifier* gdrm = new GeographicDistanceRateModifier(geo_coords);
    gdrm->update();
   
    // range size bd rates
    ConstantNode<double>* extinctionPower_pr = new ConstantNode<double>("ext_pow_pr", new double(1.0));
    ConstantNode<double>* dispersalPower_pr = new ConstantNode<double>("disp_pow_pr", new double(1.0));
    StochasticNode<double>* extinctionPower = new StochasticNode<double>("ext_pow", new ExponentialDistribution(extinctionPower_pr));
    StochasticNode<double>* dispersalPower = new StochasticNode<double>("disp_pow", new ExponentialDistribution(dispersalPower_pr));
    
    ConstantNode<double>* areaStationaryFrequency_pr_a = new ConstantNode<double>("area_freq_pr_a", new double(1.0));
    ConstantNode<double>* areaStationaryFrequency_pr_b = new ConstantNode<double>("area_freq_pr_b", new double(1.0));
    StochasticNode<double>* areaStationaryFrequency = new StochasticNode<double>("area_freq", new BetaDistribution(areaStationaryFrequency_pr_a, areaStationaryFrequency_pr_b));
    
    RangeSizeRateModifier* grsrm = new RangeSizeRateModifier((unsigned int)numCharacters, 1);
    RangeSizeRateModifier* lrsrm = new RangeSizeRateModifier((unsigned int)numCharacters, 1);

    
    
    // area size rate modifiers    
    ConstantNode<double>* areaPower_pr = new ConstantNode<double>("area_pow_pr", new double(1.0));
    StochasticNode<double>* areaPower = new StochasticNode<double>("area_pow", new ExponentialDistribution(areaPower_pr));
    AreaSizeRateModifier* asrm = new AreaSizeRateModifier(areaSizes);
    
    // simulation settings
    
    //gdrm = NULL;
    //grsrm = NULL;
    //lrsrm = NULL;
    //asrm = NULL;
    
    branchRate->setValue(1.0);
    div->setValue(new double(pow(1.0,-2)));
    extinctionPower->setValue(new double(2.0));
    dispersalPower->setValue(new double(2.0));
    distancePower->setValue(new double(2.0));
    areaStationaryFrequency->setValue(new double(0.2));
    areaPower->setValue(new double(0.5));

    rateGain->setValue(new double(2));
    rateLoss->setValue(new double(2));
    std::vector<const TypedDagNode<double>* > rates;
    rates.push_back(rateLoss);
    rates.push_back(rateGain);
    
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
        DispersalHistoryCtmc* tmp_ahc = new DispersalHistoryCtmc(rates, tau, br_vector[i], distancePower, dispersalPower, extinctionPower, areaStationaryFrequency, areaPower, numCharacters, numStates, i, gdrm, grsrm, lrsrm, asrm);
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
    moves.push_back( new ScaleMove(dispersalPower, 1.0, true, 5.0));
    moves.push_back( new ScaleMove(extinctionPower, 1.0, true, 5.0));
    moves.push_back( new BetaSimplexMove(areaStationaryFrequency, 10.0, true, 5.0));
    moves.push_back( new ScaleMove(areaPower, 1.0, true, 5.0) );
    
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
        moves.push_back( new CharacterHistoryCtmcPathUpdate(bh_sn, 0.2, true, 2.0) );
        if (i >= numTaxa)
            moves.push_back( new CharacterHistoryCtmcNodeUpdate(bh_sn, bh_vector_stochastic, tau, 0.2, true, 5.0));
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
    monitoredNodes.insert( dispersalPower );
    monitoredNodes.insert( extinctionPower );
    monitoredNodes.insert( areaStationaryFrequency );
    monitoredNodes.insert( areaPower );
    
    
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
    monitors.push_back( new CharacterHistoryNodeMonitor( tau, bh_vector_stochastic, 10, filepath + "rb.tree_chars.txt", "\t" ));
    unsigned int burn = (unsigned int)(maxGen * .2);
    monitors.push_back( new PhylowoodNhxMonitor( tau, bh_vector_stochastic, geo_coords, 1, maxGen, burn, filepath + "rb.phylowood.txt", "\t" ));
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

    
    /*
    for (size_t i = 0; i < numNodes; i++)
    {
        TypedDagNode<BranchHistory>* bh_tdn = const_cast<TypedDagNode<BranchHistory>* >(bh_vector[i]);
        bh_tdn->getValue().print();
    }
    std::cout << "rateGain " << rateGain->getValue() << "\n";
    std::cout << "rateLoss " << rateLoss->getValue() << "\n";
    */
    
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



/*

 
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


*/