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
#include "TestFilteredStandardLikelihood.h"
#include "TestAutocorrelatedBranchHeterogeneousGtrModel.h"
#include "TestCoala.h"
#include "TestConstantPopCoalescent.h"
#include "TestCharacterHistory.h"
#include "TestDPPRelClock.h"
#include "TestGtrGammaLikelihood.h"
#include "TestBranchHeterogeneousGtrModel.h"
#include "TestBranchHeterogeneousHkyModel.h"
#include "TestBranchHeterogeneousTamura92Model.h"
#include "TestMixtureBranchHeterogeneousGtrModel.h"
#include "TestMultispeciesCoalescent.h"
#include "TestMultispeciesCoalescentCombinatorics.h"
#include "TestMultispeciesCoalescentWithSequences.h"
#include "TestPathSampling.h"
#include "TestPomoModel.h"
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
    int numPassed = 0;
    int numAttempted = 0;
    
    ////////////////
    // Newer tests
    ////////////////
    
    try {
        numAttempted += 1;
        TestFilteredStandardLikelihood testFSL = TestFilteredStandardLikelihood("data/morpho.nex", "data/morpho.tre");
        if (testFSL.run()) {
            numPassed += 1;
        } else {
            std::cerr << "TestFilteredStandardLikelihood failed!" << std::endl;
        }
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
#if 0 // commenting out broken tests
	   
    
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
    
    /* A GTR+Gamma model test */
    try {
        TestGtrGammaLikelihood testGtrGamma = TestGtrGammaLikelihood("data/primates.nex", "trees/primates.tree");
        testGtrGamma.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    
    /* A branch-heterogeneous Tamura 1992 model test */
    try {
          TestBranchHeterogeneousTamura92Model testHeteroT92 = TestBranchHeterogeneousTamura92Model("data/primates.nex", "trees/primates.tree", 1000);
        //TestBranchHeterogeneousTamura92Model testHeteroT92 = TestBranchHeterogeneousTamura92Model("data/LSU.phy", "trees/LSUrootedClocklike.dnd", 1000);
        
//        testHeteroT92.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }

    /* A branch-heterogeneous GTR model test */
    try {
      //  TestBranchHeterogeneousGtrModel testHeteroGtr = TestBranchHeterogeneousGtrModel("data/primates.nex", "trees/primates.tree", 1000);
        TestBranchHeterogeneousGtrModel testHeteroGtr = TestBranchHeterogeneousGtrModel("data/LSU.phy", "trees/LSUrootedClocklike.dnd", 1000);

       // testHeteroGtr.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
	
    /* A branch-heterogeneous HKY model test */
    try {
        TestBranchHeterogeneousHkyModel testHeteroHky = TestBranchHeterogeneousHkyModel("/Users/boussau/Dropbox/HeterogeneousModelsRevBayes/data/SheffieldBeetles.fasta", 10000);
		
		//testHeteroHky.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    /* A autocorrelated branch-heterogeneous GTR model test */
    try {
        TestAutocorrelatedBranchHeterogeneousGtrModel testAutocorrHeteroGtr = TestAutocorrelatedBranchHeterogeneousGtrModel("data/primates.nex", "trees/primates.tree", 1000);
//        TestAutocorrelatedBranchHeterogeneousGtrModel testAutocorrHeteroGtr = TestAutocorrelatedBranchHeterogeneousGtrModel("data/LSU.phy", "trees/LSUrootedClocklike.dnd", 1000);
        
//        testAutocorrHeteroGtr.run();
    } catch (RbException &e) {
        std::cout << e.getMessage() << std::endl;
    }
    
    /* A mixture branch-heterogeneous GTR model test */
    try {
//        TestMixtureBranchHeterogeneousGtrModel testHeteroGtr = TestMixtureBranchHeterogeneousGtrModel("data/primates.nex", "trees/primates.tree", 1000);
        TestMixtureBranchHeterogeneousGtrModel testHeteroGtr = TestMixtureBranchHeterogeneousGtrModel("data/LSU.phy", "trees/LSUrootedClocklike.dnd", 1000);
        
        //testHeteroGtr.run();
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

#endif // commenting out broken tests
    time (&end);
    double dif = difftime(end,start);
    std::cout << "The tests ran in " << dif << " seconds." << std::endl;
    if (numPassed == numAttempted) {
        std::cout << "Finished Tests!!!" << std::endl;
        return true;
    }
    std::cout << "Unfortunately, only " << numPassed << " out of " << numAttempted << " tests passed." << std::endl;
    return false;

}
