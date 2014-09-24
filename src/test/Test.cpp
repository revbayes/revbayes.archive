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
#include "RbOptions.h"
#include "Test.h"
#include "TestACLNDPPBranchRates.h"
#include "TestACLNRelaxedClock.h"
#include "TestACLNRatesGen.h"
#include "TestAutocorrelatedBranchHeterogeneousGtrModel.h"
#include "TestBayesFactor.h"
#include "TestBirthDeath.h"
#include "TestCharEvoModelImplementationPerformance.h"
#include "TestCoala.h"
#include "TestConstantPopCoalescent.h"
#include "TestDistributions.h"
#include "TestCharacterHistory.h"
#include "TestDPPRelClock.h"
#include "TestGtrGammaLikelihood.h"
#include "TestGtrGammaModel.h"
#include "TestGtrModel.h"
#include "TestGtrModelFixedTree.h"
#include "TestGtrUnrooted.h"
#include "TestBranchHeterogeneousGtrModel.h"
#include "TestBranchHeterogeneousHkyModel.h"
#include "TestBranchHeterogeneousTamura92Model.h"
#include "TestIndependentClockRates.h"
#include "TestMixtureBranchHeterogeneousGtrModel.h"
#include "TestMixtureModel.h"
#include "TestMultispeciesCoalescent.h"
#include "TestMultispeciesCoalescentCombinatorics.h"
#include "TestMultispeciesCoalescentWithSequences.h"
#include "TestNNI.h"
#include "TestNormalModel.h"
#include "TestPathSampling.h"
#include "TestPomoModel.h"
#include "TestScalingMove.h"
#include "TestSimplexMove.h"
#include "TestSequenceSimulation.h"
#include "TestSlidingMove.h"
#include "TestStrictClockModel.h"
#include "TestTransitionProbabilities.h"
#include "TestTreeTraceSummary.h"
#include "TestUCLNRelaxedClock.h"
#include "Tree.h"
#include "TreeTrace.h"

#ifdef USE_LIB_ARMADILLO
#include "TestAdmixtureGraph.h"
#endif

#include <time.h>
#include <vector>

using namespace RevBayesCore;

Test::Test(void) {
    
}


Test::Test(int c, const char* v[]) : argc(c), argv(v)
{
    
}


Test::~Test() {
    
}



