//
//  Test.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 7/11/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "BranchLengthTree.h"
#include "ConstantNode.h"
#include "NewickConverter.h"
#include "RbException.h"
#include "Test.h"
#include "TestACLNRelaxedClock.h"
#include "TestAdmixtureGraph.h"
#include "TestAutocorrelatedBranchHeterogeneousGtrModel.h"
#include "TestBayesFactor.h"
#include "TestBirthDeath.h"
#include "TestBirthDeathModelImplementations.h"
#include "TestBrownianMotion.h"
#include "TestDistributions.h"
#include "TestDPPRelClock.h"
#include "TestGtrGammaModel.h"
#include "TestGtrModel.h"
#include "TestGtrModelFixedTree.h"
#include "TestGtrUnrooted.h"
#include "TestBranchHeterogeneousGtrModel.h"
#include "TestBranchHeterogeneousTamura92Model.h"
#include "TestMixtureBranchHeterogeneousGtrModel.h"
#include "TestMixtureModel.h"
#include "TestNumericalIntegration.h"
#include "TestNNI.h"
#include "TestNormalModel.h"
#include "TestRevLanguage.h"
#include "TestScalingMove.h"
#include "TestSimplexMove.h"
#include "TestRateHeterogeneousGtrModel.h"
#include "TestSkyline.h"
#include "TestSlidingMove.h"
#include "TestTreeTraceSummary.h"
#include "TestUCLNRelaxedClock.h"
#include "TestVariableBirthDeath.h"
#include "Tree.h"
#include "TreeTrace.h"

#include <time.h>

using namespace RevBayesCore;

Test::Test() {
    
}


Test::~Test() {
    
}



