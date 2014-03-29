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
#include "BiogeographicTreeHistoryCtmc.h"
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
#include "GeographicDistanceRateModifier.h"
#include "GeographicGridRateModifier.h"
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

//#define USE_DDBD

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
    int numTimes = (int)ta.getTimes().size();
    
    //////////
    // test
    int maxGen = 1000;//(int)((double)mcmcGenerations / 10);
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
    
    size_t numCharacters = pow(5,2);
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
    GeographicDistanceRateModifier* gdrm = new GeographicDistanceRateModifier(geo_coords);
    gdrm->update();
    
    // geographic grid timeatlas
    std::vector<GeographicGridRateModifier*> ggrmv;
    for (size_t i = 0; i < numTimes; i++)
    {
        ggrmv.push_back(new GeographicGridRateModifier(&ta, i));
    }
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
    //gdrm = NULL;
    grsrm = NULL;
    lrsrm = NULL;
    asrm = NULL;
    

    distancePower->setValue(new double(3.0));
    areaStationaryFrequency->setValue(new double(0.2));
    areaPower->setValue(new double(0.5));

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