bool Test::performTests(int argc, const char * argv[]) {
    time_t start,end;
    time (&start);
    
    ////////////////
    // Newer tests
    ////////////////
    
    
	// #######
    // TAH: working on relaxed-clock models, setting up consistent test files
	
    /* A DPP relaxed model test */
    try {
		//        TestDPPRelClock testDPPRC = TestDPPRelClock("data/Primates.nex", "data/primates.tree", 100);
        TestDPPRelClock testDPPRC = TestDPPRelClock("data/test_data_clock_gtr.nex", "data/true_calib_clk.tre", 100000);
		
//		testDPPRC.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
	
	/* A strict clock model test */
    try {
        TestStrictClockModel testGMC = TestStrictClockModel("clock_test/test_data_clock_gtr.nex", "clock_test/true_calib_clk.tre", 100000);
		
//		testGMC.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
	/* An independent clock rate model test */
    try {
        TestIndependentClockRates testIRMC = TestIndependentClockRates("clock_test/test_data_clock_gtr.nex", "clock_test/true_calib_clk.tre", 100000);
		
//		testIRMC.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
	/* An autocorrelated rate model test */
    try {
        TestACLNRatesGen testACLNG = TestACLNRatesGen("clock_test/test_data_clock_gtr.nex", "clock_test/true_calib_clk.tre", 100000);
		
//		testACLNG.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }


	/* An autocorrelated rate model test with dpp on node-wise nu */
    try {
        TestACLNDPPBranchRates testACLNDPP = TestACLNDPPBranchRates("clock_test/test_data_clock_gtr.nex", "clock_test/true_calib_clk.tre", 100000);
		
//		testACLNDPP.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
	// #######
	
		    
    // discrete dependence model
    try
    {
        TestCharacterHistory testDdm = TestCharacterHistory("", "", "", 10000);
//        testDdm.run();
    }
    catch (RbException &e)
    {
        std::cout << e.getMessage() << std::endl;
    }
    
    try
    {
        TestPathSampling testPs = TestPathSampling();
        testPs.run();
    }
    catch (RbException &e)
    {
        std::cout << e.getMessage() << std::endl;
    }
    
    
    ////////////////
    // Older tests
    ////////////////
   
    
    /* The transition probability test = "tpt" */
    try {
        TestTransitionProbabilities testTP = TestTransitionProbabilities();
//        testTP.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    /* The sequence simulation test */
    try {
        TestSequenceSimulation test = TestSequenceSimulation("trees/primates.tree");
        //test.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
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
        
//        testGtr.run();
        TestTreeTraceSummary testTrace = TestTreeTraceSummary("TestGtrUnrooted.tree");
//        testTrace.run();
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
    
    
    /* A coalescent model test */
    try {
        TestConstantPopCoalescent testCoal = TestConstantPopCoalescent("trees/cetaceans cytb_final1_mrc.tree");
//        testCoal.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    
    /* A coalescent model test */
    try {
        TestMultispeciesCoalescent testCoal = TestMultispeciesCoalescent("../../examples/data/primates.tree");
//        TestMultispeciesCoalescent testCoal = TestMultispeciesCoalescent("trees/smallTest.tree");
     //  testCoal.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    /* A coalescent model test complete with simulations and inference from sequences */
    try {
        TestMultispeciesCoalescentWithSequences testCoal = TestMultispeciesCoalescentWithSequences("/Users/boussau/sharedFolderLinux/revBayes/revbayes-code-git/examples/data/primates.tree");
//        TestMultispeciesCoalescent testCoal = TestMultispeciesCoalescent("trees/smallTest.tree");
//       testCoal.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    
    /* A coalescent model test */
    try {
        TestMultispeciesCoalescentCombinatorics testCoal = TestMultispeciesCoalescentCombinatorics();
//        testCoal.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    
    /* A Pomo model test */
    try {
        TestPomoModel testPomo = TestPomoModel("/Users/boussau/sharedFolderLinux/revBayes/revbayes-code-git/examples/data/primates.tree", 10, 10000);
         //  testPomo.run();
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
        TestGtrModel testGtr = TestGtrModel("/Users/boussau/sharedFolderLinux/revBayes/revbayes-code-git/examples/data/primates.fas", "/Users/boussau/sharedFolderLinux/revBayes/revbayes-code-git/examples/data/primates.tree", 200);
//        TestGtrModel testGtr = TestGtrModel("data/anolis.nex", "trees/anolis_MAP.dnd", 10000);
  //      TestGtrModel testGtr = TestGtrModel("data/LSU.phy", "trees/LSUrootedClocklike.dnd", 1000);

     //  testGtr.run();
        TestTreeTraceSummary testTrace = TestTreeTraceSummary("TestGtrModel.tree");
//        testTrace.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    /* A GTR+Gamma model test */
    try {
        TestGtrGammaLikelihood testGtrGamma = TestGtrGammaLikelihood("data/primates.nex", "trees/primates.tree");
        testGtrGamma.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    /* A GTR+Gamma model test */
    try {
        TestGtrGammaModel testGtr = TestGtrGammaModel("data/primates.nex", "trees/primates.tree", 10000);
//        testGtr.run();
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
	
    /* A branch-heterogeneous HKY model test */
    try {
        TestBranchHeterogeneousHkyModel testHeteroHky = TestBranchHeterogeneousHkyModel("/Users/boussau/Dropbox/HeterogeneousModelsRevBayes/data/SheffieldBeetles.fasta", 10000);
		
		//testHeteroHky.run();
        TestTreeTraceSummary testTrace = TestTreeTraceSummary("TestHeteroHkyModel.tree");
          //  testTrace.run();
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
    
    
    
    /* A COALA model test */
    try {
        //        TestDPPRelClock testACLNRC = TestDPPRelClock("data/ucln_sim.nex", "data/ucln_sim.tre", 1000);
        TestCoala testCoala = TestCoala("data/simLG_4species_1.phy", 1000);
		
//		testCoala.run();
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

    
    /* A char-evo-model performance test */
    try {
        TestCharEvoModelImplementationPerformance testGtr = TestCharEvoModelImplementationPerformance("data/primates.nex", "trees/primates.tree", 10000);
//		        testGtr.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
#ifdef USE_LIB_ARMADILLO
    // Admixture graph
    try {
        //TestAdmixtureGraph testAG = TestAdmixtureGraph("canmap2_snps_removedMissingData.txt", 10000, "/Users/mlandis/data/admix/input/", "");
        //TestAdmixtureGraph testAG = TestAdmixtureGraph("moo_snps.no_999111.removedMissingData.txt", 10000, "/Users/mlandis/data/admix/input/", "");
        //TestAdmixtureGraph testAG = TestAdmixtureGraph("humans.071713.txt", 10000, "/Users/mlandis/data/admix/input/", "");
        //TestAdmixtureGraph testAG = TestAdmixtureGraph("modern.clovis.130723.txt", 10000, "/Users/mlandis/data/admix/input/", "");
        //TestAdmixtureGraph testAG = TestAdmixtureGraph("modern.saqqaq.130723.txt", 10000, "/Users/mlandis/data/admix/input/", "");
        //TestAdmixtureGraph testAG = TestAdmixtureGraph("modern.malta.130723.txt", 10000, "/Users/mlandis/data/admix/input/", "");
        //TestAdmixtureGraph testAG = TestAdmixtureGraph("modern.939.130723.txt", 10000, "/Users/mlandis/data/admix/input/", "");
        //TestAdmixtureGraph testAG = TestAdmixtureGraph(argc, argv, "small.Modern_Saqqaq_Clovis.genotypes.polymorphic.txt.noMayanAleutsTsimshianAltai.TreeMixInput.txt", 2000, "/Users/mlandis/data/admix/input/", "");
        //TestAdmixtureGraph testAG = TestAdmixtureGraph(argc, argv, "Modern_Saqqaq_Clovis_Malta.fix_haploid.txt", 2000, "/Users/mlandis/data/admix/input/", "");
        TestAdmixtureGraph testAG = TestAdmixtureGraph(argc, argv, "hgdp_humans.txt", 2000, "/Users/mlandis/data/admix/input/", "");
        //testAG.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
#endif
    time (&end);
    double dif = difftime(end,start);
    std::cout << "The tests ran in " << dif << " seconds." << std::endl;
    std::cout << "Finished Tests!!!" << std::endl;
    
    return 0;
}