bool Test::performTests(void) {
    time_t start,end;
    time (&start);
    
    /* The distribution test */
    try {
        TestDistributions test = TestDistributions();
//        test.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    /* The scaling move test */
    try {
        TestScalingMove test = TestScalingMove(10000);
//        test.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    /* The sliding move test */
    try {
        TestSlidingMove test = TestSlidingMove(10000);
//        test.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    /* The simplex move test */
    try {
        TestSimplexMove test = TestSimplexMove(10000);
//        test.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    /* The skyline model test */
    try {
        TestSkyline test = TestSkyline(100000);
//        test.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    /* The Brownian motion test */
    try {
        TestBrownianMotion test = TestBrownianMotion(100000);
//        test.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    
    /* The normal model test */
    try {
        TestNormalModel testNormal = TestNormalModel(10000);
//        testNormal.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    /* A Mixture model test */
    try {
        TestMixtureModel testMixture = TestMixtureModel(10000);
//        testMixture.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    /* A (unrooted) GTR model test */
    try {
        TestGtrUnrooted testGtr = TestGtrUnrooted("data/primates_mtDNA.nex", "trees/primates.tree", 200);
        
        testGtr.run();
        TestTreeTraceSummary testTrace = TestTreeTraceSummary("TestGtrUnrooted.tree");
        testTrace.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    /* A GTR model test */
    try {
        TestGtrModelFixedTree testGtr = TestGtrModelFixedTree("data/primates.nex", "trees/primates.tree", 20);
//        testGtr.run();
        TestTreeTraceSummary testTrace = TestTreeTraceSummary("TestGtrModelFixedTree.tree");
//        testTrace.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    /* A birth-death model test */
    try {
        TestBirthDeath testBD = TestBirthDeath("trees/primates.tree", 100000);
//        testBD.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    /* A NNI test */
    try {
        TestNNI testNNI = TestNNI(100000);
//        testNNI.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    /* A GTR model test */
    try {
//        TestGtrModel testGtr = TestGtrModel("data/primates.nex", "trees/primates.tree", 10000);
        TestGtrModel testGtr = TestGtrModel("data/primates_mtDNA.nex", "trees/primates.tree", 200);
//        TestGtrModel testGtr = TestGtrModel("data/anolis.nex", "trees/anolis_MAP.dnd", 10000);
  //      TestGtrModel testGtr = TestGtrModel("data/LSU.phy", "trees/LSUrootedClocklike.dnd", 1000);

        testGtr.run();
        TestTreeTraceSummary testTrace = TestTreeTraceSummary("TestGtrModel.tree");
        testTrace.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    /* A GTR+Gamma model test */
    try {
        TestGtrGammaModel testGtr = TestGtrGammaModel("data/primates.nex", "trees/primates.tree", 1000);
//        testGtr.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    /* A GTR+Gamma model test */
    try {
        TestRateHeterogeneousGtrModel testGtr = TestRateHeterogeneousGtrModel("data/primates.nex", "trees/primates.tree", 50);
        //testGtr.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    /* A branch-heterogeneous Tamura 1992 model test */
    try {
          TestBranchHeterogeneousTamura92Model testHeteroT92 = TestBranchHeterogeneousTamura92Model("data/primates.nex", "trees/primates.tree", 1000);
        //TestBranchHeterogeneousTamura92Model testHeteroT92 = TestBranchHeterogeneousTamura92Model("data/LSU.phy", "trees/LSUrootedClocklike.dnd", 1000);
        
//        testHeteroT92.run();
        TestTreeTraceSummary testTrace = TestTreeTraceSummary("TestHeteroGtrModel.tree");
//           testTrace.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }

    /* A branch-heterogeneous GTR model test */
    try {
      //  TestBranchHeterogeneousGtrModel testHeteroGtr = TestBranchHeterogeneousGtrModel("data/primates.nex", "trees/primates.tree", 1000);
        TestBranchHeterogeneousGtrModel testHeteroGtr = TestBranchHeterogeneousGtrModel("data/LSU.phy", "trees/LSUrootedClocklike.dnd", 1000);

       // testHeteroGtr.run();
        TestTreeTraceSummary testTrace = TestTreeTraceSummary("TestHeteroGtrModel.tree");
        //    testTrace.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    /* A autocorrelated branch-heterogeneous GTR model test */
    try {
        TestAutocorrelatedBranchHeterogeneousGtrModel testAutocorrHeteroGtr = TestAutocorrelatedBranchHeterogeneousGtrModel("data/primates.nex", "trees/primates.tree", 1000);
//        TestAutocorrelatedBranchHeterogeneousGtrModel testAutocorrHeteroGtr = TestAutocorrelatedBranchHeterogeneousGtrModel("data/LSU.phy", "trees/LSUrootedClocklike.dnd", 1000);
        
//        testAutocorrHeteroGtr.run();
        TestTreeTraceSummary testTrace = TestTreeTraceSummary("TestHeteroGtrModel.tree");
        //    testTrace.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    /* A mixture branch-heterogeneous GTR model test */
    try {
//        TestMixtureBranchHeterogeneousGtrModel testHeteroGtr = TestMixtureBranchHeterogeneousGtrModel("data/primates.nex", "trees/primates.tree", 1000);
        TestMixtureBranchHeterogeneousGtrModel testHeteroGtr = TestMixtureBranchHeterogeneousGtrModel("data/LSU.phy", "trees/LSUrootedClocklike.dnd", 1000);
        
        //testHeteroGtr.run();
        TestTreeTraceSummary testTrace = TestTreeTraceSummary("TestHeteroGtrModel.tree");
        //    testTrace.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }

    
    /* A numerical integration test */
    try {
        TestNumericalIntegration testIntegration = TestNumericalIntegration();
//        testIntegration.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }

    /* A UCLN relaxed model test */
    try {
//        TestUCLNRelaxedClock testUCLNRC = TestUCLNRelaxedClock("data/ucln_sim.nex", "data/ucln_sim.tre", 1000);
        TestUCLNRelaxedClock testUCLNRC = TestUCLNRelaxedClock("data/primates.nex", "trees/primates.tree", 1000);
		
		//testUCLNRC.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    
    
    /* A ACLN relaxed model test */
    try {
        //        TestACLNRelaxedClock testACLNRC = TestACLNRelaxedClock("data/ucln_sim.nex", "data/ucln_sim.tre", 1000);
        TestACLNRelaxedClock testACLNRC = TestACLNRelaxedClock("data/primates.nex", "trees/primates.tree", 1000);
		
		//testACLNRC.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    
    
    /* A DPP relaxed model test */
    try {
        //        TestDPPRelClock testACLNRC = TestDPPRelClock("data/ucln_sim.nex", "data/ucln_sim.tre", 1000);
        TestDPPRelClock testACLNRC = TestDPPRelClock("data/primates.nex", "trees/primates.tree", 1000);
		
		//testACLNRC.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    
    /* A time varying birth-death model test */
    try {
        TestBirthDeathModelImplementations testBD = TestBirthDeathModelImplementations("trees/Sim_2_1.75.tre", 10000);
//        TestBirthDeathModelImplementations testBD = TestBirthDeathModelImplementations("trees/simple.tree", 10000);
//        testBD.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    
    /* A time varying birth-death model test */
    try {
//        TestVariableBirthDeath testBD = TestVariableBirthDeath("trees/AAtimetreeAUTOhard.tre", 100000);
        TestVariableBirthDeath testBD = TestVariableBirthDeath("trees/MammalianPhylogeny.tre", 100000);
//        TestVariableBirthDeath testBD = TestVariableBirthDeath("trees/MammalianPhylogeny2.tre", 100000);
//        TestVariableBirthDeath testBD = TestVariableBirthDeath("trees/Sim_2_1.75.tre", 2000);
//        TestVariableBirthDeath testBD = TestVariableBirthDeath("trees/Sim_2_1.75_large.tre", 10000);
//        testBD.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    
    /* The whole RevLanguage test suite */
    try {
        TestTreeTraceSummary testTrace = TestTreeTraceSummary("TestGtrModel.tree");
//        testTrace.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    
    /* The Bayes factors */
    try {
        TestBayesFactor testBF = TestBayesFactor(10000);
//        testBF.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    try {
//        TestAdmixtureGraph testAG = TestAdmixtureGraph("/Users/mlandis/data/admix/input/21pop.no_edge.ultra.bm.txt", 10);
//        TestAdmixtureGraph testAG = TestAdmixtureGraph("/Users/mlandis/data/admix/input/21pop.21to1.ultra.bm.txt", 10);
//        testAG.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    /* The whole RevLanguage test suite */
    try {
        TestRevLanguage testRL = TestRevLanguage();
        //        testRL.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    time (&end);
    double dif = difftime(end,start);
    std::cout << "The tests ran in " << dif << " seconds." << std::endl;
    std::cout << "Finished Tests!!!" << std::endl;
    
    return 0;
}
