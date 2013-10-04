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
    
    size_t numTaxa = 3;
    size_t numNodes = 2 * numTaxa - 1;
    std::vector<std::string> taxonNames;
    for (size_t i = 0; i < numTaxa; i++)
    {
        std::stringstream ss;
        ss << i;
        taxonNames.push_back("p" + ss.str());
    }
    
    size_t numCharacters = pow(3,2);
    size_t numStates = 2;
    size_t branchIndex = 0;
    
    
    //////////////////////
    // initialize branch history
    
    BranchHistory* bh = new BranchHistory(numCharacters, numStates);
    
    std::multiset<CharacterEvent*,CharacterEventCompare> updateSet;
    std::set<CharacterEvent*> parentSet, childSet;
    std::set<size_t> indexSet;
    
    for (size_t i = 0; i < numCharacters; i++)
        parentSet.insert(new CharacterEvent(i, (i < 4 ? 1 : 0), 0.0));
    
    for (size_t i = 0; i < numCharacters; i++)
        childSet.insert(new CharacterEvent(i, (i < 4 && i != 0? 0 : 1), 1.0));
    
    double dt = 1.0 / (numCharacters+1);
    for (size_t i = 1; i < numCharacters; i++)
    {
        updateSet.insert(new CharacterEvent(i, (i > 3 ? 1 : 0), dt*(i+1)));
    }
    
    for (size_t i = 0; i < numCharacters; i++)
        indexSet.insert(i);
    
    //bh->updateHistory(updateSet, parentSet, childSet, indexSet);
  
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
    rateGain->setValue(new double(1.0));
    rateLoss->setValue(new double(1.0));
    std::vector<const TypedDagNode<double>* > rates;
    rates.push_back(rateLoss);
    rates.push_back(rateGain);
    
    // branch rate
    ConstantNode<double>* branchRate_pr = new ConstantNode<double>("br_pr", new double(1.0));
    StochasticNode<double>* branchRate = new StochasticNode<double>("br", new ExponentialDistribution(branchRate_pr));
    branchRate->setValue(1.0);
    
    // geographical distance powers
    ConstantNode<double>* distancePower_pr = new ConstantNode<double>("dist_pow_pr", new double(10.0));
    StochasticNode<double>* distancePower = new StochasticNode<double>("dist_pow", new ExponentialDistribution(distancePower_pr));
    distancePower->setValue(new double(1.0));
    
    // tree
    StochasticNode<double> *div = new StochasticNode<double>("diversification", new UniformDistribution(new ConstantNode<double>("", new double(0.0)), new ConstantNode<double>("", new double(100.0)) ));
    ConstantNode<double> *turn = new ConstantNode<double>("turnover", new double(0.0));
    ConstantNode<double> *rho = new ConstantNode<double>("rho", new double(1.0));
    ConstantNode<std::vector<double> > *met = new ConstantNode<std::vector<double> >("MET",new std::vector<double>() );
    ConstantNode<std::vector<double> > *mep = new ConstantNode<std::vector<double> >("MESP",new std::vector<double>() );
    StochasticNode<TimeTree> *tau = new StochasticNode<TimeTree>( "tau", new ConstantBirthDeathProcess(div, turn, met, mep, rho, "uniform", "survival", int(taxonNames.size()), taxonNames, std::vector<Clade>()) );

    //////////
    // test distances
    std::vector<std::vector<double> > geo_coords;
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
    
    // create a branch history
    GeographicDistanceRateModifier* gdrm = new GeographicDistanceRateModifier(geo_coords);
    
    /*
    DispersalHistoryCtmc* ahc = new DispersalHistoryCtmc(q, rates, tau, branchRate, distancePower, numCharacters, numStates, 0, gdrm);
    StochasticNode<BranchHistory>* br_chm = new StochasticNode<BranchHistory>("br_model",ahc);
    br_chm->redraw();
    br_chm->getValue().print();
    */

    std::cout << "--------------\n";
    std::cout << "rateGain       = " << rateGain->getValue() << "\n";
    std::cout << "rateLoss       = " << rateLoss->getValue() << "\n";
    std::cout << "distancePower  = " << distancePower->getValue() << "\n";
    //std::cout << "lnL            = " << ahc->computeLnProbability() << "\n";
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
    for (size_t i = 0; i < numNodes; i++)
    {
        std::stringstream ss;
        ss << i;
        DispersalHistoryCtmc* tmp_ahc = new DispersalHistoryCtmc(q, rates, tau, br_vector[i], distancePower, numCharacters, numStates, i, gdrm);
        bh_vector.push_back(new StochasticNode<BranchHistory>("bh" + ss.str(), tmp_ahc));
    }
    
    // tree history
    TreeCharacterHistory* tch = new TreeCharacterHistory( bh_vector, tau, numCharacters, numStates );
    StochasticNode<BranchHistory>* tr_chm = new StochasticNode<BranchHistory>("tr_model", tch);
    tr_chm->redraw();
    
    
    
    
    ///////////////
    
    std::cout << "Adding moves\n";
    std::vector<Move*> moves;
    moves.push_back( new ScaleMove(distancePower, 1.0, true, 2.0) );
    moves.push_back( new ScaleMove(rateGain, 1.0, true, 2.0) );
    moves.push_back( new ScaleMove(rateLoss, 1.0, true, 2.0) );
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
    }
    
    for (size_t i = numTaxa; i < bh_vector.size(); i++)
    {
        ; // node update for bh
    }
    
    ///////////////
    
    std::cout << "Adding moves\n";
    std::vector<Monitor*> monitors;

    std::set<DagNode*> monitoredNodes;
    monitoredNodes.insert( distancePower );
    monitoredNodes.insert( rateGain );
    monitoredNodes.insert( rateLoss );
    
    for (size_t i = 0; i < br_vector.size(); i++)
    {
        TypedDagNode<double>* br_tdn = const_cast<TypedDagNode<double>* >(br_vector[i]);
        monitoredNodes.insert( br_tdn );
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
    myMcmc.run(mcmcGenerations/10);
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

/*
 // MJL 091213:
 // DeterministicNode needs a TypedFunction for structure
 // use RateMatrix as e.g. for implementing RateModifier
 // ... so what is the type of RateModifier??
 
 // Test how to best pass in the vector of rate map modifiers
 //std::vector<const DeterministicNode<RateMapModifier>* > rateMapModifiers;
 //DeterministicNode<RateMapModifier>* geo_dist = new DeterministicNode<RateMapModifier>("geo_dist", new GeoDispersalRateMapModifierFunction(dp, geo_coords));
 //rateMapModifiers.push_back(geo_dist);
 //VectorFunction<RateMapModifier>* vf = new VectorFunction<RateMapModifier>( rateMapModifiers );
 //DeterministicNode<std::vector<RateMapModifier> >* rmm = new DeterministicNode<std::vector<RateMapModifier> >("r8_map_mod", new VectorFunction<RateMapModifier>(rateMapModifiers));
 //DeterministicNode<RateMap>* qm = new DeterministicNode<RateMap>("Qmap", new GeneralRateMapFunction(numStates, numCharacters, er, pi, rmm));
 //DeterministicNode<RateMap>* qm = new DeterministicNode<RateMap>("Qmap", new GeneralRateMapFunction(numStates, numCharacters, er, pi, rateMapModifiers));
 
 // rate modifier functions ... add distance function here
 std::vector<const TypedDagNode<AbstractCharacterHistoryRateModifier*> *> rateMods;
 
 
 
 
 std::vector<CharacterEvent*> currState;
 for (size_t i = 0; i < numCharacters; i++)
 currState.push_back(new CharacterEvent(i, (i < 3 ? 1 : 0), 0.0));
 
 for (size_t i = 0; i < numCharacters; i++)
 {
 CharacterEvent* newState = new CharacterEvent(i, (currState[i]->getState() == 0 ? 1 : 0) ,0.2);
 double d = static_cast<GeoDispersalRateMapModifierFunction*>(&geo_dist->getFunction())->computeRateModifier(currState, newState);
 std::cout << i << " " << currState[i]->getState() << "->" << newState->getState() << " " << d << "\n";
 }*/

// MJL 092313: next up...
// DeterministicNode<RateMap>* qm = new DeterministicNode<RateMap>("Qmap", new GeneralRateMapFunction(numStates, numCharacters, er, pi, rateMapModifiers));




// want something like:
//    rateMods.push_back((new DeterministicNode<RateModifier>("gdrm",new GeoDistRateModFn(dp)));
// where GeoDistRateModFn has base class TypedFunction<RateModifier>
// then GeoDistRateModFn knows its structure from e.g. static_cast< RateMatrix_GTR* >(value)->setStationaryFrequencies( f );


//    DeterministicNode<GeographicDistanceRateModifier*>* geo_dist = new DeterministicNode<GeographicDistanceRateModifier*>*
//    GeographicDistanceRateModifier* geo_dist = new GeographicDistanceRateModifier(dp, geo_coords);

//    rateMods.push_back(new DeterministicNode<AbstractCharacterHistoryRateModifier*>("geo_dist_rm"));

//  std::vector<const TypedDagNode<double>* > gamma_rates = std::vector<const TypedDagNode<double>* >();
//  DeterministicNode<std::vector<double> > *site_rates = new DeterministicNode<std::vector<double> >( "site_rates", new VectorFunction<double>(gamma_rates) );


// VectorFunction< AbstractCharacterHistoryRateModifier* >* vf = new VectorFunction< AbstractCharacterHistoryRateModifier* >( rateMods );
//std::cout << "vf\n";
/*
 DeterministicNode< std::vector<AbstractCharacterHistoryRateModifier*> >* rm_vector = new DeterministicNode< std::vector< AbstractCharacterHistoryRateModifier* > >( "rm_vector", vf);
 std::cout << "rmv\n";
 */
